#version 330 core
layout (location = 0) in vec3 aPos;
layout (location =1) in vec3 aNormal;
//变化矩阵
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//计算反射需要
out vec3 Normal;
out vec3 Position;

//输出值
out vec2 TexCoords;

void main()
{
   Normal = mat3(transpose(inverse(model))) * aNormal;
   Position = vec3(model * vec4(aPos, 1.0));

   gl_Position = projection * view * model * vec4(aPos, 1.0);
}