#pragma once
#define _USE_MATH_DEFINES
#include <GL/freeglut.h>

#include <glm/vec2.hpp>

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
	GLfloat gravity = 0.98f;
	glm::vec2 velocity = glm::vec2(0.0f, gravity / 1000);
	GLfloat direction;
public:
	Sphere(double r, GLfloat* c, double h, double x, double z, double cr) :
		radius(r), color(c), maximumHeight(h), direction(-1.0f),
		y(h), x(x), z(z), restitution(cr) {
	}
	void update(double deltaTime, bool paused) {
		if (!paused) {
			if (maximumHeight > radius + 1)
			{
				velocity.y += gravity / 1000;
				
			}
			else
			{
				velocity.y = 0;
			}
			y += direction * velocity.y;
			if (y > maximumHeight) {
				y = maximumHeight;
				velocity.y = 0;
				direction = -1.0f;
			}
			else if (y < radius) {
				y = radius; 
				direction = 1.0f;
				velocity.y *= 0.5;
				maximumHeight -= 1;
			}
			//if (deltaTime > 0.02f)
			//{
			//	deltaTime = 0.02f;
			//}
			//glm::vec2 normal(0, 1);
			//velocity.y += gravity * deltaTime * deltaTime;
			//y += velocity.y;
			//if (y > maximumHeight) {
			//	y = maximumHeight - radius;
			//}
			//else if (y < radius) {
			//	y = radius;
			//	GLfloat d = -2;
			//	velocity = d * (velocity.x * normal.x + velocity.y * normal.y) * normal + velocity;
			//}
		}

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		glTranslated(x, y, z);
		glutSolidSphere(radius, 30, 30);
		glPopMatrix();
	}

	void reset(double nx, double ny, double nz)
	{
		x = nx;
		y = ny;
		z = nz;
		maximumHeight = ny;
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		glTranslated(x, y, z);
		glutSolidSphere(radius, 30, 30);
		glPopMatrix();
	}
};