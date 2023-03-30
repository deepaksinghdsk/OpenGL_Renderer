#version 330 core

uniform sampler2D tex;

varying vec2 fragTexCoord;
varying vec3 vertex;

out vec4 finalColor;

void main(){
	//vec2 texCoord = vertex.xy * vec2(0.5) + vec2(0.5);
	//gl_FragColor = texture(tex, texCoord);
	

	finalColor = texture(tex, fragTexCoord);
				//vec4(0.0, 1.0, 0.0, 1.0);
}


