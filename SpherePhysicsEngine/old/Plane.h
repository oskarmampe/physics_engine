#pragma once
// A plane class.  A plane has alternating red and white
// squares.  The number of squares is set in the constructor.  Each square
// is 1 x 1.  One corner of the board is (0, 0) and the board stretches out
// along positive x and positive z.  It rests on the xz plane.  I put a
// spotlight at (4, 3, 7).

// Colors
GLfloat WHITE[] = { 1, 1, 1 };
GLfloat RED[] = { 1, 0, 0 };
GLfloat GREEN[] = { 0, 1, 0 };
GLfloat MAGENTA[] = { 1, 0, 1 };

class Plane {
	int displayListId;
	int width;
	int depth;
public:
	Plane(int width, int depth) : width(width), depth(depth) {}
	double centerx() { return width / 2; }
	double centerz() { return depth / 2; }
	void create() {
		displayListId = glGenLists(1);
		glNewList(displayListId, GL_COMPILE);
		GLfloat lightPosition[] = { 4, 3, 7, 1 };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glBegin(GL_QUADS);
		glNormal3d(0, 1, 0);
		for (int x = 0; x < width - 1; x++) {
			for (int z = 0; z < depth - 1; z++) {
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, RED);
				glVertex3d(x, 0, z);
				glVertex3d(x + 1, 0, z);
				glVertex3d(x + 1, 0, z + 1);
				glVertex3d(x, 0, z + 1);
			}
		}
		glEnd();
		glEndList();
	}
	void draw() {
		glCallList(displayListId);
	}
};