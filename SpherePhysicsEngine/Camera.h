#pragma once

/**
  *
  * A simple camera class that has a position, zoom level, movement speed and the quantity to rotate around horizontal/vertical axis.
  *
**/
#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

// An enum used for specifying a camera movement. This is abstracted away, so that it works independently of GUI library.
enum Camera_Movement {
	LEFT,
	RIGHT,
	UP,
	DOWN
};


class Camera
{
public:
	// Camera Attributes
	glm::vec3 position;

	// Camera options
	float speed;
	float zoom;
	double horizontal;
	double vertical;

	// Basic constructor with position as vector.
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)) : speed(45.0f), zoom(10.0f)
	{
		position = position;
	}

	// Keyboard event listener, takes in a direction whatever it was represented in the GUI as, and the delta time between frames.
	void keyboard(Camera_Movement direction, float deltaTime)
	{
		// A simple calculation of velocity. Could be improved upon.
		float velocity = speed * deltaTime;
		if (direction == UP)
			position.z += velocity;
		if (direction == DOWN)
			position.z -= velocity;
		if (direction == LEFT)
			position.x -= velocity;
		if (direction == RIGHT)
			position.x += velocity;
	}

	// Mouse click event listener, takes in the offset between each 'press'.
	void mouse(float xoffset, float yoffset)
	{
		// Theta's to rotate around.
		horizontal += xoffset * 0.5;
		vertical += yoffset * 0.5;

		// Constraint the rotation.
		if (vertical > 90) 
			vertical = 90;
		if (vertical < -90) 
			vertical = -90;
	}

	// Mouse scroll event, taking in the direction of the mouse scroll. If it's negatigve then zoom in, otherwise zoom out.
	void mouse_scroll(int dir)
	{
		if (zoom >= 1.0f && zoom <= 45.0f)
		{
			if (dir == -1)
				zoom += 0.25;
			if (dir == 1)
				zoom -= 0.25;
		}

		if (zoom <= 1.0f)
			zoom = 1.0f;

		if (zoom >= 45.0f)
			zoom = 45.0f;
			
	}
};
#endif
