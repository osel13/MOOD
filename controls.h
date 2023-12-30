#pragma once
#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

class controls
{
private:
	








public:
	controls();
	~controls();

	glm::mat4  calcProjectionMatrix();
	float calcHorizontalAngle(GLFWwindow* window, float width, float horizontalAngle, float deltaTime, float mouseSpeed);
	float calcVerticalAngle(GLFWwindow* window, float height, float verticalAngle, float deltaTime, float mouseSpeed);
	glm::vec3 calcDirection(float horizontalAngle, float verticalAngle);
	glm::vec3 calcRight(float horizontalAngle);
	glm::vec3 calcUp(glm::vec3 direction, glm::vec3 right);
	glm::vec3 calcPosition(GLFWwindow* window, glm::vec3 position, glm::vec3 direction, glm::vec3 right, glm::vec3 up, float deltaTime, float speed);
	glm::mat4 calcViewMatrix(glm::vec3 position, glm::vec3 direction, glm::vec3 up);

};

