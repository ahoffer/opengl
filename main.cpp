#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "ah_graphics.h"
#include "ah_window.h"
using namespace std;

const char *vertexShaderSource();

const char *fragmentShaderSource();

float *points();
//--------------------------------------
float span = 2.0f;
int divisions = 50;
int floats_per_point = 3;
int num_verts_in_a_row = divisions + 1;
int num_floats = num_verts_in_a_row * num_verts_in_a_row * floats_per_point;
//------------------------------------

float *positions = points();

int main() {
    GLFWwindow *window = ahCreateWindow(800, 600);
    ahLoadGlFunctionPointers();
    GLuint programID = ahCreateGlProgram(vertexShaderSource(), fragmentShaderSource());
    unsigned int VBO_pos, VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO_pos);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
    glBufferData(GL_ARRAY_BUFFER, num_floats * sizeof(float), positions, GL_STATIC_DRAW);
    int numComponents = 3; // (x,y,z) and (r,g,b)
    std::size_t stride = numComponents * sizeof(float);
    glVertexAttribPointer(0, numComponents, GL_FLOAT, GL_FALSE, stride, (void *) 0);
    glEnableVertexAttribArray(0);

    auto projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

    auto view = glm::lookAt(
            glm::vec3(0.0f, -1.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
    );

    while (!glfwWindowShouldClose(window)) {
        // input
        ahProcessInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        GLuint mvpId = glGetUniformLocation(programID, "mvp");
        auto mvp = projection * view;
        glUniformMatrix4fv(mvpId, 1, GL_FALSE, &mvp[0][0]);

        for (int i = 0; i < num_verts_in_a_row; i++) {
            glDrawArrays(GL_LINE_STRIP, i * num_verts_in_a_row, num_verts_in_a_row);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}

const char *vertexShaderSource() {
    return "#version 330 core\n"
           "layout (location = 0) in vec3 aPos;\n"
           "uniform mat4 mvp;\n"
           "void main()\n"
           "{\n"
           "  gl_Position = mvp * vec4(aPos, 1.0);\n"
           "}\0";
}

const char *fragmentShaderSource() {
    return "#version 330 core\n"
           "void main()\n"
           "{\n"
           " gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
           "}\n\0";
}

float *points() {

    float *pos = (float *) malloc(num_floats * sizeof(float));
    float step = span / (float) divisions;
//    cout << fixed;
    for (int row = 0; row < num_verts_in_a_row; row++) {
        float y = step * row - 1;
        for (int col = 0; col < num_verts_in_a_row; col++) {
            // -1 to translate it back to {-1,1} space
            float x = step * col - 1;
            int vertex_idx = num_verts_in_a_row * row * floats_per_point + col * floats_per_point;
//            cout << "vert_idx=" << vertex_idx << endl;
            float z = (2.0 / (rand() % 16) + 1) - 1;
            z=z/10.0;
//            float z = 0.0f;
            pos[vertex_idx] = x;
            pos[vertex_idx + 1] = y;
            pos[vertex_idx + 2] = z;
        }
    }
    float *ptr = pos;
    for (int a = 0; a < num_floats; a += 3)
        cout << "a=" << a << " (" << ptr[a] << "," << ptr[a + 1] << "," << ptr[a + 2] << ")" << endl;
    return pos;
}

