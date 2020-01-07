#pragma once
#define _USE_MATH_DEFINES
#include <GL/freeglut.h>

#include <glm/vec2.hpp>
#include <glm/gtc/constants.hpp>

// A Sphere. Has basic properties you'd expect; radius, colour, and position in space.
// Has some additional material properties, such as restitution.
// Lastly, this class contains some properties regarding physics, which, aruguably, should not be stored here i.e. gravity, air_density etc.
class Sphere {
	double radius;
	GLfloat* colour;
	double x;
	double y;
	double z;
	double restitution;
	float* gravity;
	float* air_density;
	float* drag_coeff;
	float velocity;
	int direction;
public:
	// A basic constructor, taking in any properties of the sphere.
	Sphere(double r, GLfloat* c, double nx, double ny, double nz, double cr, float vel, float* grav, float* air, float* drag) :
		radius(r), colour(c), x(nx), y(ny), z(nz), restitution(cr), direction(-1), velocity(vel), gravity(grav), air_density(air), drag_coeff(drag) {}

	// Update the sphere position.
	void update(int delta_time, bool paused) {

		// Divide delta time to make time pass slower.
		float dt = delta_time / 1000.0f;

		// Check if paused, draw, otherwise dont.
		if (!paused) {
			// BASIC COLISION DETECTION
			if (y <= radius && direction == -1) {
				if (velocity <= 0) // REVERSE DIRECTION
				{
					y = radius;
					direction = 0;
				}
				else // APPLY RESTITUTION AND CHANGE DIRECTION
				{
					velocity *= restitution;
					direction = 1;
					y = radius;
				}
			}
			else // If no colision, apply acceleration and any other forces. This uses implicit Euler.
			{
				if (direction == 1) // if direction is upward
				{
					// VELOCITY at t_1
					velocity -= calculate_velocity(dt); // GET SLOWER

					// EULER
					y += velocity + (1/2)*calculate_velocity(dt)*dt;

					// Change direction when velocity becomes 0.
					if (velocity <= 0)
					{
						direction = -1;
					}
				}
				else if (direction == -1) // if direction is downward
				{
					// VELOCITY at t_1
					velocity += calculate_velocity(dt); // GET FASTER

					// EULER
					y -= velocity + (1 / 2)*calculate_velocity(dt)*dt;
				}
			}
		}

		draw_sphere();
	}

	// Reset the sphere back to the specified properties.
	void reset(double nx, double ny, double nz, float cr, float vel)
	{
		x = nx;
		y = ny;
		z = nz;
		restitution = cr;
		direction = -1;
		velocity = vel;

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

	// A separate function to calculate velocity. 
	float calculate_velocity(float dt)
	{
		float hemisphere_area = glm::pi<float>() * radius; // Only get half the area as air resistance effects bottom more than top of sphere.
		// F = density * drag * area * velocity^2 / 2
		float acceleration = sqrt((2*(*gravity))/((*air_density) * (*drag_coeff) * hemisphere_area));
		return acceleration * dt;
	}
};