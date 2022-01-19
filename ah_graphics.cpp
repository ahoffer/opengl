
#include "ah_graphics.h"
unsigned int createVertexShader(const char *);
unsigned int createFragmentShader(const char *);
unsigned int ah_create_program(unsigned int, unsigned int);

// glad: load all OpenGL function pointers
// ---------------------------------------
void ahLoadGlFunctionPointers() {
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(2);
    }
}

void ahDefineVertexAttribute(GLuint idx) {
    int numComponents = 3; // (x,y,z) and (r,g,b)
    std::size_t stride = numComponents * sizeof(float);
    glVertexAttribPointer(idx, numComponents, GL_FLOAT, GL_FALSE, stride, (void *) 0);
    glEnableVertexAttribArray(idx);
}

void ahDrawTriangles() {
    GLsizei numVerticesInOneGeometryPrimitive = 3;  //A triangle is 3 points
    GLint startingIndex = 0;
    glDrawArrays(GL_TRIANGLES, startingIndex, numVerticesInOneGeometryPrimitive);
}


// build and compile vertex shader program
// ------------------------------------
unsigned int createVertexShader(const char *source) {

    unsigned int shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    // check for shader compile errors
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

    }
    return shader;
}

// fragment shader
// ------------------------------------
unsigned int createFragmentShader(const char *source) {
    unsigned int shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    // check for shader compile errors
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

// link shaders
unsigned int ah_create_program(unsigned int vertexShader, unsigned int fragmentShader) {
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    // check for linking errors
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    return program;
}

GLuint ahCreateGlProgram(const char *vsource, const char *fsource) {
    unsigned int vertexShader = createVertexShader(vsource);
    unsigned int fragmentShader = createFragmentShader(fsource);
    unsigned int program = ah_create_program(vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glUseProgram(program);
    return program;
}

