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
int FPS = 60;
int counter = 0;

// Sphere Settings
float sphere_y = 6;
float sphere_x = 3;
float sphere_z = 4;
float resitution = 1;

// Creating the scene.
Plane ground_plane(8, 8);
Sphere spheres[] = {
  Sphere(1.5, MAGENTA, sphere_y, sphere_x, sphere_z, resitution)
};
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

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

// An event handler for the menu. Takes a menu entry id, and then executes the correct corresponding action to its id.
void menu(int num) {
	if (num == 0) {
		glutDestroyWindow(main_window);
		exit(0);
	}
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
	delta_time = glutGet(GLUT_ELAPSED_TIME);

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(camera.Zoom, GLfloat(SCR_WIDTH) / GLfloat(SCR_HEIGHT), 1.0, 150.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*
	std::cout << "POSITION: X;" <<  camera.Position.x << " Y; " << camera.Position.y << " Z; " << camera.Position.z << std::endl;
	std::cout << "FRONT: X; " << camera.Front.x << " Y; " << camera.Front.y << " Z; " << camera.Front.z << std::endl;
	std::cout << "UP: X; " << camera.Up.x << " Y; " << camera.Up.y << " Z; " << camera.Up.z << std::endl;
	*/
	//gluLookAt(camera.Position.x, camera.Position.y, camera.Position.z,
	//	camera.Front.x + camera.Position.x, camera.Front.y  + camera.Position.y, camera.Front.z + camera.Front.z,
	//	camera.Up.x, camera.Up.y, camera.Up.z);

	gluLookAt(camera.Position.x, camera.Position.y, camera.Position.z,
		ground_plane.centerx(), 0.0, ground_plane.centerz(),
		0.0, 1.0, 0.0);

	glRotatef(camera.vert_angle, 1, 0, 0);
	glRotatef(camera.horiz_angle, 0, 1, 0);

	ground_plane.draw();
	if (value == 2)
	{
		paused = false;
	}
	else if (value == 3)
	{
		paused = true;
	}
	else if (value == 4)
	{
		spheres[0].reset(sphere_y, sphere_x, sphere_z, resitution);
		value = 2;
	}
	for (int i = 0; i < sizeof spheres / sizeof(Sphere); i++) {
		spheres[i].update(delta_time, paused);
	}

	glFlush();
	glutSwapBuffers();

	frame_count++;
	final_time = time(NULL);

	if (final_time - initial_time > 0)
	{
		std::cout << "FPS: " << frame_count / (final_time - initial_time) << std::endl;
		frame_count = 0;
		initial_time = final_time;
	}
}


// Reshape functions that constructs a camera with the correct orientation.
void reshape(GLint w, GLint h) {
	//glViewport(0, 0, w, h);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(camera.Zoom, GLfloat(w) / GLfloat(h), 1.0, 150.0);
	//glMatrixMode(GL_MODELVIEW);
}

// Requests to draw the next frame.
void timer(int v) {
	/* Delta time in seconds. */
	float t = glutGet(GLUT_ELAPSED_TIME);
	delta_time = (t - last_frame) / 1000.0;
	last_frame = t;
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, timer, 0);
}


// A fuction that handles any keyboard key presses.
// The only function available in this application is the ability to move the camera around.
void keyboard(int key, int, int) {
	switch (key) {
		case GLUT_KEY_LEFT: camera.ProcessKeyboard(LEFT, delta_time); break;
		case GLUT_KEY_RIGHT: camera.ProcessKeyboard(RIGHT, delta_time); break;
		case GLUT_KEY_UP: camera.ProcessKeyboard(FORWARD, delta_time); break;
		case GLUT_KEY_DOWN: camera.ProcessKeyboard(BACKWARD, delta_time); break;
	}

	glutPostRedisplay();
}


// A function that handles any mouse key press
void mouse(int button, int state, int mousex, int mousey)
{
	std::cout << button << std::endl;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		first_mouse = true;
	}
	//if ((button == 3) || (button == 4))
	//{
	//	if (state == GLUT_UP) return;
	//	std::cout << "SCROLL" << std::endl;
	//	camera.ProcessMouseScroll();
	//	glutPostRedisplay();
	//
	//}
	glutPostRedisplay();
}


void mouse_motion(int mousex, int mousey)
{
	std::cout << "LEFT MOUSE" << std::endl;
	if (first_mouse)
	{
		last_x = mousex;
		last_y = mousey;
		first_mouse = false;
	}

	float xoffset = mousex - last_x;
	float yoffset = mousey - last_y; // reversed since y-coordinates go from bottom to top

	last_x = mousex;
	last_y = mousey;

	std::cout << "X OFFSET: " << xoffset << std::endl;
	std::cout << "Y OFFSET: " << yoffset << std::endl;

	camera.ProcessMouseMovement(xoffset, yoffset);

	//if ((button == 3) || (button == 4))
	//{
	//	if (state == GLUT_UP) return;
	//	std::cout << "SCROLL" << std::endl;
	//	camera.ProcessMouseScroll();
	//	glutPostRedisplay();
	//	
	//}
	glutPostRedisplay();
}

// A function that handles wheel motion.
void mouseWheel(int button, int dir, int x, int y)
{
	std::cout << "SCROLL" << std::endl;
	std::cout << y << std::endl;
	std::cout << "DIRECTION: " << dir << std::endl;
	camera.ProcessMouseScroll(dir);
	glutPostRedisplay();
}

// A function that handles GLUI mouse click.
void gluiClick(int control)
{
	if (control == 0) {
		spheres[0].reset(sphere_x, sphere_y, sphere_z, resitution);
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
	glutReshapeFunc(reshape);
	glutSpecialFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouse_motion);
	glutMouseWheelFunc(mouseWheel);
	
	// Creating a GLUI window to allow reposition etc.
	GLUI *glui = GLUI_Master.create_glui("Help on GLUI Widgets", 0);
	glui->set_main_gfx_window(main_window);
	GLUI_EditText *x_edittext = new GLUI_EditText(glui, "X: ", &sphere_x);
	GLUI_EditText *y_edittext = new GLUI_EditText(glui, "Y: ", &sphere_y);
	GLUI_EditText *z_edittext = new GLUI_EditText(glui, "Z: ", &sphere_z);
	GLUI_EditText *surface_edittext = new GLUI_EditText(glui, "Resitution: ", &resitution);
	new GLUI_Button(glui, "Update Sphere", 0, gluiClick);
	GLUI_EditText *fps_edittext = new GLUI_EditText(glui, "FPS: ", &FPS);
	new GLUI_Button(glui, "Quit", 0, (GLUI_Update_CB)exit);

	// Initialise with default settings.
	init();

	// Create a timer function.
	glutTimerFunc(1000 / FPS, timer, 0);

	// Run main game loop.
	glutMainLoop();
}