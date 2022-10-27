#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<cstdio>
#include<stdint.h>
#include<iostream>

#include"Shader.h"

void error_callback(int err_code, const char* description)
{
	fprintf(stderr, "description: %s\n", description);
}


//game setup
int moveDir = 0;
bool firePress = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod)
{

    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, true);
        break;
    case GLFW_KEY_LEFT:
        if (action == GLFW_PRESS) moveDir -= 1;
        else if (action == GLFW_RELEASE) moveDir += 1;
        break;
    case GLFW_KEY_RIGHT:
        if (action == GLFW_PRESS) moveDir += 1;
        else if (action == GLFW_RELEASE) moveDir -= 1;
        break;
    case GLFW_KEY_SPACE:
        if (action == GLFW_PRESS) firePress = true;
        break;
    default:
        break;
    }
}

GLFWwindow* create_window()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

       // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(800, 600, "Space Invaders", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    //call_backs
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }
    return window;
}





uint32_t rgb_to_uint32(uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t color = (r << 24) | (g << 16) | (b << 8) | 255;
    return color;
}
enum SpriteType : uint8_t
{
    ALIEN_DEAD = 0,
    ALIEN_TYPE_1 = 1,
    ALIEN_TYPE_2 = 2,
    ALIEN_TYPE_3 = 3
};

class Buffer
{
public:

    size_t width;
    size_t height;
    uint32_t* data;

    Buffer(size_t width, size_t height)
    {
        this->width = width;
        this->height = height;
        this->data = new uint32_t[width * height];
    }
    

    void clearBuffer(uint32_t color)
    {

        for (int i = 0; i < width * height; ++i)
        {
            data[i] = color;
        }

    }
    
    ~Buffer()
    {
        delete this->data;
    }
};

class Sprite
{
public:
    size_t width;
    size_t height;
    uint8_t* data;

    Sprite() = default;

    Sprite(size_t width, size_t height)
    {
        this->height = height;
        this->width = width;
        this->data = nullptr;
    }

    void bufferSpriteDraw(Buffer& buffer, size_t offsetX, size_t offsetY, uint32_t color)
    {
        for (int dy = 0; dy < height; ++dy)
        {
            for (int dx = 0; dx < width; ++dx)
            {
                int sy = offsetY + height - 1 - dy;
                int sx = offsetX + dx;
                //精灵像素在buffer之内，且有颜色，则绘制
                if (sx < buffer.width && sy < buffer.height && data[dx + dy * width] != 0)
                {
                    
                    buffer.data[sx + sy * buffer.width] = color;
                }
            }
        }
    }
};

struct Alien
{
    int x;
    int y;
    size_t type;
};

struct Player
{
    int x;
    int y;
    size_t life;
};

struct Bullet
{
    int x; 
    int y;
    int dir;
};

struct Game
{
#define MAX_BULLETS  120
    size_t width;
    size_t height;

    size_t numAliens;
    Alien* aliens;
    Player player;

    Bullet bullets[MAX_BULLETS];
    size_t numBullets;
};

class SpriteAnimation {
public:
    bool loop;
    int time;
    int timeDuration;
    int numFrames;
    Sprite** frames;

    SpriteAnimation(bool loop, int timeDuration, int numFrames, Sprite **frames)
    {
        this->loop = loop;
        this->time = 0;
        this->timeDuration = timeDuration;
        this->numFrames = numFrames;
        this->frames = frames;
    }

};





