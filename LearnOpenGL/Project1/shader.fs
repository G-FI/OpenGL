#version 330 core

uniform vec3 objColor;
uniform vec3 lightColor;

uniform float mixValue;
void main()
{
    FragColor = vec4(objColor * lightColor, 1.0);
}