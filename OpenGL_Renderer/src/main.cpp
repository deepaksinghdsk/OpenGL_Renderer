#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <list>

#include "ogl/Program.h"
#include "ogl/Texture.h"
#include "ogl/Camera.h"
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\string_cast.hpp>

//constants
const glm::vec2 SCREEN_SIZE(800, 600);

struct ModelAsset {
	ogl::Program* shaders;
	ogl::Texture* texture;
	GLuint vbo = 0;
	GLuint vao = 0;
	GLenum drawType;
	GLint drawstart;
	GLint drawCount;
	GLfloat shininess;
	glm::vec3 specularColor;
};

struct ModelInstance {
	ModelAsset* asset;
	glm::mat4 transform;
};

struct Light {
	glm::vec3 position;
	glm::vec3 intensities; //a.k.a. the color of the light
	float attenuation;
	float ambientCoefficient;
};

//globals
//ogl::Program* gProgram = NULL;
//ogl::Texture* gTexture = NULL;
//GLuint gVAO = 0;
//GLuint gVBO = 0;
GLFWwindow* gWindow = NULL;
ModelAsset gWoodenCrate;
std::list<ModelInstance> gInstances;
ogl::Camera gCamera;
Light gLight;
ogl::Program* currentShaders = NULL;
ogl::Texture* currentTextures = NULL;
double gScrollY = 0.0;
GLfloat gDegreesRotated = 0.0f;

// loads the vertex shader and fragment shader, and links them to make a global gProgram
ogl::Program* LoadShaders(const char* vertexShader, const char* fragmentShader) {
	std::vector<ogl::Shader> shaders;
	shaders.push_back(ogl::Shader::shaderFromFile(vertexShader, GL_VERTEX_SHADER));
	shaders.push_back(ogl::Shader::shaderFromFile(fragmentShader, GL_FRAGMENT_SHADER));

	ogl::Program* program = new ogl::Program(shaders);
	
	glUseProgram(program->object());
	
	//set the projection uniform in vertex shader, becuase it's not going to change
	//glm::mat4 projection = glm::perspective(glm::radians(50.0f), SCREEN_SIZE.x/SCREEN_SIZE.y, 0.1f, 10.0f);
	//gProgram->setUniformMat4f("projection", projection, 1, false);
	
	//set the camera uniform in vertex shader, becuase it's not going to change
	//glm::mat4 camera = glm::lookAt(glm::vec3(3, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	//gProgram->setUniformMat4f("camera", camera, 1, false);
	
	//std::cout << glm::to_string(camera) << std::endl;

	glUseProgram(0);

	return program;
}

