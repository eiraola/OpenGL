#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <iostream>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Object.h>
float lastX = 400, lastY = 300;   
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
bool  firstMouse = false;
float yaw = -90.0f;
float pitch = 0.0f;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

float planeVertices[] = {
    // positions          // colors           // texture coords
     0.09f,  0.09f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.09f, -0.09f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.09f, -0.09f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.09f,  0.09f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // top left 
};
float cubeVertices[] = {
    // positions          // colors           // texture coords
      0.1f,  0.1f, -0.1f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right 0
      0.1f, -0.1f, -0.1f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right 1
     -0.1f, -0.1f, -0.1f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left 2
     -0.1f,  0.1f, -0.1f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // top left  3

      0.1f,  0.1f, 0.1f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right 4
      0.1f, -0.1f, 0.1f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right 5
     -0.1f, -0.1f, 0.1f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left 6 
     -0.1f,  0.1f, 0.1f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // top left 7
};
float cubeVertices2[] = {
    // positions          // colors           // texture coords
      0.2f,  0.2f, -0.1f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right 0
      0.2f,  0.0f, -0.1f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right 1
      0.0f,  0.0f, -0.1f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left 2
      0.0f,  0.2f, -0.1f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // top left  3

      0.2f,  0.2f, 0.1f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right 4
      0.2f,  0.0f, 0.1f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right 5
      0.0f,  0.0f, 0.1f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left 6 
      0.0f,  0.2f, 0.1f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // top left 7
};
unsigned int cubeIndices[] = {  // note that we start from 0!
    0, 1, 2,   // first triangle
    2, 3, 0,   // second triangle

    4, 5, 6,   // third triangle
    6, 7, 4,   // forth triangle

    4, 0, 3,   // third triangle
    3, 7, 4,   // forth triangle

    1, 5, 6,   // third triangle
    6, 2, 1,   // forth triangle

    3, 2, 6,   // third triangle
    6, 7, 3,   // forth triangle

    0, 1, 5,   // third triangle
    5, 4, 0   // forth triangle
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
void processInput(GLFWwindow* window)
{
    float movementValue = 0.1f;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

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

//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//    std::cout << xpos << "X pos " << std::endl;
//    if (firstMouse)
//    {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//
//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos;
//    lastX = xpos;
//    lastY = ypos;
//
//    float sensitivity = 0.1f;
//    xoffset *= sensitivity;
//    yoffset *= sensitivity;
//
//    yaw += xoffset;
//    pitch += yoffset;
//
//    if (pitch > 89.0f)
//        pitch = 89.0f;
//    if (pitch < -89.0f)
//        pitch = -89.0f;
//
//    glm::vec3 direction;
//    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//    direction.y = sin(glm::radians(pitch));
//    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//    cameraFront = glm::normalize(direction);
//}
int main()
{
    InitializeOpenGl();
    int Vwidth = 1200;
    int Vheigth = 800;
    GLFWwindow* window = glfwCreateWindow(Vwidth, Vheigth, "Snake", NULL, NULL);
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetCursorPosCallback(window, mouse_callback);
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
    glEnable(GL_DEPTH_TEST);
    //GENERATE SHADERS

    Shader shaderProgram("Vertex.v", "Fragment.fr");
    Shader lightShaderProgram("Vertex.v", "LightFragment.fr");

    //
    lightShaderProgram.SetVec3("objectColor", 1.0f, 0.5f, 0.31f);
    lightShaderProgram.SetVec3("objectColor", 1.0f, 1.0f, 1.0f);
    shaderProgram.Use();
    //GENERATE MESHES
    Mesh cubeMesh(cubeVertices2,
        sizeof(cubeVertices2)/ sizeof(float),
        cubeIndices,
        sizeof(cubeIndices)/sizeof(unsigned int));
    Mesh cube2Mesh(cubeVertices2,
        sizeof(cubeVertices2) / sizeof(float),
        cubeIndices,
        sizeof(cubeIndices) / sizeof(unsigned int)); 
    Mesh ligthSource(cubeVertices2,
        sizeof(cubeVertices2) / sizeof(float),
        cubeIndices,
        sizeof(cubeIndices) / sizeof(unsigned int));

    //

    //GENERATE RENDERERS

    Renderer MeshRenderer(&cubeMesh, &shaderProgram);
    Renderer MeshRenderer2(&cube2Mesh, &shaderProgram);
    
    //

    //GENERATE ENTITIES

    Object food(0, &MeshRenderer);
    Object SnakePart(0, &MeshRenderer);

    Object cube(0, &MeshRenderer);
    Object cube2(0, &MeshRenderer2);
    Object lightSourceObj(0, &MeshRenderer);

    // GENERATE SCENE

    Object scene(0, nullptr);
    scene.AddChild(&cube);
    scene.AddChild(&cube2);
    scene.AddChild(&lightSourceObj);

    //
    glViewport(0, 0, Vwidth, Vheigth);
    float previousTime = (float)glfwGetTime();
    float time = (float)glfwGetTime();
    float deltaTime = 0.0f;
    float transformDistance = 0.0f;

   
    float originalMoveTime = 1.0f;
    float moveTime = originalMoveTime;
    float lastMoveTime = (float)glfwGetTime();

    
    Camera camera(glm::vec3(0.0f, 2.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        45.0f,
        Vwidth,
        Vheigth,
        0.1f,
        100.0f);


    lightSourceObj.SetPosition(lightPos);
    lightSourceObj.Scale(glm::vec3(0.5f, 0.5f, 0.5f));
    while (!glfwWindowShouldClose(window))
    {
        previousTime = time;
        time = (float)glfwGetTime();
        deltaTime = time - previousTime;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        processInput(window);
        cube.Translate(glm::vec3(0.3f, 0.0f, 0.0f)*deltaTime);
        scene.Draw(&camera, glm::mat4(1.0f));
        
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glDeleteProgram(shaderProgram.ID);
    stbi_image_free(data);
    glfwTerminate();
    return 0;
}
