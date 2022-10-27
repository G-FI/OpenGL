#define STB_IMAGE_IMPLEMENTATION


#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Shader.h"
#include"stb_image.h"
#include"Camera.hpp"



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

Shader objShader;
Shader lightShader;
GLFWwindow* window;
GLFWwindow* createWindow();
unsigned int loadTexture(const char* path);
//test
void shadingTest();
void lightingMapTest();
void pointLightTest();
void spotLightTest();
void multipleLightTest();

int main()
{
    window = createWindow();
    //shadingTest();
    //lightingMapTest();
    //pointLightTest();
    //spotLightTest();
    multipleLightTest();
}

void multipleLightTest()
{
    objShader = Shader("E:\\code\\OpenGL\\LearnOpenGL\\Light\\map_shader.vs", "E:\\code\\OpenGL\\LearnOpenGL\\Light\\multiple_light_shader.frag");
    lightShader = Shader("E:\\code\\OpenGL\\LearnOpenGL\\Light\\shader.vs", "E:\\code\\OpenGL\\LearnOpenGL\\Light\\light_shader.fs");
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    glm::vec3 pointLightPositions[] = {
      glm::vec3(0.7f,  0.2f,  2.0f),
      glm::vec3(2.3f, -3.3f, -4.0f),
      glm::vec3(-4.0f,  2.0f, -12.0f),
      glm::vec3(0.0f,  0.0f, -3.0f)
    };
    unsigned int objVAO;
    glGenVertexArrays(1, &objVAO);
    glBindVertexArray(objVAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //设置顶点位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //设置顶点法线属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //设置纹理坐标
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //解绑VAO和VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //生成纹理
    unsigned int textureKD = loadTexture("C:\\Users\\65796\\Desktop\\container2.png");
    unsigned int textureKS = loadTexture("C:\\Users\\65796\\Desktop\\container2_specular.png");

    //漫反射贴图
    objShader.use();
    objShader.setInt("material.kd", 0);
    objShader.setInt("material.ks", 1);

    //开启深度检测z-buffer
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //清除fragment颜色和深度缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //设置纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureKD);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureKS);

        objShader.use();
        //观察位置，用于高光着色
        objShader.setVec3("viewPos", camera.Position);
        //dirLight属性
        objShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        objShader.setVec3("dirLight.ka", 0.05f, 0.05f, 0.05f);
        objShader.setVec3("dirLight.kd", 0.4f, 0.4f, 0.4f);
        objShader.setVec3("dirLight.ks", 0.5f, 0.5f, 0.5f);

        //pointLights属性
        objShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        objShader.setVec3("pointLights[0].ka", 0.05f, 0.05f, 0.05f);
        objShader.setVec3("pointLights[0].kd", 0.8f, 0.8f, 0.8f);
        objShader.setVec3("pointLights[0].ks", 1.0f, 1.0f, 1.0f);
        objShader.setFloat("pointLights[0].constant", 1.0f);
        objShader.setFloat("pointLights[0].linear", 0.09f);
        objShader.setFloat("pointLights[0].quadratic", 0.032f);

        objShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        objShader.setVec3("pointLights[1].ka", 0.05f, 0.05f, 0.05f);
        objShader.setVec3("pointLights[1].kd", 1.8f, 0.8f, 0.8f);
        objShader.setVec3("pointLights[1].ks", 1.0f, 1.0f, 1.0f);
        objShader.setFloat("pointLights[1].constant", 1.0f);
        objShader.setFloat("pointLights[1].linear", 0.09f);
        objShader.setFloat("pointLights[1].quadratic", 0.032f);

        objShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        objShader.setVec3("pointLights[2].ka", 0.05f, 0.05f, 0.05f);
        objShader.setVec3("pointLights[2].kd", 0.8f, 0.8f, 0.8f);
        objShader.setVec3("pointLights[2].ks", 1.0f, 1.0f, 1.0f);
        objShader.setFloat("pointLights[2].constant", 1.0f);
        objShader.setFloat("pointLights[2].linear", 0.09f);
        objShader.setFloat("pointLights[2].quadratic", 0.032f);

        objShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        objShader.setVec3("pointLights[3].ka", 0.05f, 0.05f, 0.05f);
        objShader.setVec3("pointLights[3].kd", 0.8f, 0.8f, 0.8f);
        objShader.setVec3("pointLights[3].ks", 1.0f, 1.0f, 1.0f);
        objShader.setFloat("pointLights[3].constant", 1.0f);
        objShader.setFloat("pointLights[3].linear", 0.09f);
        objShader.setFloat("pointLights[3].quadratic", 0.032f);

        //spotLight属性
        objShader.setVec3("spotLight.ka", 0.0f, 0.0f, 0.0f);
        objShader.setVec3("spotLight.kd", 1.0f, 1.0f, 1.0f);
        objShader.setVec3("spotLight.ks", 1.0f, 1.0f, 1.0f);

        objShader.setFloat("spotLight.constant", 1.0f);
        objShader.setFloat("spotLight.linear", 0.09f);
        objShader.setFloat("spotLight.quadratic", 0.032f);

        objShader.setVec3("spotLight.position", camera.Position);
        objShader.setVec3("spotLight.direction", camera.Front);
        objShader.setFloat("spotLight.cutOff", cos(glm::radians(12.5f)));
        objShader.setFloat("spotLight.outerCutOff", cos(glm::radians(15.f)));




        //材质属性
        objShader.setFloat("material.shininess", 64.0f);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 project = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.f);
        objShader.setMat4fv("view", view);
        objShader.setMat4fv("project", project);

        //物体mvp变换
        glBindVertexArray(objVAO);
        for (int i = 0; i < 10; i++)
        {
            glm::mat4 model(1.f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            objShader.setMat4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }




        //设置light颜色以及变换
        lightShader.use();
       

        lightShader.setMat4fv("view", view);
        lightShader.setMat4fv("project", project);
        for (int i = 0; i < 4; i++)
        {
            glm::mat4 model = glm::mat4(1.f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightShader.setMat4fv("model", model);

            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }
        

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &lightVAO);
    glDeleteVertexArrays(1, &objVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
}
void spotLightTest()
{
    objShader = Shader("E:\\code\\OpenGL\\LearnOpenGL\\Light\\map_shader.vs", "E:\\code\\OpenGL\\LearnOpenGL\\Light\\spot_light_obj_shader.fs");
    lightShader = Shader("E:\\code\\OpenGL\\LearnOpenGL\\Light\\shader.vs", "E:\\code\\OpenGL\\LearnOpenGL\\Light\\light_shader.fs");
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int objVAO;
    glGenVertexArrays(1, &objVAO);
    glBindVertexArray(objVAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //设置顶点位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //设置顶点法线属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //设置纹理坐标
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //解绑VAO和VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //生成纹理
    unsigned int textureKD = loadTexture("C:\\Users\\65796\\Desktop\\container2.png");
    unsigned int textureKS = loadTexture("C:\\Users\\65796\\Desktop\\container2_specular.png");

    //漫反射贴图
    objShader.use();
    objShader.setInt("material.kd", 0);
    objShader.setInt("material.ks", 1);

    //开启深度检测z-buffer
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //清除fragment颜色和深度缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //设置纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureKD);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureKS);

        objShader.use();
        //观察位置，用于高光着色
        objShader.setVec3("viewPos", camera.Position);
        //light属性
        objShader.setVec3("light.ka", 0.2f, 0.2f, 0.2f);
        objShader.setVec3("light.kd", 0.5f, 0.5f, 0.5f);
        objShader.setVec3("light.ks", 1.0f, 1.0f, 1.0f);

        objShader.setFloat("light.constant", 1.0f);
        objShader.setFloat("light.linear", 0.09f);
        objShader.setFloat("light.quadratic", 0.032f);

        objShader.setVec3("light.position", camera.Position);
        objShader.setVec3("light.direction", camera.Front);
        objShader.setFloat("light.cutOff", cos(glm::radians(12.5f)));
        objShader.setFloat("light.outerCutOff", cos(glm::radians(17.5f)));

        
       

        //材质属性
        objShader.setFloat("material.shininess", 64.0f);

        glm::mat4 model(1.f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 project = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.f);
        objShader.setMat4fv("view", view);
        objShader.setMat4fv("project", project);

        //物体mvp变换
        glBindVertexArray(objVAO);
        for (int i = 0; i < 10; i++)
        {
            glm::mat4 model(1.f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            objShader.setMat4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }




        //设置light颜色以及变换
        lightShader.use();
        model = glm::mat4(1.f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightShader.setMat4fv("model", model);
        lightShader.setMat4fv("view", view);
        lightShader.setMat4fv("project", project);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &lightVAO);
    glDeleteVertexArrays(1, &objVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
}
void pointLightTest()
{
    objShader = Shader("E:\\code\\OpenGL\\LearnOpenGL\\Light\\map_shader.vs", "E:\\code\\OpenGL\\LearnOpenGL\\Light\\point_light_obj_shader.fs");
    lightShader = Shader("E:\\code\\OpenGL\\LearnOpenGL\\Light\\shader.vs", "E:\\code\\OpenGL\\LearnOpenGL\\Light\\light_shader.fs");
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int objVAO;
    glGenVertexArrays(1, &objVAO);
    glBindVertexArray(objVAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //设置顶点位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //设置顶点法线属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //设置纹理坐标
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //解绑VAO和VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //生成纹理
    unsigned int textureKD = loadTexture("C:\\Users\\65796\\Desktop\\container2.png");
    unsigned int textureKS = loadTexture("C:\\Users\\65796\\Desktop\\container2_specular.png");

    //漫反射贴图
    objShader.use();
    objShader.setInt("material.kd", 0);
    objShader.setInt("material.ks", 1);

    //开启深度检测z-buffer
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //清除fragment颜色和深度缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //设置纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureKD);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureKS);

        objShader.use();
        //观察位置，用于高光着色
        objShader.setVec3("viewPos", camera.Position);
        //light属性
        objShader.setVec3("light.ka", 0.2f, 0.2f, 0.2f);
        objShader.setVec3("light.kd", 0.5f, 0.5f, 0.5f);
        objShader.setVec3("light.ks", 1.0f, 1.0f, 1.0f);
        objShader.setVec3("light.position", lightPos);
        objShader.setFloat("light.constant", 1.0f);
        objShader.setFloat("light.linear", 0.09f);
        objShader.setFloat("light.quadratic", 0.032f);

        //材质属性
        objShader.setFloat("material.shininess", 64.0f);

        glm::mat4 model(1.f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 project = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.f);
        objShader.setMat4fv("view", view);
        objShader.setMat4fv("project", project);

        //物体mvp变换
        glBindVertexArray(objVAO);
        for (int i = 0; i < 10; i++)
        {
            glm::mat4 model(1.f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
           
            objShader.setMat4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        

        

        //设置light颜色以及变换
        lightShader.use();
        model = glm::mat4(1.f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightShader.setMat4fv("model", model);
        lightShader.setMat4fv("view", view);
        lightShader.setMat4fv("project", project);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &lightVAO);
    glDeleteVertexArrays(1, &objVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
}
void lightingMapTest()
{
    objShader = Shader("E:\\code\\OpenGL\\LearnOpenGL\\Light\\map_shader.vs", "E:\\code\\OpenGL\\LearnOpenGL\\Light\\map_shader.fs");
    lightShader = Shader("E:\\code\\OpenGL\\LearnOpenGL\\Light\\shader.vs", "E:\\code\\OpenGL\\LearnOpenGL\\Light\\light_shader.fs");
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    unsigned int objVAO;
    glGenVertexArrays(1, &objVAO);
    glBindVertexArray(objVAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //设置顶点位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //设置顶点法线属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //设置纹理坐标
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //解绑VAO和VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //生成纹理
    unsigned int textureKD = loadTexture("C:\\Users\\65796\\Desktop\\container2.png");
    unsigned int textureKS = loadTexture("C:\\Users\\65796\\Desktop\\container2_specular.png");

    //漫反射贴图
    objShader.use();
    objShader.setInt("material.kd", 0);
    objShader.setInt("material.ks", 1);

    //开启深度检测z-buffer
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //清除fragment颜色和深度缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
        //设置纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureKD);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureKS);

        objShader.use();
        //观察位置，用于高光着色
        objShader.setVec3("viewPos", camera.Position);
        //light属性
        objShader.setVec3("light.ka", 0.2f, 0.2f, 0.2f);
        objShader.setVec3("light.kd", 0.5f, 0.5f, 0.5f);
        objShader.setVec3("light.ks", 1.0f, 1.0f, 1.0f);
        objShader.setVec3("light.position", lightPos);

        //材质属性
        objShader.setFloat("material.shininess", 64.0f);


        glm::mat4 model(1.f);
        model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 project = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.f);

        //物体mvp变换
        objShader.setMat4fv("model", model);
        objShader.setMat4fv("view", view);
        objShader.setMat4fv("project", project);

        glBindVertexArray(objVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //设置light颜色以及变换
        lightShader.use();
        model = glm::mat4(1.f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightShader.setMat4fv("model", model);
        lightShader.setMat4fv("view", view);
        lightShader.setMat4fv("project", project);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &lightVAO);
    glDeleteVertexArrays(1, &objVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
}
void shadingTest()
{
    objShader = Shader("E:\\code\\OpenGL\\LearnOpenGL\\Light\\shader.vs", "E:\\code\\OpenGL\\LearnOpenGL\\Light\\obj_shader.fs");
    lightShader = Shader("E:\\code\\OpenGL\\LearnOpenGL\\Light\\shader.vs", "E:\\code\\OpenGL\\LearnOpenGL\\Light\\light_shader.fs");
    float vertices[] = {
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
    
    unsigned int objVAO;
    glGenVertexArrays(1, &objVAO);
    glBindVertexArray(objVAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //设置顶点位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //设置顶点法线属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //解绑VAO和VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //开启深度检测z-buffer
    glEnable(GL_DEPTH_TEST);


    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //清除fragment颜色和深度缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        objShader.use();
        //观察位置，用于高光着色
        objShader.setVec3("viewPos", camera.Position);
        //light属性
        objShader.setVec3("light.ka", 0.2f, 0.2f, 0.2f);
        objShader.setVec3("light.kd", 0.5f, 0.5f, 0.5f);
        objShader.setVec3("light.ks", 1.0f, 1.0f, 1.0f);
        objShader.setVec3("light.position", lightPos);

        //材质属性
        objShader.setVec3("material.ka", 1.0f, 0.5f, 0.31f);
        objShader.setVec3("material.kd", 1.0f, 0.5f, 0.31f);
        objShader.setVec3("material.ks", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
        objShader.setFloat("material.shininess", 32.0f);

        glm::mat4 model(1.f);
        model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 project = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.f);
        
        //mvp变换
        objShader.setMat4fv("model", model);
        objShader.setMat4fv("view", view);
        objShader.setMat4fv("project", project);


        glBindVertexArray(objVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //设置lightShader以及变换
        lightShader.use();
        model = glm::mat4(1.f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightShader.setMat4fv("model", model);
        lightShader.setMat4fv("view", view);
        lightShader.setMat4fv("project", project);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }

    glDeleteVertexArrays(1, &lightVAO);
    glDeleteVertexArrays(1, &objVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
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
    //stbi_set_flip_vertically_on_load(true);
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