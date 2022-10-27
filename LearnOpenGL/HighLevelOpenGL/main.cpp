#define STB_IMAGE_IMPLEMENTATION


#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<vector>
#include<map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Shader.h"
#include"stb_image.h"
#include"Camera.hpp"

//ImageUI
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3.h>

//call_backs
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double posX, double posY);
void scroll_callback(GLFWwindow* window, double offsetX, double offsetY);
void processInput(GLFWwindow* window);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float aspect = SCR_WIDTH / SCR_HEIGHT;

//相机
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float deltaTime = 0.f;
float lastFrame = 0.f;
//鼠标输入
bool firstMouse = true;
double lastX = SCR_WIDTH / 2;
double lastY = SCR_HEIGHT / 2;

//light
glm::vec3 lightPos(1.f, 1.f, 1.f);


GLFWwindow* window;
GLFWwindow* createWindow();
unsigned int loadTexture(const char* path);
unsigned int loadCubeTexture(const std::vector<std::string>& texture_faces);

//test
void BlendTest1();
void BlendTest2();
void StencilTest();
void CullFaceTest();
void FrameBufferTest();
void CubeTexutreTest();
void CubeTextureReflect();
void GeometryTest1();
void GeometryTest2();
void GeometryTest3();
void Instance1();

int main()
{
    window = createWindow();
    //BlendTest1();
    
    //BlendTest2();
    //StencilTest();
    //CullFaceTest();
    //FrameBufferTest();
    //CubeTexutreTest();
    //CubeTextureReflect();
    //GeometryTest1();
    //GeometryTest2();
    Instance1();
}
void Instance1() 
{
    Shader shader("E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\instance.vert", "E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\instance.frag");

    float quadVertices[] = {
        // 位置          // 颜色
        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
         0.05f,  0.05f,  0.0f, 1.0f, 1.0f
    };


    //设置instance的offset array
    glm::vec2 offsets[100];
    int idx = 0;
    float off = 0.1f;
    for (int y = -10; y < 10; y += 2) {
        for (int x = -10; x < 10; x += 2) {
            glm::vec2 offset;
            offset.x = (float)x / 10 + off;
            offset.y = (float)y / 10 + off;
            offsets[idx++] = offset;
        }
    }

    shader.use();
    //将offset array传入shader
    /*for (int i = 0; i < 100; ++i) {
        std::string name{ "offsets[" + std::to_string(i) + "]" };
        shader.setVec2(name, offsets[i]);
    }*/



    unsigned int vao, vbo, instance_vbo;
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
       
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

        glGenBuffers(1, &instance_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, instance_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(offsets), &offsets[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        //默认情况下，属性除数是0，告诉OpenGL我们需要在顶点着色器的每次迭代时更新顶点属性。将它设置为1时，我们告诉OpenGL我们希望在渲染一个新实例的时候更新顶点属性。
        glVertexAttribDivisor(2, 1);

        glBindVertexArray(0);
    }
    unsigned int vao2;
    glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);

    glBindBuffer(GL_ARRAY_BUFFER, instance_vbo);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    //默认情况下，属性除数是0，告诉OpenGL我们需要在顶点着色器的每次迭代时更新顶点属性。将它设置为1时，我们告诉OpenGL我们希望在渲染一个新实例的时候更新顶点属性。
    glVertexAttribDivisor(2, 1);


    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //glBindVertexArray(vao);
    while (!glfwWindowShouldClose(window)) {
        {
            processInput(window);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
       
        //说明绘制方式是以6个顶点作为一个实例，绘制100个实例
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);

        {
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
   
    
}

void GemotryTest3()
{
    //Model model("E:\\code\\OpenGL\\LearnOpenGL\\Assets\\nanosuit\\nanosuit.obj");

}

void GeometryTest2()
{
    float points[] = {
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // 左上
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // 右上
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 右下
    -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // 左下
    };

    unsigned int VAO, VBO;
    {
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

        glBindVertexArray(0);
    }
    Shader shader("E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\geometry2.vert", "E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\geometry2.frag", "E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\geometry2.geom");
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1, 0.1, 0.1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 4);

        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}
void GeometryTest1()
{
    float points[] = {
        -0.5f, 0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f
    };

    unsigned int VAO, VBO;
    {
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
       
        glBindVertexArray(0);
    }
    Shader shader("E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\geometry.vert", "E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\geometry.frag", "E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\geometry.geom");
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1, 0.1, 0.1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 4);
        
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void FrameBufferTest()
{
    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };
    float screenVertices[] = {
        -1.f, 1.f,    0.f, 1.f,
        -1.f, -1.f,    0.f, 0.f,
        1.f, -1.f,       1.f, 0.f,

        -1.f, 1.f,    0.f, 1.f,
        1.f, -1.f,      1.f, 0.f,
        1.f, 1.f,       1.f, 1.f,

    };
    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    unsigned int planeVAO, planeVBO;
    unsigned int screenVAO, screenVBO;
    {
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);

        glGenVertexArrays(1, &planeVAO);
        glGenBuffers(1, &planeVBO);
        glBindVertexArray(planeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);

        glGenVertexArrays(1, &screenVAO);
        glBindVertexArray(screenVAO);
        glGenBuffers(1, &screenVBO);
        glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), &screenVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    }


    unsigned int cubeTexture = loadTexture("E:\\code\\OpenGL\\LearnOpenGL\\Assets\\container.jpg");
    unsigned int floorTexture = loadTexture("E:\\code\\OpenGL\\LearnOpenGL\\Assets\\metal.png");
    
    Shader shader_normal("E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\shader.vert", "E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\shader.frag");
    Shader shader_buffer("E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\framebuffer.vert", "E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\framebuffer.frag");

    shader_normal.use();
    shader_normal.setInt("texture1", 0);

    shader_buffer.use();
    shader_buffer.setInt("screenTexture", 0);
   

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    /*完整帧缓冲需要的条件
        附加至少一个缓冲（颜色、深度或模板缓冲）。
        至少有一个颜色附件(Attachment)。
        所有的附件都必须是完整的（保留了内存）。
        每个缓冲都应该有相同的样本数。
    */
    //1.纹理附件,接下来我们需要创建一个纹理图像，我们将它作为一个颜色附件附加到帧缓冲上
    unsigned int screenTexture;
    glGenTextures(1, &screenTexture);
    glBindTexture(GL_TEXTURE_2D, screenTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);//只有内存没有数据
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);//解绑

    //附加到帧缓冲
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);//帧目标，想要附加的附件类型，纹理，多级渐远纹理的级别

    //2.想要深度，模板测试，需要添加深度，模板附件到帧缓冲中，由于不需要采样，所以使用帧缓冲对象rbo
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);//配置渲染缓冲对对象格式，24为用于深度，8位用于模板
    glBindRenderbuffer(GL_RENDERBUFFER, 0);//解绑

    //3. 将渲染缓冲对象附加到缓冲帧中
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);


    //检查帧缓冲是否完整。它将会检测当前绑定的帧缓冲，并返回规范中这些值的其中之一
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {   //之后所有的渲染操作将会渲染到当前绑定帧缓冲的附件中，也称离屏渲染
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /*
    要想绘制场景到一个纹理上，我们需要采取以下的步骤：

    将新的帧缓冲绑定为激活的帧缓冲，和往常一样渲染场景
    绑定默认的帧缓冲
    绘制一个横跨整个屏幕的四边形，将帧缓冲的颜色缓冲作为它的纹理。
    */

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        //将绘制在framebuffer这个帧缓冲中
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH);

        shader_normal.use();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shader_normal.setMat4fv("view", view);
        shader_normal.setMat4fv("projection", projection);



        // floor
        {
            glBindVertexArray(planeVAO);
            glBindTexture(GL_TEXTURE_2D, floorTexture);
            shader_normal.setMat4fv("model", glm::mat4(1.0f));
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }


        // cubes
        {
            glBindVertexArray(cubeVAO);
            glBindTexture(GL_TEXTURE_2D, cubeTexture);
            model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
            shader_normal.setMat4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
            shader_normal.setMat4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);//复制缓冲不用深度测试

        //绘制缓冲帧到默人屏幕帧中
        shader_buffer.use();
        glBindVertexArray(screenVAO);
        glBindTexture(GL_TEXTURE_2D, screenTexture);
       //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteFramebuffers(1, &framebuffer);
}
void CubeTextureReflect()
{

        float cube_vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };
        float skybox_vertices[] = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };
        // cube VAO
        unsigned int skybox_vao, skybox_vbo;
        {
            glGenVertexArrays(1, &skybox_vao);
            glGenBuffers(1, &skybox_vbo);
            glBindVertexArray(skybox_vao);
            glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
          
            glBindVertexArray(0);
           
        }
        unsigned int cube_vao, cube_vbo;
        {
            glGenVertexArrays(1, &cube_vao);
            glGenBuffers(1, &cube_vbo);
            glBindVertexArray(cube_vao);
            glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), &cube_vertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        }

        //创建天空盒纹理
        std::vector<std::string> texture_faces{
            "E:\\code\\OpenGL\\LearnOpenGL\\Assets\\skybox\\right.jpg",
            "E:\\code\\OpenGL\\LearnOpenGL\\Assets\\skybox\\left.jpg",
            "E:\\code\\OpenGL\\LearnOpenGL\\Assets\\skybox\\top.jpg",
            "E:\\code\\OpenGL\\LearnOpenGL\\Assets\\skybox\\bottom.jpg",
            "E:\\code\\OpenGL\\LearnOpenGL\\Assets\\skybox\\front.jpg",
             "E:\\code\\OpenGL\\LearnOpenGL\\Assets\\skybox\\back.jpg",

        };
        unsigned int skybox_texture = loadCubeTexture(texture_faces);
        Shader skybox_shader("E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\cube_texture.vert", "E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\cube_texture.frag");
        skybox_shader.use();
        skybox_shader.setInt("skybox", 0);

        //创建普通纹理
        Shader shader("E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\shader_reflect.vert", "E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\shader_reflect.frag");
        shader.use();
        shader.setInt("skybox", 0);

        glEnable(GL_DEPTH_TEST);


        while (!glfwWindowShouldClose(window))
        {
            {
                processInput(window);
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }

           
            {//优化
                {
                    shader.use();
                    glm::mat4 model = glm::mat4(1.f);
                   
                    glm::mat4 view = camera.GetViewMatrix();
                    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                    shader.setMat4fv("model", model);
                    shader.setMat4fv("view", view);
                    shader.setMat4fv("projection", projection);
                    shader.setVec3("cameraPos", camera.Position);

                    glBindVertexArray(cube_vao);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                    glBindVertexArray(0);
                }
                {
                    glDepthFunc(GL_LEQUAL);
                    skybox_shader.use();
                    //glm::mat4 view = camera.GetViewMatrix();
                    //box纹理是以（0，0，0）为原点，使用方向向量采样，而skybox的顶点坐标也是以（0，0，0）为原点的地点集合，所以传入的skybox顶点位置恰好为采样带点的方向向量
                    //如此将纹理会知道立方体上，但是通过view变化后立方体在视野外(因为有移动的分量)，所以将移动分量去掉的话，就相当于，相机始终在skybox中，也就相当于天空盒跟着相机走

                    glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
                    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                    skybox_shader.setMat4fv("view", view);
                    skybox_shader.setMat4fv("projection", projection);

                    glBindVertexArray(skybox_vao);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                    glBindVertexArray(0);
                    glDepthFunc(GL_LESS);
                }
            }

            {
                glfwSwapBuffers(window);
                glfwPollEvents();
            }

        }
        glDeleteBuffers(1, &skybox_vbo);
        glDeleteVertexArrays(1, &skybox_vao);

}
void CubeTexutreTest()
{
    float cube_vertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    float skybox_vertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    // cube VAO
    unsigned int skybox_vao, skybox_vbo;
    {
        glGenVertexArrays(1, &skybox_vao);
        glGenBuffers(1, &skybox_vbo);
        glBindVertexArray(skybox_vao);
        glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindVertexArray(0);
    }
    unsigned int cube_vao, cube_vbo;
    {
        glGenVertexArrays(1, &cube_vao);
        glGenBuffers(1, &cube_vbo);
        glBindVertexArray(cube_vao);
        glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), &cube_vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }

    //创建天空盒纹理
    std::vector<std::string> texture_faces{
        "E:\\code\\OpenGL\\LearnOpenGL\\Assets\\skybox\\right.jpg",
        "E:\\code\\OpenGL\\LearnOpenGL\\Assets\\skybox\\left.jpg",
        "E:\\code\\OpenGL\\LearnOpenGL\\Assets\\skybox\\top.jpg",
        "E:\\code\\OpenGL\\LearnOpenGL\\Assets\\skybox\\bottom.jpg",
        "E:\\code\\OpenGL\\LearnOpenGL\\Assets\\skybox\\front.jpg",
         "E:\\code\\OpenGL\\LearnOpenGL\\Assets\\skybox\\back.jpg",

    };
    unsigned int skybox_texture = loadCubeTexture(texture_faces);
    Shader skybox_shader("E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\cube_texture.vert", "E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\cube_texture.frag");
    skybox_shader.use();
    skybox_shader.setInt("skybox", 0);

    //创建普通纹理
    Shader shader("E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\shader.vert", "E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\shader.frag");
    unsigned int cube_texture = loadTexture("E:\\code\\OpenGL\\LearnOpenGL\\Assets\\container.jpg");
    shader.use();
    shader.setInt("texture1", 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);//多重采样
    
    while (!glfwWindowShouldClose(window))
    {
        {
            processInput(window);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        /* {
            glDepthMask(GL_FALSE);//禁止深度写，使得天空盒不写入深度缓存，一直作为背景
            {//绘制skybox
                skybox_shader.use();
                //glm::mat4 view = camera.GetViewMatrix();
                //box纹理是以（0，0，0）为原点，使用方向向量采样，而skybox的顶点坐标也是以（0，0，0）为原点的地点集合，所以传入的skybox顶点位置恰好为采样带点的方向向量
                //如此将纹理会知道立方体上，但是通过view变化后立方体在视野外(因为有移动的分量)，所以将移动分量去掉的话，就相当于，相机始终在skybox中，也就相当于天空盒跟着相机走
                glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
                glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                skybox_shader.setMat4fv("view", view);
                skybox_shader.setMat4fv("projection", projection);

                glBindVertexArray(skybox_vao);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
            }

            glDepthMask(GL_TRUE);//打开深度写绘制其余场景
            {
                shader.use();
                glm::mat4 model = glm::mat4(1.f);
                model = glm::translate(model, glm::vec3(3.f, 3.f, 3.f));
                glm::mat4 view = camera.GetViewMatrix();
                glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                shader.setMat4fv("model", model);
                shader.setMat4fv("view", view);
                shader.setMat4fv("projection", projection);

                glBindVertexArray(cube_vao);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, cube_texture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
            }
        }*/
        {//优化
            {
                shader.use();
                glm::mat4 model = glm::mat4(1.f);
                model = glm::translate(model, glm::vec3(3.f, 3.f, 3.f));
                glm::mat4 view = camera.GetViewMatrix();
                glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                shader.setMat4fv("model", model);
                shader.setMat4fv("view", view);
                shader.setMat4fv("projection", projection);

                glBindVertexArray(cube_vao);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, cube_texture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
            }
            {
                glDepthFunc(GL_LEQUAL);
                skybox_shader.use();
                //glm::mat4 view = camera.GetViewMatrix();
                //box纹理是以（0，0，0）为原点，使用方向向量采样，而skybox的顶点坐标也是以（0，0，0）为原点的地点集合，所以传入的skybox顶点位置恰好为采样带点的方向向量
                //如此将纹理会知道立方体上，但是通过view变化后立方体在视野外(因为有移动的分量)，所以将移动分量去掉的话，就相当于，相机始终在skybox中，也就相当于天空盒跟着相机走
                
                glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
                glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                skybox_shader.setMat4fv("view", view);
                skybox_shader.setMat4fv("projection", projection);

                glBindVertexArray(skybox_vao);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
                glDepthFunc(GL_LESS);
            }
        }

        {
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    }
    glDeleteBuffers(1, &skybox_vbo);
    glDeleteVertexArrays(1, &skybox_vao);

}
void StencilTest()
{
    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };

    Shader shader_normal("E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\shader.vert", "E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\shader.frag");
    Shader shader_single("E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\shader.vert", "E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\stencil_single.frag");
   
    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    unsigned int cubeTexture = loadTexture("E:\\code\\OpenGL\\LearnOpenGL\\Assets\\marble.jpg");
    unsigned int floorTexture = loadTexture("E:\\code\\OpenGL\\LearnOpenGL\\Assets\\metal.png");

    shader_normal.use();
    shader_normal.setInt("texture1", 0);

    //全局配置
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);//模板测试失败，模板成功但深度失败，都成功

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT);


        
        shader_single.use();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shader_single.setMat4fv("view", view);
        shader_single.setMat4fv("projection", projection);

        shader_normal.use();
        shader_normal.setMat4fv("view", view);
        shader_normal.setMat4fv("projection", projection);


        // floor
        {
            //绘制地板时，不进行写模板
            glStencilMask(0x00);

            glBindVertexArray(planeVAO);
            glBindTexture(GL_TEXTURE_2D, floorTexture);
            shader_normal.setMat4fv("model", glm::mat4(1.0f));
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }


        // cubes
        {
            //绘制正常立方体时写模板，并将其有像素的位置置为一
            glStencilFunc(GL_ALWAYS, 1, 0xff);  //使用下面立方体制造模板
            glStencilMask(0xff);//开启模板写入


            glBindVertexArray(cubeVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, cubeTexture);
            model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
            shader_normal.setMat4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
            shader_normal.setMat4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // scaled cubes
        {
            //绘制正常立方体时写模板，并将其有像素的位置置为一
            glStencilFunc(GL_NOTEQUAL, 1, 0xff);  //模板值不等于一时绘制，也就是原箱子轮廓，本来这些位置的模板会被ref（1）重写，但是使用glStencilMask关掉模板缓冲写了
            glStencilMask(0x00);
            glDisable(GL_DEPTH_TEST); //关闭缓冲，让被放大的箱子不会被其他地方覆盖
            shader_single.use();


            glBindVertexArray(cubeVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, cubeTexture);
            glm::mat4 model = glm::mat4(1.f);
            model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
            model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
            shader_single.setMat4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
            shader_single.setMat4fv("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        
        glBindVertexArray(0);
        glStencilFunc(GL_ALWAYS, 0, 0xff);

        glEnable(GL_DEPTH_TEST);
        glStencilMask(0xff);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);
}
void CullFaceTest()
{
    Shader shader("E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\shader.vert", "E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\shader.frag");
    float cubeVertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        // Right face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
    };
}
void BlendTest2()
{
    Shader shader("E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\shader.vert", "E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\shader.frag");
    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };

    float rectVertices[] = {
         -0.5f, -0.5f, 0.f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.f,  1.0f, 0.0f,
         0.5f,  0.5f, 0.f,  1.0f, 1.0f,
         0.5f,  0.5f, 0.f,  1.0f, 1.0f,
        -0.5f,  0.5f, 0.f,  0.0f, 1.0f,
        -0.5f, -0.5f, 0.f,  0.0f, 0.0f,
    };

    std::vector<glm::vec3> windowPositions;
    windowPositions.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    windowPositions.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
    windowPositions.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    windowPositions.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    windowPositions.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    //rectangle VAO
    unsigned int rectVAO, rectVBO;
    glGenVertexArrays(1, &rectVAO);
    glBindVertexArray(rectVAO);
    glGenBuffers(1, &rectVBO);
    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices), rectVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // load textures
    // -------------
    unsigned int cubeTexture = loadTexture("E:\\code\\OpenGL\\LearnOpenGL\\Assets\\marble.jpg");
    unsigned int floorTexture = loadTexture("E:\\code\\OpenGL\\LearnOpenGL\\Assets\\metal.png");
    unsigned int blendTexture = loadTexture("E:\\code\\OpenGL\\LearnOpenGL\\Assets\\blending_transparent_window.png");

    shader.use();
    shader.setInt("texture1", 0);

    //深度测试
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LESS);

    //开启混合
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4fv("view", view);
        shader.setMat4fv("projection", projection);
        // cubes
        {
            glBindVertexArray(cubeVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, cubeTexture);
            model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
            shader.setMat4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
            shader.setMat4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // floor
        {
            glBindVertexArray(planeVAO);
            glBindTexture(GL_TEXTURE_2D, floorTexture);
            shader.setMat4fv("model", glm::mat4(1.0f));
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }

        //blend window
        {
            glBindVertexArray(rectVAO);
            glBindTexture(GL_TEXTURE_2D, blendTexture);

            std::map<float, glm::vec3> sortedPostions;
            for (int i = 0; i < windowPositions.size(); i++)
            {
                float length = glm::length(camera.Position - windowPositions[i]);
                sortedPostions[length] = windowPositions[i];
            }



            for (std::map<float, glm::vec3>::reverse_iterator it = sortedPostions.rbegin(); it != sortedPostions.rend(); ++it)
            {
                model = glm::mat4(1.0f);
                model = glm::translate(model, it->second);
                shader.setMat4fv("model", model);
                //由后往前，绘制时考虑背景与当前颜色的混合
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }
        glBindVertexArray(0);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);
}
void BlendTest1()
{
    Shader shader("E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\shader.vert", "E:\\code\\OpenGL\\LearnOpenGL\\HighLevelOpenGL\\shader.frag");
    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };

    float rectVertices[] = {
         -0.5f, -0.5f, 0.f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.f,  1.0f, 0.0f,
         0.5f,  0.5f, 0.f,  1.0f, 1.0f,
         0.5f,  0.5f, 0.f,  1.0f, 1.0f,
        -0.5f,  0.5f, 0.f,  0.0f, 1.0f,
        -0.5f, -0.5f, 0.f,  0.0f, 0.0f,
    };

    std::vector<glm::vec3> vegetation;
    vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
    vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    //rectangle VAO
    unsigned int rectVAO, rectVBO;
    glGenVertexArrays(1, &rectVAO);
    glBindVertexArray(rectVAO);
    glGenBuffers(1, &rectVBO);
    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices), rectVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // load textures
    // -------------
    unsigned int cubeTexture = loadTexture("C:\\Users\\65796\\Desktop\\marble.jpg");
    unsigned int floorTexture = loadTexture("C:\\Users\\65796\\Desktop\\metal.png");
    unsigned int vegetationTexture = loadTexture("C:\\Users\\65796\\Desktop\\grass.png");

    shader.use();
    shader.setInt("texture1", 0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4fv("view", view);
        shader.setMat4fv("projection", projection);
        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.setMat4fv("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.setMat4fv("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // floor
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        shader.setMat4fv("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        //vegetation
        glBindVertexArray(rectVAO);
        glBindTexture(GL_TEXTURE_2D, vegetationTexture);
        for (int i = 0; i < vegetation.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, vegetation[i]);
            shader.setMat4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            model = glm::rotate(model, glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
            shader.setMat4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);


            model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
            shader.setMat4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            model = glm::rotate(model, glm::radians(135.f), glm::vec3(0.f, 1.f, 0.f));
            shader.setMat4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        glBindVertexArray(0);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);
}





GLFWwindow* createWindow()
{
    // glfw: initialize and configure
 // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //msaa
    glfwWindowHint(GLFW_SAMPLES, 4);
       // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    //��׽��꣬�γ�FPS���ϵͳ
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //ȫ�ֻص�����
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }
    return window;
}
unsigned int loadTexture(const char* path)
{
    ////创建纹理并绑定
    unsigned int textureID;
    glGenTextures(1, &textureID);

   
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        //绑定两个纹理到两个纹理单元
        glBindTexture(GL_TEXTURE_2D, textureID);
        // 为当前绑定的纹理对象设置环绕、过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return textureID;
}
unsigned int loadCubeTexture(const std::vector<std::string>& texture_faces)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

        int width, height, nrChannels;
        unsigned char* data;
        for (int i = 0; i < texture_faces.size(); i++)
        {
            data = stbi_load(texture_faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                //创建纹理
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);//创建纹理之后就可以释放内存
            }
            else
            {
                std::cout << "cube map load failure: " << texture_faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        //纹理环绕和过滤
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
        return texture;
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

        
    {//设置相机
        float currentFarme = (float)glfwGetTime();
        deltaTime = currentFarme - lastFrame;
        lastFrame = currentFarme;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
           camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
            std::cout << "cameraPos: " << camera.Position.x << "," << camera.Position.y << ", " << camera.Position.z << std::endl;

        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
            std::cout << "cameraPos: " << camera.Position.x << "," << camera.Position.y << ", " << camera.Position.z << std::endl;

        }if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
            std::cout << "cameraPos: " << camera.Position.x << "," << camera.Position.y << ", " << camera.Position.z << std::endl;
                
        }if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
            std::cout << "cameraPos: " << camera.Position.x << "," << camera.Position.y << ", " << camera.Position.z << std::endl;

        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double posX, double posY)
{
    if (firstMouse)
    {
        lastX = posX;
        lastY = posY;
        firstMouse = false;
    }
    float offsetX = posX - lastX;
    float offsetY = lastY - posY;

    lastX = posX;
    lastY = posY;
    
    camera.ProcessMouseMovement(offsetX, offsetY);

    std::cout << "cameraFront: " << camera.Front.x << ", " << camera.Front.y << ", " << camera.Front.z << std::endl;
    std::cout << "cameraUp: " << camera.Up.x << ", " << camera.Up.y << ", " << camera.Up.z << std::endl;
}
void scroll_callback(GLFWwindow* window, double offsetX, double offsetY)
{
    camera.ProcessMouseScroll(offsetY);
    std::cout << "fov: " << camera.Zoom << std::endl;
}