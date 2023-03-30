#include "Shader.h"

#include <fstream>
#include <sstream>

#include <iostream>
#include <string>
#include <cassert>

using namespace ogl;

Shader::Shader(const std::string& shaderCode, GLenum shaderType) :
	_object(0), _refcount(NULL) 
{
	//create the shader object
	_object = glCreateShader(shaderType);
	if (_object == 0)
		throw std::runtime_error("glCreateShader failed");

	//set the source code
	const char* code = shaderCode.c_str();
	glShaderSource(_object, 1, (const GLchar**) &code, NULL);

	//compile
	glCompileShader(_object);

	//throw exception if compile error occurred
	GLint status;
	glGetShaderiv(_object, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		std::string msg("Compile failure in shader:\n");
		
		GLint infoLogLength;
		glGetShaderiv(_object, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* strInfoLog = new char[infoLogLength + 1];
		glGetShaderInfoLog(_object, infoLogLength, NULL, strInfoLog);
		msg += strInfoLog;
		delete[] strInfoLog;

		glDeleteShader(_object); _object = 0;
		throw std::runtime_error(msg);
	}

	_refcount = new unsigned;
	*_refcount = 1;
}

Shader::Shader(const Shader& other) :
	_object(other._object), _refcount(other._refcount) 
{
	_retain();
}

Shader::~Shader() {
	//_refcount will be null if constructor failed and threw an exception
	if(_refcount) _release();
}

Shader Shader::shaderFromFile(const std::string& filePath, GLenum shaderType) {
	//open file
	std::ifstream f;
	f.open(filePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open())
		throw std::runtime_error(std::string("Failed to open file: ") + filePath);

	//read whole file into stringstream buffer
	std::stringstream buffer;
	buffer << f.rdbuf();

	//return new shader
	Shader shader(buffer.str(), shaderType);
	return shader;
}



GLuint Shader::object() const {
	return _object;
}

Shader& Shader::operator=(const Shader& other) {
	_release();
	_object = other._object;
	_refcount = other._refcount;
	_retain();
	return *this;
}


void Shader::_retain() {
	assert(_refcount);
	*_refcount += 1;
}

void Shader::_release() {
	assert(_refcount && *_refcount > 0);
	*_refcount -= 1;
	if (*_refcount == 0) {
		glDeleteShader(_object); _object = 0;
		delete _refcount; _refcount = NULL;
	}
}