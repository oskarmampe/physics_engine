// Basic Sphere Engine. Creates a ball, which bounces up and down.

#include <Windows.h>

#include <GL/glui.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <cmath>
#include <time.h>

#include "Camera.h"
#include "Plane.h"
#include "Sphere.h"

// Application settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float FPS = 60;
int counter = 0;

// Sphere Settings
float sphere_x = 3;
float sphere_y = 6;
float sphere_z = 4;
float restitution = 0.7;

// Colors
GLfloat RED[] = { 1, 0, 0 };
GLfloat WHITE[] = { 1, 1, 1 };

// Creating the scene.
Plane ground_plane(8, 8);
Sphere spheres[] = {
  Sphere(1.5, RED, sphere_x, sphere_y, sphere_z, restitution)
};
Camera camera(glm::vec3(20.0f, 50.0f, 110.0f));

// Required for event handling
float last_x = SCR_WIDTH / 2.0f;
float last_y = SCR_HEIGHT / 2.0f;
bool first_mouse = true;

// Required for timing
float delta_time = 0.0f;	// time between current frame and last frame
float last_frame = 0.0f;
int initial_time = time(NULL), final_time, frame_count = 0;

bool paused = false;

// Menu variables
static int main_window;
static int sub_window;
static int menu_id;
static int submenu_id;
static int value = 0;

// GLUI Object
GLUI *glui;

// An event handler for the menu. Takes a menu entry id, and then executes the correct corresponding action to its id.
void menu(int num) 
{
	// If the pressed ID menu matches that of 0, then quit the application.
	if (num == 0) 
	{
		glutDestroyWindow(main_window);
		exit(0);
	}
	// Otherwise assign the value to be reused for other functions.
	else {
		value = num;
	}
	glutPostRedisplay();
}

// Create a glut menu for basic operations.
void create_menu(void) {
	// Create first sub menu for basic play operations
	submenu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Play", 2);
	glutAddMenuEntry("Pause", 3);
	glutAddMenuEntry("Stop", 0);
	glutAddMenuEntry("Restart", 4);     
	// A menu for additional settings, like opening a window where you can set ball position etc.
	menu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Settings", 1);
	glutAddSubMenu("Control", submenu_id);
	// Attach that to right click mouse button.
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Initial settings to be applied to the application.
void init() {
	// Get the initial delta time.
	delta_time = 0;

	// Settings for basic lightning.
	glEnable(GL_DEPTH_TEST);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
	glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
	glMaterialf(GL_FRONT, GL_SHININESS, 30);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Create the ground plane.
	ground_plane.create();
}


// A function that handles displaying the next frame.
void display() {
	// Clear the colour buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Assign the viewport.
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glMatrixMode(GL_PROJECTION);

	// Assign the projection matrix.
	glLoadIdentity();
	gluPerspective(camera.zoom, GLfloat(SCR_WIDTH) / GLfloat(SCR_HEIGHT), 1.0, 150.0);

	// Assign the model matrix.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(camera.position.x, camera.position.y, camera.position.z,
		ground_plane.x_midpoint(), 0.0, ground_plane.z_midpoint(),
		0.0, 1.0, 0.0);

	// Rotate the scene if needed
	glRotatef(camera.vertical, 1, 0, 0);
	glRotatef(camera.horizontal, 0, 1, 0);

	// Draw the plane.
	ground_plane.draw();

	//Check for value changed through GLUT menu.
	if (value == 2) // RESUME
	{
		paused = false;
	}
	else if (value == 3) // PAUSE
	{
		paused = true;
	}
	else if (value == 4) // RESTART
	{
		spheres[0].reset(sphere_x, sphere_y, sphere_z, restitution);
		if (!paused)
			value = 2;
		else
			value = 3;
	}
	else if (value == 1)
	{
		glui->show();
	}

	// Draw spheres if there are more than one.
	for (int i = 0; i < sizeof spheres / sizeof(Sphere); i++) {
		spheres[i].update(delta_time, paused);
	}

	// Flush the buffers.
	glFlush();
	glutSwapBuffers();
}

// Requests to draw the next frame.
void timer(int v) {
	// Delta time in seconds.
	float t = glutGet(GLUT_ELAPSED_TIME);
	delta_time = t - last_frame;
	last_frame = t;
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, timer, 0);
}


// A fuction that handles any keyboard key presses.
// The only function available in this application is the ability to move the camera around.
void keyboard(int key, int, int) {
	switch (key) {
		case GLUT_KEY_LEFT: camera.keyboard(LEFT, delta_time); break;
		case GLUT_KEY_RIGHT: camera.keyboard(RIGHT, delta_time); break;
		case GLUT_KEY_UP: camera.keyboard(UP, delta_time); break;
		case GLUT_KEY_DOWN: camera.keyboard(DOWN, delta_time); break;
	}
}


// A function that handles any mouse key press
void mouse(int button, int state, int mouse_x, int mouse_y)
{
	// A simple check if the button is pressed for the first time. This is to prevent from awkward camera movement.
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		first_mouse = true;
	}
}

// Function that handles continous mouse movement, if a mouse button is held. 
void mouse_motion(int mousex, int mousey)
{
	// If first mouse, then reset and forget any value that was there before.
	if (first_mouse)
	{
		last_x = mousex;
		last_y = mousey;
		first_mouse = false;
	}

	// Calculate the offset i.e. how far it has moved.
	float xoffset = mousex - last_x;
	float yoffset = mousey - last_y; 

	// Update the last mouse movement.
	last_x = mousex;
	last_y = mousey;

	// Move the camera and redisplay
	camera.mouse(xoffset, yoffset);
}

// A function that handles wheel motion.
void mouse_wheel(int button, int dir, int x, int y)
{
	camera.mouse_scroll(dir);
	glutPostRedisplay();
}

// A function that handles GLUI mouse click.
void glui_click(int control)
{
	if (control == 0) // UPDATE SPHERE
	{
		spheres[0].reset(sphere_x, sphere_y, sphere_z, restitution);
	}
	if (control == 1) // QUIT
	{
		glui->hide(); // hide the glui
		if (!paused) // change value so it doesn't continually unhide
			value = 2;
		else
			value = 3;
	}
}

// Initializes GLUT and enters the main loop.
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Main GLUT window.
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
	main_window = glutCreateWindow("Bouncing Balls");
	create_menu();

	// Attaching GLUT functions.
	glutDisplayFunc(display);
	glutSpecialFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouse_motion);
	glutMouseWheelFunc(mouse_wheel);
	
	// Creating a GLUI window to allow reposition etc.
	glui = GLUI_Master.create_glui("Sphere Settings", 0);
	glui->set_main_gfx_window(main_window);
	GLUI_EditText *x_edittext = new GLUI_EditText(glui, "X: ", &sphere_x);
	GLUI_EditText *y_edittext = new GLUI_EditText(glui, "Y: ", &sphere_y);
	GLUI_EditText *z_edittext = new GLUI_EditText(glui, "Z: ", &sphere_z);
	GLUI_EditText *surface_edittext = new GLUI_EditText(glui, "Resitution: ", &restitution);
	new GLUI_Button(glui, "Update Sphere", 0, glui_click);
	GLUI_EditText *fps_edittext = new GLUI_EditText(glui, "FPS: ", &FPS);
	new GLUI_Button(glui, "Quit", 1, glui_click);

	glui->hide();

	// Initialise with default settings.
	init();

	// Create a timer function.
	glutTimerFunc(1000 / FPS, timer, 0);

	// Run main game loop.
	glutMainLoop();
}