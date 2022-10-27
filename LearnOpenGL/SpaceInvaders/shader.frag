#version 330 core

in vec2 TexCoords;

uniform sampler2D Buffer;

out vec4 FragColor;

void main()
{
	FragColor = texture(Buffer, TexCoords);	
} 