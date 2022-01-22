#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "ah_graphics.h"
#include "ah_window.h"
#include "perlin.h"

using namespace std;

const char *vertexShaderSource();

const char *fragmentShaderSource();

float *points();

void update_vertexes();

//--------------------------------------
float span = 2.0f; // NDC (-1, 1), span = 1 - (-1) = 2
int divisions = 100;
int floats_per_point = 3;
int num_verts_in_a_row = divisions + 1;
int num_rows = num_verts_in_a_row;
int float_per_row = floats_per_point * num_verts_in_a_row;
int num_bytes_in_a_row = float_per_row * sizeof(float);
int num_floats_in_one_direction = num_verts_in_a_row * num_verts_in_a_row * floats_per_point;
int numn_floats_in_both_directions = 2 * num_floats_in_one_direction;
//------------------------------------

float *positions = points();

int main() {
    GLFWwindow *window = ahCreateWindow(800, 600);
    ahLoadGlFunctionPointers();
    GLuint programID = ahCreateGlProgram(vertexShaderSource(), fragmentShaderSource());
    unsigned int vbo_horiz_verts, vbo_vertical_verts, VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &vbo_horiz_verts);
    glGenBuffers(1, &vbo_vertical_verts);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_horiz_verts);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertical_verts);
    glBufferData(GL_ARRAY_BUFFER, num_floats_in_one_direction * sizeof(float), positions, GL_STATIC_DRAW);
    int numComponents = 3; // (x,y,z) and (r,g,b)
    std::size_t stride = numComponents * sizeof(float);
    glVertexAttribPointer(0, numComponents, GL_FLOAT, GL_FALSE, stride, (void *) 0);
    glEnableVertexAttribArray(0);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -50.0f, 50.0f);;

    auto view = glm::lookAt(
            glm::vec3(0.0f, -1.0f, 2.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
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
        update_vertexes();
        glUniformMatrix4fv(mvpId, 1, GL_FALSE, &mvp[0][0]);
        for (int i = 0; i < num_rows; i++) {
            glDrawArrays(GL_LINE_STRIP, i * num_rows, num_rows);
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

    float *pos = (float *) malloc(num_floats_in_one_direction * sizeof(float));
    float step = span / (float) divisions;
//    cout << fixed;
    for (int row = 0; row < num_verts_in_a_row; row++) {
        float y = step * row - 1;
        for (int col = 0; col < num_verts_in_a_row; col++) {
            // -1 to translate it back to {-1,1} space
            float x = step * col - 1;
            int vertex_idx = row * float_per_row + col * floats_per_point;
//            cout << "vert_idx=" << vertex_idx << endl;

            float z = (float) perlin2d((step * col), (step * row), 3, 2);
            cout << "z=" << z << endl;
            pos[vertex_idx] = x;
            pos[vertex_idx + 1] = y;
            pos[vertex_idx + 2] = z;
        }
    }


//    float *ptr = pos;
//    for (int a = 0; a < num_floats_in_one_direction; a += 3)
//        cout << "a=" << a << " (" << ptr[a] << "," << ptr[a + 1] << "," << ptr[a + 2] << ")" << endl;
    return pos;
}


void update_vertexes() {

    for (int i = num_rows; i >= 0; i--) {
        if (i == 0) {
            for (int j = 0; j < num_verts_in_a_row; j++) {
                float *vert = &positions[j];
                *(vert + 2) = perlin2d(vert[0], 0, 3, 2);
            }
        } else {
            for (int j = 0; j < float_per_row; j++) {
                float *vert_a = &positions[i * floats_per_point + j];
                float *vert_b = &positions[(i - 1) * floats_per_point + j];
                vert_a[2] = vert_b[2];
            }
        }

    }
}
