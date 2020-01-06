#pragma once

#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float SPEED = 10.0f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
	double horiz_angle;
	double vert_angle;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : MovementSpeed(SPEED), Zoom(ZOOM)
	{
		Position = position;
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		std::cout << deltaTime << std::endl;
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position.y += velocity;
		if (direction == BACKWARD)
			Position.y -= velocity;
		if (direction == LEFT)
			Position.x -= velocity;
		if (direction == RIGHT)
			Position.x += velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		horiz_angle += xoffset * 0.5; /* assuming deltas are in pixels */
		vert_angle += yoffset * 0.5;
		if (vert_angle > 90) vert_angle = 90;
		if (vert_angle < -90) vert_angle = -90;
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(int dir)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
		{
			if (dir == -1)
				Zoom += 0.25;
			if (dir == 1)
				Zoom -= 0.25;
		}

		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}
};
#endif
