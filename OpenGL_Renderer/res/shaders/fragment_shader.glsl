#version 330 core

uniform mat4 model;
uniform vec3 cameraPosition;
uniform sampler2D tex;
uniform float materialShininess = 0.01f;
uniform vec3 materialSpecularColor;
						 
uniform struct Light{
	vec3 position;
	vec3 intensities; //a.k.a the color of the light
	float attenuation;
	float ambientCoefficient;
} light;

varying vec2 fragTexCoord;
varying vec3 fragVert;
varying vec3 fragNormal;

out vec4 finalColor;

void main(){
	//vec2 texCoord = vertex.xy * vec2(0.5) + vec2(0.5);
	//gl_FragColor = texture(tex, texCoord);
	
	vec3 surfacePosition = vec3(model * vec4(fragVert, 1));

	//calculate normal in world coordinates
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 normal = normalize(normalMatrix * fragNormal);

	//calculate the location of this fragment (pixel) in world coordinates
	vec3 fragPosition = vec3(model * vec4(fragVert, 1));

	//calculate the vector from this pixel to the light source
	vec3 surfaceToLight = light.position - fragPosition;

	//brightness/cos(angle)
	float brightness = dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
	brightness = clamp(brightness, 0, 1);

	//specular
	vec3 incidenceVector = -surfaceToLight; //a unit vector
	vec3 reflectionVector = reflect(incidenceVector, normal); //also a unit vector
	vec3 surfaceToCamera = normalize(cameraPosition - surfacePosition); //also a unit vector
	float cosAngle = max(0.0, dot(surfaceToCamera, reflectionVector));
	float specularCoefficient = pow(cosAngle, materialShininess);

	//calculate final color of the pixel, based on:
	// 1. The angle of incidence: brightness
	// 2. The color/intensities of the light: light.intensities
	// 3. The texture and texture coord: texture(tex, fragTexCoord)
	
	vec4 surfaceColor = texture(tex, fragTexCoord);

	vec3 diffuse = brightness * light.intensities * surfaceColor.rgb;
	
	vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.intensities;

	vec3 specular = specularCoefficient * materialSpecularColor * light.intensities;

	float distanceToLight = length(light.position - surfacePosition);
	float attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

	//linear color(color before gamma correction)
	vec3 linearColor = ambient + attenuation * (diffuse + specular);
	
	vec3 gamma = vec3(1.0/2.2);
	linearColor = pow(linearColor, gamma);
	finalColor = vec4(linearColor, surfaceColor.a);
				 //surfaceColor;
				 //vec4(0.0, 1.0, 0.0, 1.0);
}


