#pragma once
/**
  *
  * A class representing a plane that has a corner at (0,0) stretching out along positive x,z axis, meaning that it rests on these axes.
  * A light is also added to illuminate the scene, making it easier to see motion.
  *
**/

GLfloat BLUE[] = { 0, 0, 1 };

class Plane {
	// Plane properties.
	int id;
	int width;
	int depth;

public:
	// Simple setter constructor.
	Plane(int width, int depth) : width(width), depth(depth) {}

	// Functinos to get centers.
	double x_midpoint() 
	{ 
		return width / 2; 
	}

	double z_midpoint() 
	{
		return depth / 2; 
	}

	// Create the actual plane and display it.
	void create() {

		// Creating OPENGL Id
		id = glGenLists(1);
		glNewList(id, GL_COMPILE);
		
		// Creating light.
		GLfloat light[] = { 4, 3, 7, 1 };
		glLightfv(GL_LIGHT0, GL_POSITION, light);

		// Begin creating the plane, which is a square/quad.
		glBegin(GL_QUADS);

		// Surface Normal
		glNormal3d(0, 1, 0);

		// Create all the required vertices.
		for (int i = 0; i < width - 1; ++i) {
			for (int j = 0; j < depth - 1; ++j) {
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, BLUE);
				glVertex3d(i, 0, j);
				glVertex3d(i + 1, 0, j);
				glVertex3d(i + 1, 0, j + 1);
				glVertex3d(i, 0, j + 1);
			}
		}
		glEnd();
		glEndList();
	}

	// OpenGL method to draw the plane.
	void draw() 
	{
		glCallList(id);
	}
};