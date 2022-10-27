#version 330 core



out vec4 FragColor;


void main()
{
//    if(texColor.a < 0.1)
//        discard;
    FragColor = vec4(0.04, 0.28, 0.26,1.0);

    //FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}