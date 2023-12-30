#include <iostream>
#include <stdio.h>
#pragma warning(disable : 4996) //vyøešit! "fopen" je zastaralý

//glfw opengl
#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>


//glm gpu math
//#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/transform.hpp>
#include "glm/ext.hpp"

//dear imgui
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

//assimp blender import
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//enet networking
#include <enet/enet.h>

//openAL audio
#include <AL/al.h>


//bullet physics
#include "btBulletDynamicsCommon.h"


//Soil image loading
#include <SOIL2/soil2.h>
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

//classes
#include "shaderLoader.h"
#include "controls.h"

//Do window - check!
//Do triangle - check!
//shader reading function - check!

//textures -> texture compression?

//verticles reading function
//color reading function

//raining cubes
//noisy raining cubes


//function declaration
GLuint loadImg(const char* imagepath);
GLuint loadDDS(const char* imagepath);

//tohle tahat ze souboru? pak nahradí assimp
static const GLfloat g_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};


//uv map of cube
static const GLfloat g_uv_buffer_data[] = {
	0.0f,0.0f,
	0.0f,1.0f,
	1.0f, 1.0f,
	0.f, 0.f,
	0.f,1.0f,
	1.0f, 1.0f,
	0.f,0.f,
	0.f,1.0f,
	1.0f, 1.0f,
	0.f, 0.f,
	0.f,1.0f,
	1.0f, 1.0f,
	0.f,0.f,
	0.f,1.0f,
	1.0f, 1.0f,
	0.f, 0.f,
	0.f,1.0f,
	1.0f, 1.0f,
	0.f,0.f,
	0.f,1.0f,
	1.0f, 1.0f,
	0.f, 0.f,
	0.f,1.0f,
	1.0f, 1.0f,
	0.f,0.f,
	0.f,1.0f,
	1.0f, 1.0f,
	0.f, 0.f,
	0.f,1.0f,
	1.0f, 1.0f,
	0.f,0.f,
	0.f,1.0f,
	1.0f, 1.0f,
	0.f, 0.f,
	0.f,1.0f,
	1.0f, 1.0f,
};

//public variables
float height = 720.0;
float width = 1280.0;

	// horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// vertical angle : 0, look at the horizon
float verticalAngle = 0.0f;  //= 0.0f;

glm::vec3 position = glm::vec3(0, 0, 5);
glm::vec3 direction = glm::vec3(0, 0, 0);

glm::vec3 right = glm::vec3(0,0,0);
glm::vec3 up = glm::vec3(0, 0, 0);

float speed = 5.0f; // 3 units / second
float mouseSpeed = 0.1f;

glm::mat4 ProjectionMatrix = glm::mat4(0.0f);
glm::mat4 ViewMatrix = glm::mat4(0.0f);
glm::mat4 ModelMatrix = glm::mat4(0.0f);
glm::mat4 MVP = glm::mat4(0.0f);

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
	window = glfwCreateWindow(width, height, "Fuck you Steve!", NULL, NULL);
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

	//UVBuffer
// This will identify our UV buffer
	GLuint UVbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &UVbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, UVbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);



	GLuint textureID = loadImg("Assets/textures/Uncompressed/diagTex.tga");

	//shaders
	shaderLoader sL;
	GLuint programID = sL.loadShaders("SimpleVertexShader.vs", "SimpleFragmentShader.fs");

	// Get a handle for our "MVP" uniform
	// Only during the initialisation
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);


	// pøedìlat! esc bude do pause menu! -> game loop;
	double currentTime = glfwGetTime();
	double lastTime;
	float deltaTime;

	//if game not paused
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	controls ctrl;

	do {
	

		lastTime = currentTime;
		currentTime = glfwGetTime();
		deltaTime = float(currentTime - lastTime);

			// ...

			// Compute the MVP matrix from keyboard and mouse input
		horizontalAngle = ctrl.calcHorizontalAngle(window, width, horizontalAngle, deltaTime, mouseSpeed);
		verticalAngle = ctrl.calcVerticalAngle(window, height, verticalAngle, deltaTime, mouseSpeed);
		glfwSetCursorPos(window, width / 2, height / 2);
		direction = ctrl.calcDirection(horizontalAngle, verticalAngle);
		right = ctrl.calcRight(horizontalAngle);
		up = ctrl.calcUp(direction, right);
		position = ctrl.calcPosition(window, position, direction, right, up,deltaTime, speed);
		ProjectionMatrix = ctrl.calcProjectionMatrix();
		ViewMatrix = ctrl.calcViewMatrix(position, direction, up);
		ModelMatrix = glm::mat4(1.0);
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

			// ...
		

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

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

		// 2nd attribute buffer : UV
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, UVbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);


		//use shader
		glUseProgram(programID);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12*3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);


	return 0;
}

//Uncompressed textures
GLuint loadImg(const char* imagepath) {

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Read the file, call glTexImage2D with the right parameters
	//glfwLoadTexture2D(imagepath, 0);
	
	/* load an image file directly as a new OpenGL texture */
	GLuint tex_2d = SOIL_load_OGL_texture
	(
		imagepath,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	/* check for an error during the load process */
	if (0 == tex_2d)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}
	
	// Nice trilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;
}
//
GLuint loadDDS(const char* imagepath) {

	unsigned char header[124];

	FILE* fp;

	/* try to open the file */
	fp = fopen(imagepath, "rb");
	if (fp == NULL)
		return 0;

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);

	unsigned char* buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;
	}
	free(buffer);

	return textureID;

	
}