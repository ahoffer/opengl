#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "ah_graphics.h"
#include "ah_window.h"

const char *vertexShaderSource();

const char *fragmentShaderSource();

void transform(GLuint, float *);


// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
float positions[] = {
        -0.1f, -0.5f, 0.0f, // left
        0.1f, -0.5f, 0.0f, // right
        0.0f, 0.5f, 0.0f // top
};

float colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
};

int main() {
    GLFWwindow *window = ahCreateWindow(800, 600);

    // Must come after initializing GLFW
    ahLoadGlFunctionPointers();

    GLuint program = ahCreateGlProgram(vertexShaderSource(), fragmentShaderSource());
    // Create a uniform variable for use in the shaders
    unsigned int modelID = glGetUniformLocation(program, "model");
    // Create GL object placeholders in the context.
    unsigned int VBO_pos, VAO, VBO_color;
    glGenVertexArrays(1, &VAO);

    // First, bind the object to the vertex array
    glBindVertexArray(VAO);
    // Second, bind the Buffer objects
    glGenBuffers(1, &VBO_pos);
    glGenBuffers(1, &VBO_color);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    // Assign each data set a successive index. 0, 1, ...
    ahDefineVertexAttribute(0);
    ahDefineVertexAttribute(1);

    // uncomment this call to draw in wireframe polygons.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    float angle = 0;
    while (!glfwWindowShouldClose(window)) {
        // input
        ahProcessInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        transform(modelID, &angle);

        ahDrawTriangles();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO_pos);
    glDeleteBuffers(1, &VBO_color);
    glDeleteProgram(program);
    glfwTerminate();
    return 0;
}

void transform(GLuint modelMatId, float *angle) {
   glm::mat4 model = glm::mat4(1.0f);
//        model = glm::translate(model, glm::vec3(-0.25f, -0.25f, 0.0f));
     *angle += 1;
    if (*angle > 360.0);
    *angle -= 360.0;
    model = glm::rotate(model, *angle / 180.0f * 3.1415926f, glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(modelMatId, 1, GL_FALSE, value_ptr(model));
}


const char *vertexShaderSource() {
    return "#version 330 core\n"
           "uniform mat4 model;\n"
           "layout (location = 0) in vec3 aPos;\n"
           "in vec3 vColor;\n"
           "out vec3 vVaryingColor;\n"
           "void main()\n"
           "{\n"
           "  vVaryingColor = vColor;\n"
           "  gl_Position = model * vec4(aPos, 1.0);\n"
           "}\0";
}

const char *fragmentShaderSource() {
    return "#version 330 core\n"
           "in vec3 vVaryingColor;\n"
           "out vec4 FragColor;\n"
           "void main()\n"
           "{\n"
           "  FragColor = vec4(vVaryingColor, 1.0);\n"
           "}\n\0";
}

