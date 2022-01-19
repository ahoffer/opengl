#ifndef OPENGL_AH_WINDOW_H
#define OPENGL_AH_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

GLFWwindow *ahCreateWindow(int, int);

void ahProcessInput(GLFWwindow *);

#endif //OPENGL_AH_WINDOW_H
