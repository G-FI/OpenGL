#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;


out vec3 fColor;

uniform vec2 offsets[100];

//ʹ�� uniform ���� instance offset������ʵ������ʱ��offset����������GLSL�涨
void test1(){
	
	//ʹ�� gl_InstanceID��ȡ��instance������
	vec2 offset = offsets[gl_InstanceID];

	//�ö���λ�ã�aPos + instance ��ƫ��
	gl_Position = vec4(aPos + offset, 0, 1.0);
	fColor = aColor;

}

layout (location = 2) in vec2 aOffset;
void test2(){
	
	vec2 pos = aPos * (gl_InstanceID / 100.0);

	gl_Position = vec4(pos + aOffset, 0, 1.0);
	fColor = aColor;
}

void main(){
	test2();
}