#pragma once


#include <iostream>
#include "Controls.h"

glm::mat4 projectionMatrix; //= glm;

glm::mat4 viewMatrix;

controls::controls()
{
}

controls::~controls()
{
}



float controls::calcHorizontalAngle(GLFWwindow* window,float width, float horizontalAngle, float deltaTime, float mouseSpeed)
{
    float ha = 0;
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    float mS = mouseSpeed;

    ha = horizontalAngle += mS * deltaTime * float((width * 0.5) - xpos);
    return ha;
}

float controls::calcVerticalAngle(GLFWwindow* window, float height, float verticalAngle, float deltaTime, float mouseSpeed)
{
    float va = 0;
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    float mS = mouseSpeed;

    va = verticalAngle += mS * deltaTime * float((height * 0.5) - ypos);
    return va;
}



glm::vec3 controls::calcDirection(float horizontalAngle, float verticalAngle)
{
    glm::vec3 direction(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));
    return direction;
}

glm::vec3 controls::calcRight(float horizontalAngle)
{
    // Right vector
    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f)
    );

    return right;

}

glm::vec3 controls::calcUp(glm::vec3 direction, glm::vec3 right)
{
    // Up vector : perpendicular to both direction and right
    glm::vec3 up = glm::cross(right, direction);
    return up;
}

glm::vec3 controls::calcPosition(GLFWwindow* window, glm::vec3 position, glm::vec3 direction, glm::vec3 right,glm::vec3 up,float deltaTime, float speed)
{
    glm::vec3 poz = position;;


    // Move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        poz = position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        poz = position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        poz = position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        poz = position -= right * deltaTime * speed;
    }
    // Float Up
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        poz = position += glm::vec3(0,1,0) * deltaTime * speed;
    }
    // Float Down
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        poz = position -= glm::vec3(0, 1, 0) * deltaTime * speed;

        //float up, is up relative to camera poz = position -= up * deltaTime * speed;
    }

    return poz;
}

glm::mat4 controls::calcProjectionMatrix()
{
    // Initial Field of View
    float initialFoV = 45.0f;
    float FoV = initialFoV;
    glm::mat4 projectionMatrix;

    // Projection matrix : 45&deg; Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    projectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);

    return projectionMatrix;
}

glm::mat4 controls::calcViewMatrix(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
{
    glm::mat4 viewMatrix;
    // Camera matrix
    viewMatrix = glm::lookAt(
        position,           // Camera is here
        position + direction, // and looks here : at the same position, plus "direction"
        up                  // Head is up (set to 0,-1,0 to look upside-down)
        );

    return viewMatrix;
}