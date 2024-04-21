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
#include <ModelImporter.h>
float lastX = 400, lastY = 300;   
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
bool  firstMouse = false;
float yaw = -90.0f;
float pitch = 0.0f;
glm::vec3 lightPos(0.0f, 2.0f, 1.5f);
bool rotate = false;


std::vector<Vertex> planeVertex = {
    Vertex(glm::vec3(0.5f, 0.0f,  0.5f), glm::vec3(0.5f, 0.0f,  0.5f), glm::vec2(1.0f, 1.0f)),
    Vertex(glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
    Vertex(glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    Vertex(glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f))
};


std::vector<unsigned int> planeIndices = {
    0, 1, 2,
    2, 3, 0,
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
    //lightShaderProgram.SetVec3("material.specular", 1.0f, 1.0f, 1.0f);
    //lightShaderProgram.SetFloat("material.shininess", 64.0f);
    lightShaderProgram.SetVec3("lights[0].ambient", 0.2f, 0.2f, 0.2f);
    lightShaderProgram.SetVec3("lights[0].diffuse", 1.0f, 1.0f, 1.0f);
    lightShaderProgram.SetVec3("lights[0].specular", 1.0f, 1.0f, 1.0f);
    lightShaderProgram.SetFloat("lights[0].constant", 1.0f);
    lightShaderProgram.SetFloat("lights[0].linear", 0.09f);
    lightShaderProgram.SetFloat("lights[0].quadratic", 0.032f);
    lightShaderProgram.SetVec3("lights[0].direction", 0.0f, -1.0f, 0.0f);
    lightShaderProgram.SetFloat("lights[0].cutOff", glm::cos(glm::radians(18.5f)));
    lightShaderProgram.SetFloat("lights[0].outerCutOff", glm::cos(glm::radians(31.0f)));
    lightShaderProgram.SetVec3("lights[0].type", 1.0f, 0.0f, 0.0f);

    ModelImporter modelImporter;
    Object* importedModel = modelImporter.LoadModel("C:/Users/emont/OneDrive/Escritorio/Xwing/X-wing.obj", &lightShaderProgram);
    //Mesh* mesh = ProcessMesh(aiMesh, scene);
    //Material* material = ProcessMaterial(aiMesh, scene, path);
    //Renderer* renderer = new Renderer(mesh, material);

    unsigned int diffuseMap = textureSpec;
    bool usesDiffuseMap = false;
    unsigned int specularMap = texture;
    bool usesSpecularMap = false;
    glm::vec3 specularValue(0.5f, 0.5f, 0.5f);
    glm::vec3 ambientValue(0.1f, 0.1f, 0.1f);
    glm::vec3 diffuseValue(0.9f, 0.1f, 0.1f);
    float shininessValue = 0.03f;

    Mesh* mesh = new Mesh(planeVertex, planeIndices);
    Material* material = new Material(diffuseMap, usesDiffuseMap, specularMap, usesDiffuseMap, specularValue, ambientValue, diffuseValue, shininessValue);
    Renderer* renderer = new Renderer(mesh, material);
    material->shader = &lightShaderProgram;
    Object* plane = new Object();
    plane->renderer = renderer;
   // // GENERATE SCENE 

    Object scene(0);
    //scene.AddChild(importedModel);
    scene.AddChild(plane);
    
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


    //lightSourceObj.GetTransform()->SetLocalPosition(lightPos);
    //lightSourceObj.GetTransform()->Scale(glm::vec3(0.1f, 0.1f, 0.1f));

    //cube.GetTransform()->SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        previousTime = time;
        time = (float)glfwGetTime();
        deltaTime = time - previousTime;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        processInput(window);
        if (rotate)
        {

            scene.GetTransform()->Rotate(glm::radians(45.0f * deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        //lightPos = glm::vec3(glm::cos(time ) * 0.0f, 1.5f, glm::sin(time ) * 0.0f);
        lightShaderProgram.Use();
        //lightShaderProgram.SetVec3("lights[0].position", lightPos.x, lightPos.y, lightPos.z);
        scene.Draw(&camera, glm::mat4(1.0f));
        scene.GetTransform()->SetLocalPosition(lightPos);
        //lightSourceObj.GetTransform()->SetLocalPosition(lightPos);
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
