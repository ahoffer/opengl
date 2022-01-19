#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

GLFWwindow *create_win();

void init_graphics_library_framework();

void load_gl_function_pointers();

unsigned int create_vertex_shader(const char *);

unsigned int create_fragment_shader(const char *);

unsigned int create_program(unsigned int, unsigned int);

const char *vertexShaderSource();

const char *fragmentShaderSource();

void defineVertexAttribute(GLuint);

void ahDrawTriangles();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
char infoLog[512];

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
    init_graphics_library_framework();
    GLFWwindow *window = create_win();
    load_gl_function_pointers();
    unsigned int vertexShader = create_vertex_shader(vertexShaderSource());
    unsigned int fragmentShader = create_fragment_shader(fragmentShaderSource());
    unsigned int program = create_program(vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Create a uniform variable for use in the shaders
    unsigned int modelID = glGetUniformLocation(program, "model");
    // Create GL objects
    unsigned int VBO_pos, VAO, VBO_color;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_pos);
    glGenBuffers(1, &VBO_color);

    // First, bind the Vertex Array object
    glBindVertexArray(VAO);
    // Second, bind the Buffer objects
    glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
    // Third, copy the data to the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    // Fourth, specify the location and format of the vertex data (position and color)
    // Assign each data set a successive index. 0, 1, ...
    int idx_pos = 0;
    int idx_color = 1;
    defineVertexAttribute(idx_pos);
    defineVertexAttribute(idx_color);

    // UNNECESSARY?
    //  Bind buffer with the position data to the vertex attribute target.
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound
    // vertex buffer object so afterwards we can safely unbind
//    glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO_color);


    // UNNECESSARY?
    // Bind the VAO to the context
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
//    glBindVertexArray(VAO);

    // uncomment this call to draw in wireframe polygons.
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 model;
    float angle = 0;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        // render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(program);
//        model = glm::translate(glm::mat4(1.0f), glm::vec3( -0.5, -0.0, 0.0f));
//        model = glm::rotate(glm::mat4(1.0f),0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.25f, -0.25f, 0.0f));
        angle += 1;
        if (angle > 360.0);
        angle -= 360.0;
        model = glm::rotate(model, angle / 360.0f * 2.0f * 3.1415926f, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelID, 1, GL_FALSE, value_ptr(model));
        ahDrawTriangles();

//        model = glm::mat4(1.0f);
//        glUniformMatrix4fv(modelID, 1, GL_FALSE, value_ptr(model));
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//        model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, -0.5, 0.0f));
//        glUniformMatrix4fv(modelID, 1, GL_FALSE, value_ptr(model));
//        glDrawArrays(GL_TRIANGLES, 0, 3);

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

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: initialize and configure
// ------------------------------
void init_graphics_library_framework() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

// glfw window creation
// --------------------
GLFWwindow *create_win() {
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

// glad: load all OpenGL function pointers
// ---------------------------------------
void load_gl_function_pointers() {

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(2);
    }
}

// build and compile vertex shader program
// ------------------------------------
unsigned int create_vertex_shader(const char *vprog) {

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vprog, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

    }
    return vertexShader;
}

// fragment shader
// ------------------------------------
unsigned int create_fragment_shader(const char *fprog) {
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fprog, NULL);
    glCompileShader(fragmentShader);
// check for shader compile errors
    int success;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return fragmentShader;
}

// link shaders
unsigned int create_program(unsigned int vertexShader, unsigned int fragmentShader) {
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    // check for linking errors
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    return program;
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
           "  gl_Position = model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
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

void defineVertexAttribute(GLuint idx) {
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