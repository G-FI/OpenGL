#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;


void main()
{
	//gl_Position =projection * view* vec4(aPos, 1.0);
	
	//优化，使用深度缓冲，使天空盒的z深度始终为1，GL_LEQUAL为谓词，只有深度缓冲值大于等于一是才会用背景复写，场景中其余物体深度肯定小于1
	vec4 pos = projection * view* vec4(aPos, 1.0);
	gl_Position = pos.xyww;

	//点的位置作为采样的纹理坐标，cubetexture的中心就在原点处
	TexCoords = aPos;
}