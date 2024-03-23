#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <iostream>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Object.h>
#include <Snake.h>
float playerVertices[] = {
    // positions          // colors           // texture coords
     0.09f,  0.09f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.09f, -0.09f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.09f, -0.09f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.09f,  0.09f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // top left 
};

float wallHorizontalVertices[] = {
    // positions          // colors           // texture coords
    1.0f, -0.8f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -1.0f, -1.0f, 0.0f ,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -1.1f, -0.8f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // top left 
};  

float wallVerticalVertices[] = {
    // positions          // colors           // texture coords
    -0.8f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
    -0.8f,  -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -1.0f, -1.0f, 0.0f ,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -1.1f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // top left 
};

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 2,   // first triangle
    2, 3, 0,   // second triangle
};
float texCoords[] = {
0.0f, 0.0f,  // lower-left corner  
1.0f, 0.0f,  // lower-right corner
0.5f, 1.0f   // top-center corner
};
float movementValueY = 0.0f;
float movementValueX = 0.0f;
void processInput(GLFWwindow* window, Snake* snake)
{
    float movementValue = 0.1f;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        movementValueY = movementValue;
        snake->ChangeDir(EDirection::Forward);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        movementValueY = -movementValue;
        snake->ChangeDir(EDirection::Back);
    }
    else {
        movementValueY = 0.00f;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        movementValueX = movementValue;
        snake->ChangeDir(EDirection::Right);
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        movementValueX = -movementValue;
        snake->ChangeDir(EDirection::Left);
    }
    else {
        movementValueX = 0.00f;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void InitializeOpenGl() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}


int main()
{
    InitializeOpenGl();
    GLFWwindow* window = glfwCreateWindow(800, 800, "Snake", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /*We generate textures*/

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    Shader shaderProgram("Vertex.v", "Fragment.fr");
    shaderProgram.use();
    //glUseProgram(shaderProgram);
    int vsize = sizeof(playerVertices) / sizeof(float);
    int isize = sizeof(indices) / sizeof(unsigned int);
    Object walls[4] = {Object(0, shaderProgram.ID, wallHorizontalVertices, vsize, indices, isize),
                       Object(0, shaderProgram.ID, wallHorizontalVertices, vsize, indices, isize) ,
                       Object(0, shaderProgram.ID, wallVerticalVertices, vsize, indices, isize) ,
                       Object(0, shaderProgram.ID, wallVerticalVertices, vsize, indices, isize) };


    walls[0].Translate(glm::vec3(0.0f, 1.8f, 0.0));
    walls[2].Translate(glm::vec3(1.8f, 0.0f, 0.0));
    Object food(0, shaderProgram.ID, playerVertices, vsize, indices, isize);
    Object SnakePart(0, shaderProgram.ID, playerVertices, vsize, indices, isize);
    Snake snake(&SnakePart, glm::vec3(0.0f, 0.0f, 0.0f));
    
    glViewport(0, 0, 800, 800);
    float previousTime = (float)glfwGetTime();
    float time = (float)glfwGetTime();
    float deltaTime = 0.0f;
    float transformDistance = 0.0f;

   
    float originalMoveTime = 1.0f;
    float moveTime = originalMoveTime;
    float lastMoveTime = (float)glfwGetTime();

    food.SetPosition(glm::vec3(0.2f, 0.2f, 0.00f) * 3.0f);
    while (!glfwWindowShouldClose(window))
    {
        previousTime = time;
        time = (float)glfwGetTime();
        deltaTime = time - previousTime;

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        shaderProgram.use();
        shaderProgram.setVec3("ourColor", 0.2f, 0.02f, 0.02f);
       // shaderProgram.setFloat("offset", 0.3f * greenValue);
        processInput(window, &snake);
        for (int i = 0; i < 4; i++)
        {
            walls[i].Draw();

        }
        shaderProgram.setVec3("ourColor", 0.0f, 0.0f, 0.0f);
        food.Draw();
        shaderProgram.setVec3("ourColor", 0.05f, 0.3f, 0.05f);
        if (lastMoveTime + moveTime < (float)glfwGetTime())
        {
            lastMoveTime = (float)glfwGetTime();
            
            if (glm::distance(food.position, snake.currentPos) < 0.05)
            {
                
                snake.Grow();
                float num1 = 1 - round(((float)rand() / (float)RAND_MAX) * 8.0f) * 0.2f;
                float num2 = 1 - round(((float)rand() / (float)RAND_MAX) * 8.0f) * 0.2f;
                std::cout << num1 << std::endl;
                std::cout << num2 << std::endl;
                food.SetPosition(glm::vec3(num1, num2, 0.0f));
                if (moveTime > 0.05f)
                {
                    moveTime -= 0.05f;
                }
            }
            snake.Advance();
           
        }
        snake.Draw();
        
       
        
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram.ID);
    stbi_image_free(data);
    glfwTerminate();
    return 0;
}
