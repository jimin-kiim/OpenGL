#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	void updateMouseInput(float xoffset, float yoffset);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();

	glm::mat4 getViewMatrix() const;

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw; // horizontal angle
	GLfloat pitch; // vertical angle 
	GLfloat distance;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();
};

