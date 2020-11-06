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
    Shader shader("res/shaders/3D.shader");
    shader.Bind();

    // Matrices
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 900.0f / 640.0f, 0.1f, 100.0f);

    glm::mat4 MVP = projection * view * model;
    glm::mat4 MV = view * model;
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MV));
    
    unsigned int loc_MVP = glGetUniformLocation(shader.getID(), "u_MVP");
    glUniformMatrix4fv(loc_MVP, 1, GL_FALSE, &MVP[0][0]);

    unsigned int loc_MV = glGetUniformLocation(shader.getID(), "u_MV");
    glUniformMatrix4fv(loc_MV, 1, GL_FALSE, &MV[0][0]);

    unsigned int loc_NM = glGetUniformLocation(shader.getID(), "u_NormalMatrix");
    glUniformMatrix4fv(loc_NM, 1, GL_FALSE, &NormalMatrix[0][0]);

    glEnable(GL_DEPTH_TEST);

    unsigned int nbSpheres = 32;
    std::vector<glm::vec3> randomAxisRotation;
    std::vector<glm::vec3> randomAxisTranslation;
    for (size_t i = 0; i < nbSpheres; i++)
    {
        randomAxisRotation.push_back(glm::sphericalRand(1.0f));
        randomAxisTranslation.push_back(glm::sphericalRand(2.0f));
    }
    
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.1, 0.15f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Planet
        shader.Bind();
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        glm::mat4 MVP = projection * view * model;
        glm::mat4 MV = view * model;
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MV));
        glUniformMatrix4fv(loc_MVP, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(loc_MV, 1, GL_FALSE, &MV[0][0]);
        glUniformMatrix4fv(loc_NM, 1, GL_FALSE, &NormalMatrix[0][0]);
        glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glBindVertexArray(0);

        // Moon
        shader.Bind();
        for (size_t i = 0; i < nbSpheres; i++)
        {
            model = glm::rotate(glm::mat4(1.0f), glm::radians((float)glfwGetTime() * 20), randomAxisRotation[i]);
            model = glm::translate(model, randomAxisTranslation[i]);
            model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
            MVP = projection * view * model;
            MV = view * model;
            NormalMatrix = glm::transpose(glm::inverse(MV));
            glUniformMatrix4fv(loc_MVP, 1, GL_FALSE, &MVP[0][0]);
            glUniformMatrix4fv(loc_MV, 1, GL_FALSE, &MV[0][0]);
            glUniformMatrix4fv(loc_NM, 1, GL_FALSE, &NormalMatrix[0][0]);
        
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