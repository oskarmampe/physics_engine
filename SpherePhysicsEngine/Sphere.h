#pragma once
#define _USE_MATH_DEFINES
#include <GL/freeglut.h>

#include <glm/vec2.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

// A Sphere. Has basic properties you'd expect; radius, colour, and position in space.
// Has some additional material properties, such as restitution.
// Lastly, this class contains some properties regarding physics, which, aruguably, should not be stored here i.e. gravity, velocity.
class Sphere {
	double radius;
	GLfloat* colour;
	double maximumHeight;
	double x;
	double y;
	double z;
	double restitution;
	GLfloat gravity = 0.98f / 1000.0f;
	glm::vec2 velocity = glm::vec2(0.0f, 0);
	GLfloat direction;
public:
	// A basic constructor, taking in any properties of the sphere.
	Sphere(double r, GLfloat* c, double h, double x, double z, double cr) :
		radius(r), colour(c), maximumHeight(h), direction(-1.0f),
		y(h), x(x), z(z), restitution(cr) {
	}

	// Update the sphere position.
	void update(float delta_time, bool paused) {

		// Check if paused, draw, otherwise dont.
		if (!paused) {
			if (y <= radius && direction == -1) {
				if (velocity.y <= 0)
				{
					y = radius;
					direction = 0;
				}
				else
				{
					velocity.y *= restitution;
					direction = 1;
					y = radius;
				}
			}
			else
			{
				if (direction == 1)
				{
					velocity.y -= gravity;
					y += direction * velocity.y;
					if (velocity.y <= 0)
					{
						direction = -1;
					}
				}
				else if (direction == -1)
				{
					velocity.y += gravity;
					y += direction * velocity.y;
					/*velocity.y += gravity * delta_time * delta_time;
					y += direction * velocity.y * delta_time;*/
				}
			}
		}

		draw_sphere();
	}

	// Reset the sphere back to the specified properties.
	void reset(double nx, double ny, double nz, float cr)
	{
		x = nx;
		y = ny;
		z = nz;
		restitution = cr;
		maximumHeight = ny;

		draw_sphere();
	}

	// Draw the sphere through GLUT solid sphere.
	void draw_sphere()
	{
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colour);
		glTranslated(x, y, z);
		glutSolidSphere(radius, 30, 30);
		glPopMatrix();
	}
};