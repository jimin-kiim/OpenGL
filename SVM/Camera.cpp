#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	// position = startPosition;
	worldUp = startUp;
	// yaw = startYaw;
	// pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	// distance = 3.0f;

	position = glm::vec3(0.0f, 0.0f, 3.0f); // Start 3 units away
    yaw = -90.0f;  // Face towards -Z initially
    pitch = 0.0f;  // No vertical tilt
    distance = 3.0f; // Fixed distance from origin
    up = glm::vec3(0.0f, 1.0f, 0.0f); // Y-up

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}
}

void Camera::updateMouseInput(float xoffset, float yoffset) {
    const float sensitivity = 0.1f;
    yaw += xoffset * sensitivity;
    pitch -= yoffset * sensitivity; // Invert for natural feel

    // Clamp pitch to avoid flipping
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Update position based on spherical coordinates
    position.x = distance * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    position.y = distance * sin(glm::radians(pitch));
    position.z = distance * cos(glm::radians(pitch)) * sin(glm::radians(yaw));
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), up); // Look at origin
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}
glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


Camera::~Camera()
{
}
