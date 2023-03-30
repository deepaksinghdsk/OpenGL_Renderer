#pragma once
#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>   

/*
 * Boring, non-OpenGL-related utility functions
 */


namespace util{

	static short le_short(unsigned char* bytes);

	
	static std::string readFromFile(const char* pathToFile);


	void* read_tga(const char* filename, int* width, int* height);
}