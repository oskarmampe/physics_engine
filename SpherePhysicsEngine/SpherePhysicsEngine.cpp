// This application shows balls bouncing on a checkerboard, with no respect
// for the laws of Newtonian Mechanics.  There's a little spotlight to make
// the animation interesting, and arrow keys move the camera for even more
// fun.

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

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int FPS = 60;
int counter = 0;
float sphere_y = 6;
float sphere_x = 3;
float sphere_z = 4;
float resitution = 1;

// Global variables: a camera, a plane and some balls.
Plane ground_plane(8, 8);
Sphere spheres[] = {
  Sphere(1.5, MAGENTA, sphere_y, sphere_x, sphere_z, resitution)
};

//Camera camera;
// camera
Camera camera(glm::vec3(20.0f, 20.0f, 20.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int initial_time = time(NULL), final_time, frame_count = 0;

bool paused = false;

static int main_window;
static int sub_window;
static int menu_id;
static int submenu_id;
static int value = 0;

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

void createMenu(void) {
	submenu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Play", 2);
	glutAddMenuEntry("Pause", 3);
	glutAddMenuEntry("Stop", 0);
	glutAddMenuEntry("Restart", 4);     
	menu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Settings", 1);
	glutAddSubMenu("Control", submenu_id);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Application-specific initialization: Set up global lighting parameters
// and create display lists.
void init() {
	deltaTime = glutGet(GLUT_ELAPSED_TIME);
	glEnable(GL_DEPTH_TEST);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
	glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
	glMaterialf(GL_FRONT, GL_SHININESS, 30);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	ground_plane.create();
}


// Draws one frame, the plane then the balls, from the current camera
// position.f
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(camera.Position.x, camera.Position.y, camera.Position.z,
		ground_plane.centerx(), 0.0, ground_plane.centerz(),
		0.0, 1.0, 0.0);
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
		spheres[i].update(deltaTime, paused);
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


// On reshape, constructs a camera that perfectly fits the window.
void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(ZOOM, GLfloat(w) / GLfloat(h), 1.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
}

// Requests to draw the next frame.
void timer(int v) {
	/* Delta time in seconds. */
	float t = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (t - lastFrame) / 1000.0;
	lastFrame = t;
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, timer, 0);
}

// Moves the camera according to the key pressed, then ask to refresh the
// display.
void special(int key, int, int) {
	switch (key) {
		case GLUT_KEY_LEFT: camera.ProcessKeyboard(LEFT, deltaTime); break;
		case GLUT_KEY_RIGHT: camera.ProcessKeyboard(RIGHT, deltaTime); break;
		case GLUT_KEY_UP: camera.ProcessKeyboard(FORWARD, deltaTime); break;
		case GLUT_KEY_DOWN: camera.ProcessKeyboard(BACKWARD, deltaTime); break;
	}

	glutPostRedisplay();
}

void mouse(int button, int state, int mousex, int mousey)
{
	std::cout << button << std::endl;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		std::cout << "LEFT MOUSE" << std::endl;
		if (firstMouse)
		{
			lastX = mousex;
			lastY = mousey;
			firstMouse = false;
		}

		float xoffset = mousex - lastX;
		float yoffset = lastY - mousey; // reversed since y-coordinates go from bottom to top

		lastX = mousex;
		lastY = mousey;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}
	if ((button == 3) || (button == 4))
	{
		if (state == GLUT_UP) return;
		std::cout << "SCROLL" << std::endl;
		camera.ProcessMouseScroll();
		glutPostRedisplay();
		
	}
	glutPostRedisplay();
}

void mouseWheel(int button, int dir, int x, int y)
{
	std::cout << "SCROLL" << std::endl;
	std::cout << y << std::endl;
	camera.ProcessMouseScroll();
	glutPostRedisplay();
}

void on_click(int control)
{
	if (control == 0) {
		spheres[0].reset(sphere_x, sphere_y, sphere_z, resitution);
	}
}

// Initializes GLUT and enters the main loop.
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// WINDOW 1
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
	main_window = glutCreateWindow("Bouncing Balls");


	createMenu();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutMouseWheelFunc(mouseWheel);
	
	
	GLUI *glui = GLUI_Master.create_glui("Help on GLUI Widgets", 0);
	glui->set_main_gfx_window(main_window);
	GLUI_EditText *x_edittext = new GLUI_EditText(glui, "X: ", &sphere_x);
	GLUI_EditText *y_edittext = new GLUI_EditText(glui, "Y: ", &sphere_y);
	GLUI_EditText *z_edittext = new GLUI_EditText(glui, "Z: ", &sphere_z);
	GLUI_EditText *surface_edittext = new GLUI_EditText(glui, "Resitution: ", &resitution);
	new GLUI_Button(glui, "Update Sphere", 0, on_click);
	GLUI_EditText *fps_edittext = new GLUI_EditText(glui, "FPS: ", &FPS);
	new GLUI_Button(glui, "Quit", 0, (GLUI_Update_CB)exit);
	init();
	glutTimerFunc(1000 / FPS, timer, 0);
	glutMainLoop();
}