#version 330 core

uniform mat4 camera;
uniform mat4 projection;
//uniform mat4 model;

attribute vec3 vert;
attribute vec2 vertTexCoord;

varying vec2 fragTexCoord;
varying vec3 vertex;

void main(){
	// Pass the tex coord straight through to the fragment shader
	fragTexCoord = vertTexCoord;
	vertex = vert;

	//print(model);

	gl_Position = 
		projection * 
		camera * 
		//model * 
		vec4(vert, 1);
}