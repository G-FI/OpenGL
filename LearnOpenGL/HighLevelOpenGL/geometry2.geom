#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT{
    vec3 color;
}gs_in[];

out vec3 fColor;

void build_house(vec4 position)
{    
    //三角形的绘制是，每emitvertex就与最近的前两个点形成三角形
    fColor = gs_in[0].color;
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:左下
    EmitVertex();

    fColor = vec3(1.0, 1.0, 1.0);
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:右下
    EmitVertex();

    fColor = vec3(0.3, 0.3, 0.3);
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:左上
    EmitVertex();

    fColor = vec3(0,0,0);
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:右上
    EmitVertex();

    fColor = vec3(0.7, 0.7, 0.7);
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:顶部
    EmitVertex();
    EndPrimitive();
}

void main() {  
    build_house(gl_in[0].gl_Position);
}