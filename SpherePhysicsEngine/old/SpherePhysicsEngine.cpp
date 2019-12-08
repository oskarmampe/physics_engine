// This application shows balls bouncing on a checkerboard, with no respect
// for the laws of Newtonian Mechanics.  There's a little spotlight to make
// the animation interesting, and arrow keys move the camera for even more
// fun.

#include <Windows.h>

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <cmath>

#include "Camera.h"
#include "Plane.h"
#include "Sphere.h"



// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


// Global variables: a camera, a checkerboard and some balls.
Plane ground_plane(8, 8);
Sphere spheres[] = {
  Sphere(1, GREEN, 7, 6, 1, 0.5),
  Sphere(1.5, MAGENTA, 6, 3, 4, 0.5),
  Sphere(0.4, WHITE, 5, 1, 7, 0.5)
};

//Camera camera;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

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
	glutAddMenuEntry("Stop", 4);
	glutAddMenuEntry("Restart", 5);     
	menu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Settings", 1);
	glutAddSubMenu("Control", submenu_id);
	glutAddMenuEntry("Quit", 0);     
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

// Draws one frame, the checkerboard then the balls, from the current camera
// position.f
void displaySettings() {
}


// Draws one frame, the checkerboard then the balls, from the current camera
// position.f
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (value == 1)
	{
		// WINDOW 2
		glutInitWindowSize(500, 500);
		glutInitWindowPosition(100, 100);
		sub_window = glutCreateSubWindow(main_window, 0, 0, 400, 200);
		glutDisplayFunc(displaySettings);
		value = -1;
	}
	glLoadIdentity();
	gluLookAt(camera.Position.x, camera.Position.y, camera.Position.z,
		ground_plane.centerx(), 0.0, ground_plane.centerz(),
		0.0, 1.0, 0.0);
	ground_plane.draw();
	for (int i = 0; i < sizeof spheres / sizeof(Sphere); i++) {
		spheres[i].update();
	}
	glFlush();
	glutSwapBuffers();
}


// On reshape, constructs a camera that perfectly fits the window.
void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, GLfloat(w) / GLfloat(h), 1.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
}

// Requests to draw the next frame.
void timer(int v) {
	/* Delta time in seconds. */
	float t = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (t - lastFrame) / 1000.0;
	lastFrame = t;
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, v);
}

// Moves the camera according to the key pressed, then ask to refresh the
// display.
void special(int key, int, int) {
	/*float cameraSpeed = 2.5 * deltaTime;*/
	//switch (key) {
	//	case GLUT_KEY_LEFT: camera.ProcessKeyboard(LEFT, glutGet(GLUT_ELAPSED_TIME)); break;
	//	case GLUT_KEY_RIGHT: camera.ProcessKeyboard(RIGHT, glutGet(GLUT_ELAPSED_TIME)); break;
	//	case GLUT_KEY_UP: camera.ProcessKeyboard(FORWARD, glutGet(GLUT_ELAPSED_TIME)); break;
	//	case GLUT_KEY_DOWN: camera.ProcessKeyboard(BACKWARD, glutGet(GLUT_ELAPSED_TIME)); break;
	//	

	//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	//		camera.ProcessKeyboard(FORWARD, deltaTime);
	//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	//		camera.ProcessKeyboard(BACKWARD, deltaTime);
	//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	//		camera.ProcessKeyboard(LEFT, deltaTime);
	//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	//		camera.ProcessKeyboard(RIGHT, deltaTime);
	//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	//		cameraPos += cameraSpeed * cameraFront;
	//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	//		cameraPos -= cameraSpeed * cameraFront;
	//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	//		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	//		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	//}
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
		camera.ProcessMouseScroll(mousey);
		glutPostRedisplay();
		
	}
	glutPostRedisplay();
}

void mouseWheel(int button, int dir, int x, int y)
{
	std::cout << "SCROLL" << std::endl;
	std::cout << y << std::endl;
	camera.ProcessMouseScroll(10);
	glutPostRedisplay();
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
	glutTimerFunc(100, timer, 0);
	glutMouseFunc(mouse);
	glutMouseWheelFunc(mouseWheel);
	init();
	glutMainLoop();
}