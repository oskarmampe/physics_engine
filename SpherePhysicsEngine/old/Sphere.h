#pragma once
#define _USE_MATH_DEFINES
#include <GL/freeglut.h>

// A ball.  A ball has a radius, a color, and bounces up and down between
// a maximum height and the xz plane.  Therefore its x and z coordinates
// are fixed.  It uses a lame bouncing algorithm, simply moving up or
// down by 0.05 units at each frame.
class Sphere {
	double radius;
	GLfloat* color;
	double maximumHeight;
	double x;
	double y;
	double z;
	double restitution;
	double gravity = 0.98;
	double velocity;
	int direction;
public:
	Sphere(double r, GLfloat* c, double h, double x, double z, double cr) :
		radius(r), color(c), maximumHeight(h), direction(-1),
		y(h), x(x), z(z), restitution(cr) {
	}
	void update() {
		velocity += gravity/1000;
		y += direction * velocity;
		if (y > maximumHeight) {
			maximumHeight *= restitution;
			y = maximumHeight;
			direction = -1;
		}
		else if (y < radius) {
			y = radius; direction = 1;
		}
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		glTranslated(x, y, z);
		glutSolidSphere(radius, 30, 30);
		glPopMatrix();
	}
};