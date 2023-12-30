#pragma once
#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

class controls
{
private:
	glm::vec3 position = glm::vec3(0, 0, 5);

	// horizontal angle : toward -Z
	float horizontalAngle;
	// vertical angle : 0, look at the horizon
	float verticalAngle;  //= 0.0f;
	// Initial Field of View
	float initialFoV = 45.0f;

	float speed = 3.0f; // 3 units / second

	float mouseSpeed = 0.005f;

	glm::mat4 projectionMatrix;

	glm::mat4 viewMatrix;


public:
	controls();
	~controls();

	void computeMatricesFromInputs(GLFWwindow* window, float deltaTime);

	void setProjectionMatrix(glm::mat4);
	glm::mat4  getProjectionMatrix();
	void setViewMatrix(glm::mat4);
	glm::mat4  getViewMatrix();

	void setHorizontalAngle(float ha = 3.14f);
	float getHorizontalAngle();
	
	void setVerticalAngle(float va = 0.0f);
	
	float getVerticalAngle();


};

