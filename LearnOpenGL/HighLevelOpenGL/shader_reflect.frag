#version 330 core

in vec3 Normal;
in vec3 Position;


//������Ҫʹ��skybox����
uniform samplerCube skybox;
uniform vec3 cameraPos;

out vec4 FragColor;


void main()
{
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    vec4 texColor = vec4(texture(skybox, R).rgb, 1.0);
    FragColor = texColor;
}