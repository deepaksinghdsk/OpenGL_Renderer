#version 330 core

//uniform mat4 projection;
uniform mat4 camera;
uniform mat4 model;

attribute vec3 vert;
attribute vec2 vertTexCoord;
attribute vec3 vertNormal;

varying vec3 fragVert;
varying vec2 fragTexCoord;
varying vec3 fragNormal;

mat4 rotation_x(float theta);
mat4 rotation_y(float theta);
mat4 rotation_z(float theta);
mat4 scale(float x, float y, float z);
mat4 translate(float x, float y, float z);

void main(){
	// Pass the tex coord straight through to the fragment shader
	fragTexCoord = vertTexCoord;
	fragVert = vert;
	fragNormal = vertNormal;

	gl_Position = 
		//projection * 
		camera *
		//translate(-1.5, 0.0, -1.5) *
		//scale(4.0/3.0, 1.0, 1.0) *
		model * 
		//rotation_x(45.0f) *
		vec4(vert, 1);
}

mat4 rotation_x(float theta){
	return mat4(
		vec4(1.0,		  0.0,		  0.0, 0.0),
		vec4(0.0,  cos(theta), sin(theta), 0.0),
		vec4(0.0, -sin(theta), cos(theta), 0.0),
		vec4(0.0,		  0.0,		  0.0, 1.0)
	);
}

mat4 rotation_y(float theta){
	return mat4(
		vec4(cos(theta), 0.0, -sin(theta), 0.0),
		vec4(		0.0, 1.0,		  0.0, 0.0),
		vec4(sin(theta), 0.0,  cos(theta), 0.0),
		vec4(		0.0, 0.0,		  0.0, 0.0)
	);
}

mat4 rotation_z(float theta){
	return mat4(
		vec4( cos(theta), sin(theta), 0.0, 0.0),
		vec4(-sin(theta), cos(theta), 0.0, 0.0),
		vec4(		 0.0,		 1.0, 0.0, 0.0),
		vec4(		 0.0,		 0.0, 0.0, 0.0)
	);
}

mat4 scale(float x, float y, float z){
	return mat4(
		vec4(  x, 0.0, 0.0, 0.0),
		vec4(0.0,   y, 0.0, 0.0),
		vec4(0.0, 0.0,   z, 0.0),
		vec4(0.0, 0.0, 0.0, 1.0)
	);
}

mat4 translate(float x, float y, float z){
	return mat4(
		vec4(1.0, 0.0, 0.0, 0.0),
		vec4(0.0, 1.0, 0.0, 0.0),
		vec4(0.0, 0.0, 1.0, 0.0),
		vec4(  x,   y,   z, 1.0)
	);
}