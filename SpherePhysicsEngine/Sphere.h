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
	double max_height;
	double x;
	double y;
	double z;
	double restitution;
	GLfloat gravity = 0.98f / 10.0f;
	glm::vec2 velocity = glm::vec2(0.0f, 0.0f);
	int direction;
public:
	// A basic constructor, taking in any properties of the sphere.
	Sphere(double r, GLfloat* c, double nx, double ny, double nz, double cr) :
		radius(r), colour(c), x(nx), y(ny), z(nz), restitution(cr), direction(-1), max_height(ny) {}

	// Update the sphere position.
	void update(float delta_time, bool paused) {

		// Divide delta time to make time pass slower.
		delta_time /= 1000.0f;

		// Check if paused, draw, otherwise dont.
		if (!paused) {
			// BASIC COLISION DETECTION
			if (y <= radius && direction == -1) {
				if (velocity.y <= 0) // REVERSE DIRECTION
				{
					y = radius;
					direction = 0;
				}
				else // APPLY RESTITUTION AND CHANGE DIRECTION
				{
					velocity.y *= restitution;
					direction = 1;
					y = radius;
				}
			}
			else // If no colision, apply acceleration
			{
				if (direction == 1) // if direction is upward
				{
					velocity.y -= gravity * delta_time; // GET SLOWER
					y += velocity.y;
					if (velocity.y <= 0)
					{
						direction = -1;
					}
				}
				else if (direction == -1) // if direction is downward
				{
					velocity.y += gravity * delta_time; // GET FASTER
					y -= velocity.y;
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
		max_height = ny;
		direction = -1;

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