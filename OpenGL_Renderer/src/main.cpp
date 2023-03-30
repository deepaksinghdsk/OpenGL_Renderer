#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "ogl/Program.h"
#include "ogl/Texture.h"
#include "ogl/Camera.h"

//constants
const glm::vec2 SCREEN_SIZE(800, 600);

//globals
GLFWwindow* gWindow = NULL;
double gScrollY = 0.0;
ogl::Program* gProgram = NULL;
ogl::Texture* gTexture = NULL;
ogl::Camera gCamera;
GLuint gVAO = 0;
GLuint gVBO = 0;
GLfloat gDegreesRotated = 0.0f;

// loads the vertex shader and fragment shader, and links them to make a global gProgram
void LoadShaders() {
	std::vector<ogl::Shader> shaders;
	shaders.push_back(ogl::Shader::shaderFromFile("../OpenGL_Renderer/res/shaders/vertex_shader.glsl", GL_VERTEX_SHADER));
	shaders.push_back(ogl::Shader::shaderFromFile("../OpenGL_Renderer/res/shaders/fragment_shader.glsl", GL_FRAGMENT_SHADER));

	gProgram = new ogl::Program(shaders);
	/*
*/
	glUseProgram(gProgram->object());
	
	//set the camera uniform in vertex shader, becuase it's not going to change
	glm::mat4 camera = glm::lookAt(glm::vec3(3, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	gProgram->setUniformMat4f("camera", 1, false, camera);

	//gProgram->setUniformMat4f("model", 1, false, glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(0, 1, 0)));

	//set the projection uniform in vertex shader, becuase it's not going to change
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 10.0f);
	gProgram->setUniformMat4f("projection", 1, false, projection);

	glUseProgram(0);
	
}

// loads a triangle into the VAO global
void LoadModel() {
	// make and bind the VAO
	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);

	// make and bind the VBO
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);

	GLfloat triangleVertexData[] = {
		//	X	  Y		Z		U	  V
		 0.0f, 0.8f, 0.0f,	 0.5f, 1.0f,
		-0.8f,-0.8f, 0.0f,	 0.0f, 0.0f,
		 0.8f,-0.8f, 0.0f,	 1.0f, 0.0f,
	};

	GLfloat cubeVertexData[] = {
		//	X	  Y		Z		U	  V
		// bottom
		-1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
		 1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
		-1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
		 1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,   0.0f, 1.0f,

		// top
		-1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
		 1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,   1.0f, 1.0f,

		// front
		-1.0f,-1.0f, 1.0f,   1.0f, 0.0f,
		 1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
		 1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,   1.0f, 1.0f,

		// back
		-1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
		-1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
		 1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
		-1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,   1.0f, 1.0f,

		// left
		-1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
		-1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
		-1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,   1.0f, 0.0f,

		// right
		 1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
		 1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
		 1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
		 1.0f, 1.0f, 1.0f,   0.0f, 1.0f	
	};

	// Put the three triangle vertices into the VBO
	GLfloat vertexData[180];// = new GLfloat();
	std::copy(std::begin(cubeVertexData), std::end(cubeVertexData), std::begin(vertexData));
	//vertexData = triangleVertexData;

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	//glm::mat4 proj = glm::ortho();
	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(gProgram->attrib("vert"));
	glVertexAttribPointer(gProgram->attrib("vert"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);

	// connect the UV coords to the "vertTexCoord" attribute of the vertex shader
	glEnableVertexAttribArray(gProgram->attrib("vertTexCoord"));
	glVertexAttribPointer(gProgram->attrib("vertTexCoord"), 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

	//unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//load the file "hazard.png" into gTexture
void LoadTexture() {
	ogl::Bitmap bmp = ogl::Bitmap::bitmapFromFile("../OpenGL_Renderer/res/textures/wooden-crate.jpg");
	bmp.flipVertically();
	gTexture = new ogl::Texture(bmp);
}

// draws a single frame
void Render() {
	//clear everything
	glClearColor(0, 0, 0, 1); // black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//bind the program (the shaders)
	glUseProgram(gProgram->object());

	//bind the texture and set the "tex" uniform in the fragment shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexture->object());
	gProgram->setUniformTexture("tex", 0); // set to 0 because the texture is bound to GL_TEXTURE0

	//set the "camera" uniform
	//gProgram->setUniformMat4f("camera", 1, false, gCamera.matrix());

	//set the "model" uniform in the vertex shader, based on the gDegreesRotated global
	//gProgram->setUniformMat4f("model", 1, false, glm::rotate(glm::mat4(), glm::radians(45.0f), glm::vec3(0, 1, 0)));

	//bind the VAO (the triangle)
	glBindVertexArray(gVAO);

	//draw the VAO
	glDrawArrays(GL_TRIANGLES, 0, 6*2*3);

	//unbind the VAO, the texture and the program
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	//swap the dispay buffers (displays what was just drawn)
	glfwSwapBuffers(gWindow);
}

void onError(int errorCode, const char* msg) {
	throw std::runtime_error(msg);
}

// update the scene based on the time elapsed since last update
void Update(float secondsElapsed) {
	//rotate the cube
	const GLfloat degreesPerSecond = 90.0f;
	gDegreesRotated += secondsElapsed * degreesPerSecond;
	while (gDegreesRotated > 360.0f) gDegreesRotated -= 360.0f;

	//move position of camera based on WASD keys
	const float moveSpeed = 2.0; //units per second
	if (glfwGetKey(gWindow, 'S'))
		gCamera.offsetPosition(secondsElapsed * moveSpeed * -gCamera.forward());
	else if (glfwGetKey(gWindow, 'W'))
		gCamera.offsetPosition(secondsElapsed * moveSpeed * gCamera.forward());

	if (glfwGetKey(gWindow, 'A'))
		gCamera.offsetPosition(secondsElapsed * moveSpeed * -gCamera.right());
	else if (glfwGetKey(gWindow, 'S'))
		gCamera.offsetPosition(secondsElapsed * moveSpeed * gCamera.right());

	if (glfwGetKey(gWindow, 'Z'))
		gCamera.offsetPosition(secondsElapsed * moveSpeed * -glm::vec3(0, 1, 0));
	else if (glfwGetKey(gWindow, 'X'))
		gCamera.offsetPosition(secondsElapsed * moveSpeed * glm::vec3(0, 1, 0));

	//rotate camera based on mouse movement
	const float mouseSensitivity = 0.1f;
	double mouseX, mouseY;
	glfwGetCursorPos(gWindow, &mouseX, &mouseY);
	gCamera.offsetOrientation(mouseSensitivity * (float)mouseY, mouseSensitivity * (float)mouseX);

	//reset the mouse, so it doesn't go out of the window
	//glfwSetCursorPos(gWindow, 0, 0);
}

// the program statrs here
void AppMain() {
	//initialise GLFW
	glfwSetErrorCallback(onError);
	if (!glfwInit())
		throw std::runtime_error("glfwInit failed");

	//open a window with GLFW
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	gWindow = glfwCreateWindow((int) SCREEN_SIZE.x, (int) SCREEN_SIZE.y, "OpenGL", NULL, NULL);
	if (!gWindow)
		throw std::runtime_error("glfwCreateWindow failed. Check support for OpenGL 3.2");

	//GLFW settings, Make this windows context current
	//glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(gWindow, 0, 0);
	glfwMakeContextCurrent(gWindow);

	//initialise GLEW
	//glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
	if (glewInit() != GLEW_OK)	
		throw std::runtime_error("glewInit failed");

	//Print info about the graphics drivers
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << std::endl;
	//make sure OpenGL version 3.2 API is avialable
	if (!GLEW_VERSION_3_2)
		throw std::runtime_error("OpenGL 3.2 API is not available");

	//OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// load vertex and fragment shaders into opengl
	LoadShaders();

	// load the texture
	LoadTexture();

	// create buffer and fill it with the points of the triangle
	LoadModel();

	// setup gCamera
	gCamera.setPosition(glm::vec3(0, 0, 4));
	gCamera.setViewportAspectRatio(SCREEN_SIZE.x / SCREEN_SIZE.y);

	//run while the window is open
	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(gWindow)) {
		//process pending events
		glfwPollEvents();

		// update the scene based on the time elapsed since last update
		/*double thisTime = glfwGetTime();
		Update((float)(thisTime - lastTime));
		lastTime = thisTime;*/

		//drow one frame
		Render();

		//exit program if escape key is pressed
		if (glfwGetKey(gWindow, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(gWindow, GL_TRUE);
	}

	//clean up and exit
	glfwTerminate();
}

int main(int argc, char* argv[]) {
	try {
		AppMain();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	//std::cin.get();
	return EXIT_SUCCESS;
}