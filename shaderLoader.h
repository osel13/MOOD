#pragma once
#include <GL/glew.h>

class shaderLoader
{
public:
	shaderLoader();
	~shaderLoader();
	GLuint loadShaders(const char* vertex_file_path, const char* fragment_file_path);
};

