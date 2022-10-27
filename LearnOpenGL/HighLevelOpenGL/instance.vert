#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;


out vec3 fColor;

uniform vec2 offsets[100];

//使用 uniform 传送 instance offset，但是实例过多时，offset数据量超过GLSL规定
void test1(){
	
	//使用 gl_InstanceID获取，instance的索引
	vec2 offset = offsets[gl_InstanceID];

	//该顶点位置：aPos + instance 的偏移
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