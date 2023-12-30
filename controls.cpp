#pragma once



#include "Controls.h"

controls::controls()
{
}

controls::~controls()
{
}

void controls::computeMatricesFromInputs(GLFWwindow* window, float deltaTime)
{
    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);


    // Reset mouse position for next frame
   // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //pause menu pøedìlat


    float ha = getHorizontalAngle();
    float va = getVerticalAngle();

    // Compute new orientation
   setHorizontalAngle(ha += mouseSpeed * deltaTime * float(1024 / 2 - xpos));
    setVerticalAngle(va += mouseSpeed * deltaTime * float(768 / 2 - ypos));

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );

    // Right vector
    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f)
    );

    // Up vector : perpendicular to both direction and right
    glm::vec3 up = glm::cross(right, direction);

    //srolling to change FOV?

    // Move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position -= right * deltaTime * speed;

        float FoV = initialFoV;
        // Projection matrix : 45&deg; Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        projectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
        // Camera matrix
        viewMatrix = glm::lookAt(
            position,           // Camera is here
            position + direction, // and looks here : at the same position, plus "direction"
            up                  // Head is up (set to 0,-1,0 to look upside-down)

        );

        // Cull triangles which normal is not towards the camera
        //glEnable(GL_CULL_FACE);
    }
}

glm::mat4 controls::getProjectionMatrix()
{
    return projectionMatrix;
}

void controls::setProjectionMatrix(glm::mat4 mat)
{
    projectionMatrix = mat;
}

glm::mat4 controls::getViewMatrix()
{
    return viewMatrix;
}

void controls::setViewMatrix(glm::mat4 mat)
{
    viewMatrix = mat;
}

void controls::setHorizontalAngle(float ha)
{
    horizontalAngle = ha;
}



float controls::getHorizontalAngle()
{
    return horizontalAngle;
}

void controls::setVerticalAngle(float va)
{
    verticalAngle = va;
}




float controls::getVerticalAngle()
{
    return verticalAngle;
}


