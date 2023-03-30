#pragma once

#include "Shader.h"
#include "glm\glm.hpp"

#include <vector>

namespace ogl {

	class Program {
	public:
		/*
			creates a proogram by linking a list of ogl::Shader objects
			
			@param shaders: The shaders to link together to make the program

			@thows std::exception if an error occurs.

			@see ogl::Shader
		*/
		Program(const std::vector<Shader>& shaders);
		~Program();

		/*
			@result The program's object id, as returned from glCreateProgram
		*/
		GLuint object() const;

		/*
			@result The attribute index for the given name, as returned by glGetAttribLocation.
		*/
		GLint attrib(const GLchar* attribName) const;

		/*
			@result The uniform index for the given uniformName, as returned by glGetUniformLocation.
		*/
		GLint uniform(const GLchar* uniformName) const;

		void setUniformTexture(const GLchar* uniformName, const int value);

		void setUniformMat4f(const GLchar* uniformName, GLsizei size, GLboolean transpose, const glm::mat4 value);

		//void setUniform(const GLchar* uniformName, const glm::vec4& v);

	private:
		GLuint _object;
		
		Program(const Program& a);
		const Program& operator=(const Program&);
	};

}