int main()
{
    const size_t width = 224, height = 256;
    GLFWwindow* window = create_window();
    Shader shader("E:\\code\\OpenGL\\LearnOpenGL\\SpaceInvaders\\shader.vert", "E:\\code\\OpenGL\\LearnOpenGL\\SpaceInvaders\\shader.frag");
    
    //纹理缓冲
    Buffer buffer(width, height);
    uint32_t color = rgb_to_uint32(0, 128, 0);
    buffer.clearBuffer(color);


    Game game{};
    game.width = width;
    game.height = height;
    game.numAliens = 55;
    game.numBullets == 0;
    game.aliens = new Alien[game.numAliens];
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            game.aliens[i * 11 + j].x = 20 + 16 * j;
            game.aliens[i * 11 + j].y = 120 + 16 * i;
            game.aliens[i * 11 + j].type = i / 2 + 1;
        }
    }

    GLuint VAO, texture;
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, buffer.width, buffer.height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, buffer.data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    shader.use();
    shader.setInt("Buffer", 0);

    //opengl setup
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
   // glfwSwapInterval(1);    //TODO， 与刷新率有关的


    Sprite player(11, 7);
    player.data = new uint8_t[77]{
        0,0,0,0,0,1,0,0,0,0,0, // .....@.....
        0,0,0,0,1,1,1,0,0,0,0, // ....@@@....
        0,0,0,0,1,1,1,0,0,0,0, // ....@@@....
        0,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@.
        1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
        1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
        1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
    };

    Sprite bullet(1, 3);
    bullet.data = new uint8_t[3]{
        1,
        1,
        1,
    };



    Sprite alien_sprites[6];

    alien_sprites[0].width = 8;
    alien_sprites[0].height = 8;
    alien_sprites[0].data = new uint8_t[64]
    {
        0,0,0,1,1,0,0,0, // ...@@...
        0,0,1,1,1,1,0,0, // ..@@@@..
        0,1,1,1,1,1,1,0, // .@@@@@@.
        1,1,0,1,1,0,1,1, // @@.@@.@@
        1,1,1,1,1,1,1,1, // @@@@@@@@
        0,1,0,1,1,0,1,0, // .@.@@.@.
        1,0,0,0,0,0,0,1, // @......@
        0,1,0,0,0,0,1,0  // .@....@.
    };

    alien_sprites[1].width = 8;
    alien_sprites[1].height = 8;
    alien_sprites[1].data = new uint8_t[64]
    {
        0,0,0,1,1,0,0,0, // ...@@...
        0,0,1,1,1,1,0,0, // ..@@@@..
        0,1,1,1,1,1,1,0, // .@@@@@@.
        1,1,0,1,1,0,1,1, // @@.@@.@@
        1,1,1,1,1,1,1,1, // @@@@@@@@
        0,0,1,0,0,1,0,0, // ..@..@..
        0,1,0,1,1,0,1,0, // .@.@@.@.
        1,0,1,0,0,1,0,1  // @.@..@.@
    };

    alien_sprites[2].width = 11;
    alien_sprites[2].height = 8;
    alien_sprites[2].data = new uint8_t[88]
    {
        0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
        0,0,0,1,0,0,0,1,0,0,0, // ...@...@...
        0,0,1,1,1,1,1,1,1,0,0, // ..@@@@@@@..
        0,1,1,0,1,1,1,0,1,1,0, // .@@.@@@.@@.
        1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
        1,0,1,1,1,1,1,1,1,0,1, // @.@@@@@@@.@
        1,0,1,0,0,0,0,0,1,0,1, // @.@.....@.@
        0,0,0,1,1,0,1,1,0,0,0  // ...@@.@@...
    };

    alien_sprites[3].width = 11;
    alien_sprites[3].height = 8;
    alien_sprites[3].data = new uint8_t[88]
    {
        0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
        1,0,0,1,0,0,0,1,0,0,1, // @..@...@..@
        1,0,1,1,1,1,1,1,1,0,1, // @.@@@@@@@.@
        1,1,1,0,1,1,1,0,1,1,1, // @@@.@@@.@@@
        1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
        0,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@.
        0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
        0,1,0,0,0,0,0,0,0,1,0  // .@.......@.
    };

    alien_sprites[4].width = 12;
    alien_sprites[4].height = 8;
    alien_sprites[4].data = new uint8_t[96]
    {
        0,0,0,0,1,1,1,1,0,0,0,0, // ....@@@@....
        0,1,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@@.
        1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
        1,1,1,0,0,1,1,0,0,1,1,1, // @@@..@@..@@@
        1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
        0,0,0,1,1,0,0,1,1,0,0,0, // ...@@..@@...
        0,0,1,1,0,1,1,0,1,1,0,0, // ..@@.@@.@@..
        1,1,0,0,0,0,0,0,0,0,1,1  // @@........@@
    };


    alien_sprites[5].width = 12;
    alien_sprites[5].height = 8;
    alien_sprites[5].data = new uint8_t[96]
    {
        0,0,0,0,1,1,1,1,0,0,0,0, // ....@@@@....
        0,1,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@@.
        1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
        1,1,1,0,0,1,1,0,0,1,1,1, // @@@..@@..@@@
        1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
        0,0,1,1,1,0,0,1,1,1,0,0, // ..@@@..@@@..
        0,1,1,0,0,1,1,0,0,1,1,0, // .@@..@@..@@.
        0,0,1,1,0,0,0,0,1,1,0,0  // ..@@....@@..
    };

    Sprite alien_death_sprite;
    alien_death_sprite.width = 13;
    alien_death_sprite.height = 7;
    alien_death_sprite.data = new uint8_t[91]
    {
        0,1,0,0,1,0,0,0,1,0,0,1,0, // .@..@...@..@.
        0,0,1,0,0,1,0,1,0,0,1,0,0, // ..@..@.@..@..
        0,0,0,1,0,0,0,0,0,1,0,0,0, // ...@.....@...
        1,1,0,0,0,0,0,0,0,0,0,1,1, // @@.........@@
        0,0,0,1,0,0,0,0,0,1,0,0,0, // ...@.....@...
        0,0,1,0,0,1,0,1,0,0,1,0,0, // ..@..@.@..@..
        0,1,0,0,1,0,0,0,1,0,0,1,0  // .@..@...@..@.
    };


    Sprite* alienFrames1[] = {&alien_sprites[0], &alien_sprites[1]};
    Sprite* alienFrames2[] = { &alien_sprites[2], &alien_sprites[3] };
    Sprite* alienFrames3[] = { &alien_sprites[4], &alien_sprites[5] };



    SpriteAnimation* alienAnimaiton1 = new SpriteAnimation(true, 120, 2, alienFrames1);
    SpriteAnimation* alienAnimation2 = new SpriteAnimation(true, 120, 2, alienFrames2);
    SpriteAnimation* alienAnimation3 = new SpriteAnimation(true, 120, 2, alienFrames3);

    SpriteAnimation* alienAnimations[3] = { alienAnimaiton1, alienAnimation2, alienAnimation3 };





    while (!glfwWindowShouldClose(window))
    {
        buffer.clearBuffer(color);
 
           
        //处理子弹
        {
            //发射
            if (firePress && game.numBullets < MAX_BULLETS)
            {

                game.bullets[game.numBullets].x = game.player.x + player.width / 2;
                game.bullets[game.numBullets].y = game.player.y + player.height;
                game.bullets[game.numBullets].dir = 2;
                ++game.numBullets;

                firePress = false;
            }

            //绘制
            for (int i = 0; i < game.numBullets; ++i)
            {
                game.bullets[i].y += game.bullets[i].dir;
                bullet.bufferSpriteDraw(buffer, game.bullets[i].x, game.bullets[i].y, rgb_to_uint32(255, 255, 255));
            }

            //碰撞检测
            //出界
            for (int bi = 0; bi < game.numBullets; ++bi)
            {
                if (game.bullets[bi].y + 3 >= game.height)
                {
                    //将最后一个子弹，与当前位置交换，数量减一，假删除
                    game.bullets[bi] = game.bullets[game.numBullets - 1];
                    --bi;
                    --game.numBullets;


                }
            }
        }
        


        //动画
        {
           
            //每一个精灵都有动画
            for (int i = 0; i < game.numAliens; ++i)
            {
               
                SpriteAnimation *animaiton = alienAnimations[game.aliens[i].type - 1];
                int currentFrame = animaiton->time / animaiton->timeDuration;
                Sprite* sprite = animaiton->frames[currentFrame];
                sprite->bufferSpriteDraw(buffer, game.aliens[i].x, game.aliens[i].y, rgb_to_uint32(128, 0, 0));

                ++animaiton->time;
                if (animaiton->time >= animaiton->timeDuration * animaiton->numFrames)
                {
                    if (animaiton->loop)
                    {
                        animaiton->time = 0;
                    }
                }

            }

            

           
        }



        //player 移动
        {
            int speed = 2;
            game.player.x += speed * moveDir;

            if (game.player.x <= 0)
            {
                game.player.x = 0;
            }
            else if (game.player.x + player.width >= game.width)
            {
                game.player.x = game.width - player.width;
            }

            player.bufferSpriteDraw(buffer, game.player.x, game.player.y, rgb_to_uint32(0, 0, 128));
        }

        glTexSubImage2D(
            GL_TEXTURE_2D, 0, 0, 0,
            buffer.width, buffer.height,
            GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
            buffer.data
        );


        

        glDrawArrays(GL_TRIANGLES, 0, 3);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete game.aliens;
    delete player.data;
    delete bullet.data;
    //todo: 判断

    glfwDestroyWindow(window);
    glfwTerminate();
}

