#include<imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
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
glm::vec3 lightPos(0.0f, -2.0f, 1.5f);
bool rotate = false;

float planeVertices[] = {
    // positions            // normals          // colors           // texture coords
     0.5f, 0.0f,  0.5f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, 0.0f, -0.5f,     0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, 0.0f, -0.5f,     0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f, 0.0f,  0.5f,     0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // top left 
};
float cubeVertices[] = {
    // positions          // normals          // colors           // texture coords
      0.5f,  0.5f, -0.5f,   0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,   // top right 0
      0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,   // bottom right 1
     -0.5f, -0.5f, -0.5f,  -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f,   // bottom left 2
     -0.5f,  0.5f, -0.5f,  -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f,    // top left  3
                                                                                   
      0.5f,  0.5f, 0.5f,    0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right 4
      0.5f, -0.5f, 0.5f,    0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right 5
     -0.5f, -0.5f, 0.5f,   -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left 6 
     -0.5f,  0.5f, 0.5f,   -0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // top left 7
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


unsigned int planeIndices[] = {  // note that we start from 0!
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
    if (glfwGetKey(window, GLFW_KEY_ENTER)== GLFW_PRESS)
    {
        rotate = !rotate;
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
unsigned int LoadTexture(char const* path) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    return texture;
}
int main()
{
    InitializeOpenGl();
    int Vwidth = 1200;
    int Vheigth = 800;
    GLFWwindow* window = glfwCreateWindow(Vwidth, Vheigth, "OpenGL Game?", NULL, NULL);
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


   // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetCursorPosCallback(window, mouse_callback);
    /*We generate textures*/ 
    unsigned int texture = LoadTexture("container2jp.jpg");
    unsigned int textureSpec = LoadTexture("container2_specular.jpg");
    glEnable(GL_DEPTH_TEST);
    //GENERATE SHADERS

    //Shader shaderProgram("Vertex.v", "Fragment.fr");
    Shader lightShaderProgram("Vertex.v", "LightFragment.fr");
    
    printf("%s\n", glGetString(GL_VERSION));
    //
    lightShaderProgram.Use(); 
    lightShaderProgram.SetVec3("lights[0].position", lightPos.x,lightPos.y, lightPos.z);
    lightShaderProgram.SetVec3("viewPos", 0.0f, 2.0f, 3.0f); 
    lightShaderProgram.SetVec3("material.specular", 1.0f, 1.0f, 1.0f);
    lightShaderProgram.SetFloat("material.shininess", 64.0f);
    lightShaderProgram.SetVec3("lights[0].ambient", 0.2f, 0.2f, 0.2f);
    lightShaderProgram.SetVec3("lights[0].diffuse", 1.0f, 1.0f, 1.0f); // darken diffuse light a bit
    lightShaderProgram.SetVec3("lights[0].specular", 1.0f, 1.0f, 1.0f);
    lightShaderProgram.SetFloat("lights[0].constant", 1.0f);
    lightShaderProgram.SetFloat("lights[0].linear", 0.09f);
    lightShaderProgram.SetFloat("lights[0].quadratic", 0.032f);
    lightShaderProgram.SetVec3("lights[0].direction", 0.0f, -1.0f, 0.0f);
    lightShaderProgram.SetFloat("lights[0].cutOff", glm::cos(glm::radians(18.5f)));
    lightShaderProgram.SetFloat("lights[0].outerCutOff", glm::cos(glm::radians(31.0f)));
    lightShaderProgram.SetVec3("lights[0].type", 0.0f, 0.0f, 1.0f);

    lightShaderProgram.SetInt("material.diffuseMap", 0);
    lightShaderProgram.SetInt("material.specular", 1);
    //shaderProgram.Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureSpec);
    //GENERATE MESHES
    Mesh cubeMesh(cubeVertices,
        sizeof(cubeVertices)/ sizeof(float),
        cubeIndices,
        sizeof(cubeIndices)/sizeof(unsigned int));
   
    Mesh planeMeshh(planeVertices,
        sizeof(planeVertices) / sizeof(float),
        planeIndices,
        sizeof(planeIndices) / sizeof(unsigned int));
    Mesh ligthSource(cubeVertices,
        sizeof(cubeVertices) / sizeof(float),
        cubeIndices,
        sizeof(cubeIndices) / sizeof(unsigned int));

    //

    //GENERATE RENDERERS

    Renderer MeshRenderer(&planeMeshh, &lightShaderProgram);
    Renderer MeshRenderer2(&planeMeshh, &lightShaderProgram);
    Renderer LMeshRenderer(&cubeMesh, &lightShaderProgram);
    
    //

    //GENERATE ENTITIES

    Object PlaneUP(0, &MeshRenderer);
    PlaneUP.Translate(glm::vec3(0.0f, 0.5f, 0.0f));

    Object PlaneDown(0, &MeshRenderer);
    PlaneDown.Translate(glm::vec3(0.0f, -0.5f, 0.0f));
    PlaneDown.Rotate(glm::radians(180.f), glm::vec3(1.0f, 0.0f, 0.0f));
    PlaneDown.Rotate(glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));

    Object PlaneForward(0, &MeshRenderer);
    PlaneForward.Translate(glm::vec3(0.0f, 0.0f, 0.5f));
    PlaneForward.Rotate(glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f));

    Object PlaneBack(0, &MeshRenderer);
    PlaneBack.Translate(glm::vec3(0.0f, 0.0f, -0.5f));
    PlaneBack.Rotate(glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f));

    Object PlaneRight(0, &MeshRenderer);
    PlaneRight.Translate(glm::vec3(0.5f, 0.0f, 0.0f));
    PlaneRight.Rotate(glm::radians(-90.f), glm::vec3(0.0f, 0.0f, 1.0f));

    Object PlaneLeft(0, &MeshRenderer);
    PlaneLeft.Translate(glm::vec3(-0.5f, 0.0f, 0.0f));
    PlaneLeft.Rotate(glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
    Object flatCube(0, nullptr);
    flatCube.AddChild(&PlaneUP);
    flatCube.AddChild(&PlaneDown);
    flatCube.AddChild(&PlaneForward);
    flatCube.AddChild(&PlaneBack);
    flatCube.AddChild(&PlaneLeft);
    flatCube.AddChild(&PlaneRight);

    Object flatCube2(0, nullptr);
    flatCube2.AddChild(&PlaneUP);
    flatCube2.AddChild(&PlaneDown);
    flatCube2.AddChild(&PlaneForward);
    flatCube2.AddChild(&PlaneBack);
    flatCube2.AddChild(&PlaneLeft);
    flatCube2.AddChild(&PlaneRight);

    Object flatCube3(0, nullptr);
    flatCube3.AddChild(&PlaneUP);
    flatCube3.AddChild(&PlaneDown);
    flatCube3.AddChild(&PlaneForward);
    flatCube3.AddChild(&PlaneBack);
    flatCube3.AddChild(&PlaneLeft);
    flatCube3.AddChild(&PlaneRight);

    Object cube(0, &MeshRenderer);
    Object cube2(0, &MeshRenderer);
    Object lightSourceObj(0, &MeshRenderer);
    Object lightSourcePlace(0, &LMeshRenderer);

    // GENERATE SCENE 

    Object scene(0, nullptr);
    scene.AddChild(&flatCube);
    scene.AddChild(&flatCube2);
    scene.AddChild(&flatCube3);
    scene.AddChild(&lightSourceObj);
    

    flatCube.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    flatCube2.SetPosition(glm::vec3(0.0f, -4.0f, 0.0f));
    flatCube3.SetPosition(glm::vec3(0.0f, -8.0f, 0.0f));
    flatCube.Rotate(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //scene.AddChild(&cube2);
    

    //
    glViewport(0, 0, Vwidth, Vheigth);
    float previousTime = (float)glfwGetTime();
    float time = (float)glfwGetTime();
    float deltaTime = 0.0f;
    float transformDistance = 0.0f;

   
    float originalMoveTime = 1.0f;
    float moveTime = originalMoveTime;
    float lastMoveTime = (float)glfwGetTime();

    
    Camera camera(glm::vec3(0.0f, 2.0f,7.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        45.0f,
        Vwidth,
        Vheigth,
        0.1f,
        100.0f);


    lightSourceObj.SetPosition(lightPos);
    lightSourceObj.Scale(glm::vec3(0.1f, 0.1f, 0.1f));

    cube.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        previousTime = time;
        time = (float)glfwGetTime();
        //lightPos = glm::vec3(
        //    glm::cos(time),
        //    0.0f,
        //    glm::sin(time)
        //);
        //
        //lightShaderProgram.Use();
        //lightShaderProgram.SetVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
        deltaTime = time - previousTime;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        processInput(window);
       // cube.Translate(glm::vec3(0.3f, 0.0f, 0.0f)*deltaTime);
        //cube.Rotate(glm::radians(45.0f *deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
        //cube2.Rotate(glm::radians(45.0f * deltaTime), glm::vec3(1.0f, 0.0f, 0.0f));
        if (rotate)
        {

            scene.Rotate(glm::radians(45.0f * deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        //lightPos = glm::vec3(glm::cos(time ) * 0.0f, 1.5f, glm::sin(time ) * 0.0f);
        lightShaderProgram.Use();
        lightShaderProgram.SetVec3("lights[0].position", lightPos.x, lightPos.y, lightPos.z);
        scene.Draw(&camera, glm::mat4(1.0f));
        lightSourceObj.SetPosition(lightPos);
        //flatCube3.SetPosition(lightPos);
        //lightSourceObj.Draw(&camera, glm::mat4(1.0f));
        ImGui::Begin("Here is my first window");
        ImGui::SliderFloat("X", &lightPos.x, -10.0f, 10.0f);
        ImGui::SliderFloat("Y", &lightPos.y, -10.0f, 10.0f);
        ImGui::SliderFloat("Z", &lightPos.z, -10.0f, 10.0f);
        if (ImGui::TreeNode("Bullets"))
        {
           
            if (ImGui::TreeNode("Tree node"))
            {
                ImGui::BulletText("Another bullet point");
                if (ImGui::TreeNode("Tree node"))
                {
                    ImGui::BulletText("Another bullet point");
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }

            ImGui::Bullet(); ImGui::SmallButton("Button");
            ImGui::TreePop();
        }
        ImGui::Text("Hi dude");
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glDeleteProgram(lightShaderProgram.ID);
    //stbi_image_free(data);
    glfwTerminate();
    return 0;
}
