#ifndef OPENGL_AH_GRAPHICS_H
#define OPENGL_AH_GRAPHICS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void ahLoadGlFunctionPointers();

GLuint ahCreateGlProgram(const char *, const char *);

void ahDefineVertexAttribute(GLuint);

void ahDrawTriangles();


#endif //OPENGL_AH_GRAPHICS_H

