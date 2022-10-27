#define STB_IMAGE_IMPLEMENTATION
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

#include <Shader.h>
#include <Camera.hpp>
#include <gl_object.hpp>
#include <stb_image.h>


#include<iostream>
#include<string>
#include<vector>

GLFWwindow* createWindow();
unsigned int loadTexture(const char* path);
unsigned int loadCubeTexture(const std::vector<std::string>& texture_faces);

//call_backs
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double posX, double posY);
void scroll_callback(GLFWwindow* window, double offsetX, double offsetY);
void processInput(GLFWwindow* window);

// settings
GLFWwindow* window;
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

//test
void ShadowMapping();

int main() {
    window = createWindow();
    ShadowMapping();
}

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
             // bottom face
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
              1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             // top face
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
              1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
              1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
              1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void renderScene(const VAO& planeVAO , const Shader& shader)
{
    // floor
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4fv("model", model);
    glBindVertexArray(planeVAO.ID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // cubes
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4fv("model", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4fv("model", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.25));
    shader.setMat4fv("model", model);
    renderCube();
}


void ShadowMapping() {
    Shader simpleDepthshader("E:\\code\\OpenGL\\LearnOpenGL\\AdvancedLighting\\simpleDepthShader.vert", "E:\\code\\OpenGL\\LearnOpenGL\\AdvancedLighting\\simpleDepthShader.frag");
    Shader debugDepthQuad("E:\\code\\OpenGL\\LearnOpenGL\\AdvancedLighting\\debug_quad_depth.vert", "E:\\code\\OpenGL\\LearnOpenGL\\AdvancedLighting\\debug_quad_depth.frag");
    float planeVertices[] = {
        // positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };
    VAO planeVAO;
    planeVAO.BindVAO();
    VBO PlaneVBO;
    PlaneVBO.BindVBO();
    PlaneVBO.BufferData(sizeof(planeVertices), &planeVertices[0]);
    planeVAO.SetVertexAttribPointer(1, 3, 8 * sizeof(float), (void*)0);
    planeVAO.SetVertexAttribPointer(2, 3, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    planeVAO.SetVertexAttribPointer(2, 2, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    planeVAO.UnBindVAO();

    Texture2D depthMap;
    int width = 1024, height = 1024;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    FBO depthMapFBO;
    depthMapFBO.BindFBO();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap.ID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    depthMapFBO.UnBindFBO();

    debugDepthQuad.use();
    debugDepthQuad.setInt("depthMap", 0);

    unsigned int woodTexture = loadTexture("E:\\code\\OpenGL\\LearnOpenGL\\Assets\\wood.png");

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        {
            processInput(window);
            glClearColor(0.1, 0.1, 0.1, 1.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        {
            //渲染深度贴图
            glViewport(0, 0, width, height);
            depthMapFBO.BindFBO();
            glClear(GL_DEPTH_BUFFER_BIT);

            glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
            glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 lightSpaceMatrix = lightProjection * lightView;

            simpleDepthshader.use();
            simpleDepthshader.setMat4fv("lightSpaceMatrix", lightSpaceMatrix);
            glActiveTexture(0);
            glBindTexture(GL_TEXTURE_2D, woodTexture);
            depthMap.BindTexture();
            renderScene(planeVAO, simpleDepthshader);
            depthMapFBO.UnBindFBO();
        }


        {//查看阴影
            glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            debugDepthQuad.use();
            debugDepthQuad.setFloat("near_plane", 1.0f);
            debugDepthQuad.setFloat("far_plane", 7.5f);
            glActiveTexture(GL_TEXTURE0);
            depthMap.BindTexture();
            renderQuad();
        }
       

        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);

        {
            glfwPollEvents();
            glfwSwapBuffers(window);
        }
    }
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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
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