// loads a triangle into the VAO global
void LoadModel(GLuint *vao, GLuint *vbo, ogl::Program* program) {
	// make and bind the VAO
	glGenVertexArrays(1, vao);
	glBindVertexArray(*vao);

	// make and bind the VBO
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);

	GLfloat triangleVertexData[] = {
		//	X	  Y		Z		U	  V
		 0.0f, 0.8f, 0.0f,	 0.5f, 1.0f,
		-0.8f,-0.8f, 0.0f,	 0.0f, 0.0f,
		 0.8f,-0.8f, 0.0f,	 1.0f, 0.0f,
	};

	GLfloat cubeVertexData[] = {
		//	X	  Y		Z		U	  V			Normal
		// bottom
		-1.0f,-1.0f,-1.0f,	 0.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		 1.0f,-1.0f,-1.0f,   1.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-1.0f,-1.0f, 1.0f,   0.0f, 1.0f,	0.0f, -1.0f, 0.0f,
		 1.0f,-1.0f,-1.0f,   1.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		 1.0f,-1.0f, 1.0f,   1.0f, 1.0f,	0.0f, -1.0f, 0.0f,
		-1.0f,-1.0f, 1.0f,   0.0f, 1.0f,	0.0f, -1.0f, 0.0f,

		// top
		-1.0f, 1.0f,-1.0f,   0.0f, 0.0f,	0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,   0.0f, 1.0f,	0.0f, 1.0f, 0.0f,
		 1.0f, 1.0f,-1.0f,   1.0f, 0.0f,	0.0f, 1.0f, 0.0f,
		 1.0f, 1.0f,-1.0f,   1.0f, 0.0f,	0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,   0.0f, 1.0f,	0.0f, 1.0f, 0.0f,
		 1.0f, 1.0f, 1.0f,   1.0f, 1.0f,	0.0f, 1.0f, 0.0f,

		// front
		-1.0f,-1.0f, 1.0f,   1.0f, 0.0f,	0.0f, 0.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,   0.0f, 0.0f,	0.0f, 0.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,   1.0f, 1.0f,	0.0f, 0.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,   0.0f, 0.0f,	0.0f, 0.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,   0.0f, 1.0f,	0.0f, 0.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,   1.0f, 1.0f,	0.0f, 0.0f, 1.0f,

		// back
		-1.0f,-1.0f,-1.0f,   0.0f, 0.0f,	0.0f, 0.0f, -1.0f,
		-1.0f, 1.0f,-1.0f,   0.0f, 1.0f,	0.0f, 0.0f, -1.0f,
		 1.0f,-1.0f,-1.0f,   1.0f, 0.0f,	0.0f, 0.0f, -1.0f,
		 1.0f,-1.0f,-1.0f,   1.0f, 0.0f,	0.0f, 0.0f, -1.0f,
		-1.0f, 1.0f,-1.0f,   0.0f, 1.0f,	0.0f, 0.0f, -1.0f,
		 1.0f, 1.0f,-1.0f,   1.0f, 1.0f,	0.0f, 0.0f, -1.0f,

		// left
		-1.0f,-1.0f, 1.0f,   0.0f, 1.0f,	-1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f,-1.0f,   1.0f, 0.0f,	-1.0f, 0.0f, 0.0f,
		-1.0f,-1.0f,-1.0f,   0.0f, 0.0f,	-1.0f, 0.0f, 0.0f,
		-1.0f,-1.0f, 1.0f,   0.0f, 1.0f,	-1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,   1.0f, 1.0f,	-1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f,-1.0f,   1.0f, 0.0f,	-1.0f, 0.0f, 0.0f,

		// right
		 1.0f,-1.0f, 1.0f,   1.0f, 1.0f,	1.0f, 0.0f, 0.0f,
		 1.0f,-1.0f,-1.0f,   1.0f, 0.0f,	1.0f, 0.0f, 0.0f,
		 1.0f, 1.0f,-1.0f,   0.0f, 0.0f,	1.0f, 0.0f, 0.0f,
		 1.0f,-1.0f, 1.0f,   1.0f, 1.0f,	1.0f, 0.0f, 0.0f,
		 1.0f, 1.0f,-1.0f,   0.0f, 0.0f,	1.0f, 0.0f, 0.0f,
		 1.0f, 1.0f, 1.0f,   0.0f, 1.0f,	1.0f, 0.0f, 0.0f
	};

	// Put the three triangle vertices into the VBO
	//GLfloat *vertexData = new GLfloat();
	//std::copy(std::begin(cubeVertexData), std::end(cubeVertexData), vertexData+1);
	//vertexData = triangleVertexData;

	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexData), cubeVertexData, GL_STATIC_DRAW);
	//glm::mat4 proj = glm::ortho();
	
	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(program->attrib("vert"));
	glVertexAttribPointer(program->attrib("vert"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), NULL);

	// connect the UV coords to the "vertTexCoord" attribute of the vertex shader
	glEnableVertexAttribArray(program->attrib("vertTexCoord"));
	glVertexAttribPointer(program->attrib("vertTexCoord"), 2, GL_FLOAT, GL_TRUE, 8 * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(program->attrib("vertNormal"));
	glVertexAttribPointer(program->attrib("vertNormal"), 3, GL_FLOAT, GL_TRUE, 8 * sizeof(GLfloat), (const GLvoid*)(5 * sizeof(GLfloat)));

	//unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//load the file "hazard.png" into gTexture
ogl::Texture* LoadTexture(const char* image) {
	ogl::Bitmap bmp = ogl::Bitmap::bitmapFromFile(image);
	bmp.flipVertically();
	return new ogl::Texture(bmp);
}

void loadWoodenCrateAsset() {
	gWoodenCrate.shaders = LoadShaders("../OpenGL_Renderer/res/shaders/vertex_shader.glsl", "../OpenGL_Renderer/res/shaders/fragment_shader.glsl");
	gWoodenCrate.drawType = GL_TRIANGLES;
	gWoodenCrate.drawstart = 0;
	gWoodenCrate.drawCount = 6 * 2 * 3;
	gWoodenCrate.texture = LoadTexture("../OpenGL_Renderer/res/textures/wooden-crate.jpg");
	gWoodenCrate.shininess = 0.4;
	gWoodenCrate.specularColor = glm::vec3(0.5f, 0.5f, 0.5f);
	LoadModel(&gWoodenCrate.vao, &gWoodenCrate.vbo, gWoodenCrate.shaders);
}

glm::mat4 translate(float x, float y, float z) {
	return glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

glm::mat4 scale(float x, float y, float z) {
	return glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void createInstances() {
	ModelInstance dot;
	dot.asset = &gWoodenCrate;
	dot.transform = glm::mat4(1.0f);
	gInstances.push_back(dot);

	ModelInstance i;
	i.asset = &gWoodenCrate;
	i.transform = translate(0, -4, 0) * scale(1, 2, 1);
	gInstances.push_back(i);

	ModelInstance hLeft;
	hLeft.asset = &gWoodenCrate;
	hLeft.transform = translate(-8, 0, 0) * scale(1, 6, 1);
	gInstances.push_back(hLeft);

	ModelInstance hMid;
	hMid.asset = &gWoodenCrate;
	hMid.transform = translate(-6, 0, 0) * scale(2, 1, 0.8);
	gInstances.push_back(hMid);

	ModelInstance hRight;
	hRight.asset = &gWoodenCrate;
	hRight.transform = translate(-4, 0, 0) * scale(1, 6, 1);
	gInstances.push_back(hRight);

}

void renderInstances(const ModelInstance* inst) {
	ModelAsset* asset = inst->asset;
	//ogl::Program* shaders = asset->shaders;

	// bind the shaders
	//currentShaders->use();

	// set the shader uniforms
	currentShaders->setUniformMat4f("camera", gCamera.matrix());
	currentShaders->setUniformMat4f("model", inst->transform);
	currentShaders->setUniformTexture("tex", 0);//set to 0 because the texture will be bound to GL_TEXTURE0
	currentShaders->setUniformf("materialShininess", asset->shininess);
	currentShaders->setUniformVec3f("materialSpecularColor", asset->specularColor);

	currentShaders->setUniformVec3f("light.position", gLight.position);
	currentShaders->setUniformVec3f("light.intensities", gLight.intensities);
	currentShaders->setUniformf("light.attenuation", gLight.attenuation);
	currentShaders->setUniformf("light.ambientCoefficient", gLight.ambientCoefficient); 

	currentShaders->setUniformVec3f("cameraPosition", gCamera.position());

	//bind the texure
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, asset->texture->object());

	//bind VAO and draw
	glBindVertexArray(asset->vao);
	glDrawArrays(asset->drawType, asset->drawstart, asset->drawCount);

	//unbind everything
	glBindVertexArray(0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//currentShaders->stopUsing();
}

//draws multiple objects
void Render() {
	//clear Everything
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//render all the instances
	for (auto &it : gInstances) {
		if (it.asset->shaders != currentShaders) {
			if (currentShaders)
				currentShaders->stopUsing();

			currentShaders = it.asset->shaders;
			currentShaders->use();
			std::cout << "shader loaded" << std::endl;
		}

		if (it.asset->texture != currentTextures) {
			if (currentTextures)
				glBindTexture(GL_TEXTURE_2D, 0);

			currentTextures = it.asset->texture;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, currentTextures->object());
		}

		renderInstances(&it);
	}

	// swap the display buffers (displays what was just drawn)
	glfwSwapBuffers(gWindow);
}

// draws a single frame
/*void Render() {
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
	gProgram->setUniformMat4f("camera", gCamera.matrix());

	//set the "model" uniform in the vertex shader, based on the gDegreesRotated global
	gProgram->setUniformMat4f("model", glm::rotate(glm::mat4(1.0f), glm::radians(gDegreesRotated), glm::vec3(0, 1, 0)));

	//std::cout << glm::to_string(rotation) << std::endl;

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
}*/

void onError(int errorCode, const char* msg) {
	throw std::runtime_error(msg);
}

// update the scene based on the time elapsed since last update
void Update(float secondsElapsed) {
	//rotate the cube
	const GLfloat degreesPerSecond = 90.0f;
	gDegreesRotated += degreesPerSecond * secondsElapsed * 0.02;
	while (gDegreesRotated > 360.0f) gDegreesRotated -= 360.0f;
	gInstances.front().transform = glm::rotate(glm::mat4(1.0f), gDegreesRotated, glm::vec3(0, 1, 0));

	//move ligh
	if (glfwGetKey(gWindow, '1'))
		gLight.position = gCamera.position();

	//change light color
	if (glfwGetKey(gWindow, '2'))
		gLight.intensities = glm::vec3(1, 0, 0); //red
	else if (glfwGetKey(gWindow, '3'))
		gLight.intensities = glm::vec3(0, 1, 0); //green
	else if (glfwGetKey(gWindow, '4'))
		gLight.intensities = glm::vec3(1, 1, 1); //white

	//move position of camera based on WASD keys
	const float moveSpeed = 6.0; //units per second
	if (glfwGetKey(gWindow, 'S'))
		gCamera.offsetPosition(secondsElapsed * moveSpeed * -gCamera.forward());
	else if (glfwGetKey(gWindow, 'W'))
		gCamera.offsetPosition(secondsElapsed * moveSpeed * gCamera.forward());

	if (glfwGetKey(gWindow, 'A'))
		gCamera.offsetPosition(secondsElapsed * moveSpeed * -gCamera.right());
	else if (glfwGetKey(gWindow, 'D'))
		gCamera.offsetPosition(secondsElapsed * moveSpeed * gCamera.right());

	if (glfwGetKey(gWindow, 'Z'))
		gCamera.offsetPosition(secondsElapsed * moveSpeed * -glm::vec3(0, 1, 0));
	else if (glfwGetKey(gWindow, 'X'))
		gCamera.offsetPosition(secondsElapsed * moveSpeed * glm::vec3(0, 1, 0));

	if(glfwGetKey(gWindow, 'F'))
		gCamera.lookAt(glm::vec3(0, 0, 0));

	//rotate camera based on mouse movement
	const float mouseSensitivity = 0.1f;
	double mouseX, mouseY;
	glfwGetCursorPos(gWindow, &mouseX, &mouseY);
	gCamera.offsetOrientation(mouseSensitivity * (float)mouseY, mouseSensitivity * (float)mouseX);

	const float zoomSensitivity = 1.0f;
	float fieldOfView = gCamera.fieldOfView() + zoomSensitivity * (float)gScrollY;
	if (fieldOfView < 5.0f) fieldOfView = 5.0f;
	if (fieldOfView > 130.0f) fieldOfView = 130.0f;
	gCamera.setFieldOfView(fieldOfView);
	gScrollY = 0;

	//reset the mouse, so it doesn't go out of the window
	glfwSetCursorPos(gWindow, 0, 0);
}

void onScroll(GLFWwindow* window, double deltaX, double deltaY) {
	gScrollY += deltaY;
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
	glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(gWindow, 0, 0);
	glfwSetScrollCallback(gWindow, onScroll);
	glfwMakeContextCurrent(gWindow);

	//initialise GLEW
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
	//LoadShaders();

	// load the texture
	//LoadTexture();

	// create buffer and fill it with the points of the triangle
	//LoadModel();

	loadWoodenCrateAsset();

	createInstances();


	// setup gCamera
	gCamera.setPosition(glm::vec3(-4, 0, 17));
	//gCamera.lookAt(glm::vec3(0, 0, 0));
	gCamera.setViewportAspectRatio(SCREEN_SIZE.x / SCREEN_SIZE.y);
	gCamera.setNearAndFarPlane(0.5f, 100.0f);

	//setup light
	gLight.position = glm::vec3(-4, 0, 1.4);
	gLight.intensities = glm::vec3(1, 1, 1); //white
	gLight.attenuation = 0.2f;
	gLight.ambientCoefficient = 0.01f;

	//run while the window is open
	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(gWindow)) {
		//process pending events
		glfwPollEvents();

		// update the scene based on the time elapsed since last update
		double thisTime = glfwGetTime();
		Update((float)(thisTime - lastTime));
		lastTime = thisTime;

		//drow one frame
		Render();

		//exit program if escape key is pressed
		if (glfwGetKey(gWindow, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(gWindow, GL_TRUE);
	}

	//clean up and exit
	glBindTexture(GL_TEXTURE_2D, 0);
	currentShaders->stopUsing();
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