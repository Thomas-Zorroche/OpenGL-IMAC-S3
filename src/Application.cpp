// ============ TP 6
// == 

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/random.hpp>

#include "Shader.h"

#include "glimac/Sphere.hpp"

struct EarthProgram
{
    Shader shader;

    GLint u_MVP;
    GLint u_MV;
    GLint u_NormalMatrix;
    GLuint texID;
    GLuint texID_Clouds;

    EarthProgram(const char * shaderPath) 
        : shader(shaderPath) 
    {
        u_MVP = glGetUniformLocation(shader.getID(), "u_MVP");
        u_MV = glGetUniformLocation(shader.getID(), "u_MV");
        u_NormalMatrix = glGetUniformLocation(shader.getID(), "u_NormalMatrix");
    }
};

struct MoonProgram
{
    Shader shader;

    GLint u_MVP;
    GLint u_MV;
    GLint u_NormalMatrix;
    GLuint texID;

    MoonProgram(const char* shaderPath)
        : shader(shaderPath)
    {
        u_MVP = glGetUniformLocation(shader.getID(), "u_MVP");
        u_MV = glGetUniformLocation(shader.getID(), "u_MV");
        u_NormalMatrix = glGetUniformLocation(shader.getID(), "u_NormalMatrix");
    }
};

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(900, 640, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize glad: load all OpenGL function pointers */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Vertices Input 
    GLfloat vertices[] = {
         // Coordinates
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2
    };
    
    // Shapes
    glimac::Sphere sphere(1, 32, 16);

    GLuint VAO, VBO, IBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
    glGenVertexArrays(1, &VAO);

    // 1. Bind Vertex Array Object
    glBindVertexArray(VAO);
        // 2. (VBO)
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sphere.getVertexCount() * 8 * sizeof(float), sphere.getDataPointer(), GL_STATIC_DRAW);
        // 3. (IBO)
        /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/
        // 3. vertex attributes pointers
        glEnableVertexAttribArray(0); // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
        glEnableVertexAttribArray(1); // Normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2); // TexCoords
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(float)));
        // 4. Unbind VAO (NOT the EBO)
    glBindVertexArray(0);

    // Shaders
    EarthProgram s_Earth("res/shaders/multi3DTex.shader");
    MoonProgram s_Moon("res/shaders/3DTex.shader");

    // Matrices
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 900.0f / 640.0f, 0.1f, 100.0f);

    unsigned int nbSpheres = 32;
    std::vector<glm::vec3> randomAxisRotation;
    std::vector<glm::vec3> randomAxisTranslation;
    for (size_t i = 0; i < nbSpheres; i++)
    {
        randomAxisRotation.push_back(glm::sphericalRand(1.0f));
        randomAxisTranslation.push_back(glm::sphericalRand(2.0f));
    }

    // Textures
    // Earth
    glActiveTexture(0);
    glGenTextures(1, &s_Earth.texID);
    glBindTexture(GL_TEXTURE_2D, s_Earth.texID);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("C:/dev/opengl/OpenGL-IMAC-S3/res/img/EarthMap.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Clouds
    glActiveTexture(1);
    glGenTextures(1, &s_Earth.texID_Clouds);
    glBindTexture(GL_TEXTURE_2D, s_Earth.texID_Clouds);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    data = stbi_load("C:/dev/opengl/OpenGL-IMAC-S3/res/img/CloudMap.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Moon
    glActiveTexture(0);
    glGenTextures(1, &s_Moon.texID);
    glBindTexture(GL_TEXTURE_2D, s_Moon.texID);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    data = stbi_load("C:/dev/opengl/OpenGL-IMAC-S3/res/img/MoonMap.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    s_Earth.shader.Bind(); 
    glUniform1i(glGetUniformLocation(s_Earth.shader.getID(), "Texture"), 0);
    glUniform1i(glGetUniformLocation(s_Earth.shader.getID(), "Clouds"), 1);
    s_Moon.shader.Bind();
    glUniform1i(glGetUniformLocation(s_Moon.shader.getID(), "Texture"), 0);

    glEnable(GL_DEPTH_TEST);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        /* Render here */
        glClearColor(0.1, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Earth
        s_Earth.shader.Bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, s_Earth.texID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, s_Earth.texID_Clouds);
        glm::mat4 modelEarth = glm::rotate(glm::mat4(1.0f), glm::radians((float)glfwGetTime() * 10), glm::vec3(0, 1, 0));
        modelEarth = glm::scale(modelEarth, glm::vec3(0.5, 0.5, 0.5));
        glm::mat4 MVP = projection * view * modelEarth;
        glm::mat4 MV = view * modelEarth;
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MV));
        glUniformMatrix4fv(s_Earth.u_MVP, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(s_Earth.u_MV, 1, GL_FALSE, &MV[0][0]);
        glUniformMatrix4fv(s_Earth.u_NormalMatrix, 1, GL_FALSE, &NormalMatrix[0][0]);
        glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glBindVertexArray(0);

        // Moon
        s_Moon.shader.Bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, s_Moon.texID);
        for (size_t i = 0; i < nbSpheres; i++)
        {
            glm::mat4 modelMoon = glm::rotate(glm::mat4(1.0f), glm::radians((float)glfwGetTime() * 12), randomAxisRotation[i]);
            modelMoon = glm::translate(modelMoon, randomAxisTranslation[i]);
            modelMoon = glm::scale(modelMoon, glm::vec3(0.05, 0.05, 0.05));
            glm::mat4 MVP = projection * view * modelMoon;
            glm::mat4 MV = view * modelMoon;
            glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MV));
            glUniformMatrix4fv(s_Moon.u_MVP, 1, GL_FALSE, &MVP[0][0]);
            glUniformMatrix4fv(s_Moon.u_MV, 1, GL_FALSE, &MV[0][0]);
            glUniformMatrix4fv(s_Moon.u_NormalMatrix, 1, GL_FALSE, &NormalMatrix[0][0]);
        
            glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
            glBindVertexArray(0);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}