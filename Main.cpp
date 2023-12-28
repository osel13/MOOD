#include <iostream>

//glfw
#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>


//glm
//#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext.hpp"
//#include <linmath.h>
//dear imgui
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
//assimp
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//enet
#include <enet/enet.h>
//openAL
#include <AL/al.h>
//ODE physics
#include <ode/ode.h>
//classes
#include "shaderLoader.h"

//Do window - check!
//Do triangle


//shader reading function
//verticles reading function
//color reading function

//raining cubes
//noisy raining cubes

//tohle tahat ze souboru? pak nahradí assimp
static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

int main()
{

	//GLFW INIT
	if (!glfwInit())
	{
		std::cout << "GLFW init failed" << std::endl;
		return -1;
	}
	else
	{
		std::cout << "GLFW init succeeded" << std::endl;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); //antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

	GLFWwindow* window;
	window = glfwCreateWindow(640, 480, "Fuck you Steve!", NULL, NULL);
	if (window == NULL) {
		std::cout << "GLFW Window open Failed" << std::endl;
		glfwTerminate();
		return -1;
	}
	else
	{
		std::cout << "GLFW Window opened" << std::endl;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW initialization failed" << std::endl;
		return -1;
	}
	else
	{
		std::cout << "GLEW initialized" << std::endl;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


	//Vertex Array Object
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//VertexBuffer
	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	//shaders
	shaderLoader sL;
	GLuint programID = sL.loadShaders("SimpleVertexShader.vs", "SimpleFragmentShader.fs");

	// pøedìlat! esc bude do pause menu!
	do {

		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		//use shader
		glUseProgram(programID);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);


	return 0;
}
