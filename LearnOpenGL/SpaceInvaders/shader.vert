#version 330 core

out vec2 TexCoords;

void main()
{
	TexCoords.x = gl_VertexID == 1? 2.0: 0.0;
	TexCoords.y = gl_VertexID == 2? 2.0: 0.0;

	gl_Position = vec4(2 * TexCoords - 1, 0.0, 1.0);
}