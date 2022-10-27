#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;


void main()
{
	//gl_Position =projection * view* vec4(aPos, 1.0);
	
	//�Ż���ʹ����Ȼ��壬ʹ��պе�z���ʼ��Ϊ1��GL_LEQUALΪν�ʣ�ֻ����Ȼ���ֵ���ڵ���һ�ǲŻ��ñ�����д������������������ȿ϶�С��1
	vec4 pos = projection * view* vec4(aPos, 1.0);
	gl_Position = pos.xyww;

	//���λ����Ϊ�������������꣬cubetexture�����ľ���ԭ�㴦
	TexCoords = aPos;
}