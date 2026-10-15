
#include <cstdlib>
#include <iostream>
#include <string>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

static const char* kWindowTitle = "Practico 1 - Triangulo";
static constexpr int kWindowWidth = 800;
static constexpr int kWindowHeight = 600;
static constexpr int kGLVerMajor = 4;
static constexpr int kGLVerMinor = 6;

static int glfw_error_code{};
static std::string glfw_error_str{};

static void error_callback(int error, const char* description);
static void framebuffer_size_callback(GLFWwindow* window,
                                      int width, int height);
static void processInput(GLFWwindow* window);
static void print_gl_version();

static GLuint compile_shader(GLenum type, const char* source);
static GLuint create_shader_program();

int main()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cerr << "Error al inicializar GLFW: "
                  << glfw_error_code << " - "
                  << glfw_error_str << '\n';
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, kGLVerMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, kGLVerMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        kWindowWidth, kWindowHeight,
        kWindowTitle, nullptr, nullptr);

    if (window == nullptr) {
        std::cerr << "No se pudo crear la ventana OpenGL.\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Error al inicializar GLAD.\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    print_gl_version();

    glfwSetFramebufferSizeCallback(
        window, framebuffer_size_callback);

    glViewport(0, 0, kWindowWidth, kWindowHeight);
    glfwSwapInterval(1);

    // 1. Coordenadas de los tres vertices del triangulo.
    // OpenGL utiliza coordenadas normalizadas entre -1 y 1.
    const float vertices[] = {
         0.0f,  0.65f,  // Vertice superior
        -0.65f, -0.55f, // Vertice inferior izquierdo
         0.65f, -0.55f  // Vertice inferior derecho
    };

    // 2. Crear y configurar el VAO y el VBO.
    GLuint vao = 0;
    GLuint vbo = 0;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
                 vertices, GL_STATIC_DRAW);

    // Cada vertice tiene dos componentes: x e y.
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE,
        2 * sizeof(float), nullptr);

    glEnableVertexAttribArray(0);

    // Ya configuramos los objetos; podemos desasociarlos.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 3. Compilar los shaders y crear el programa.
    GLuint shader_program = create_shader_program();

    if (shader_program == 0) {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // 4. Bucle principal.
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // Limpiar la ventana con un fondo azul grisaceo.
        glClearColor(51.0f / 256.0f,
                     55.0f / 256.0f,
                     76.0f / 256.0f,
                     1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Activar el programa de shaders.
        glUseProgram(shader_program);

        // Dibujar los tres vertices como un triangulo.
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Liberar los recursos de OpenGL.
    glDeleteProgram(shader_program);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}

static GLuint compile_shader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success != GL_TRUE) {
        GLint log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

        std::string log(
            static_cast<std::size_t>(log_length), '\0');

        glGetShaderInfoLog(shader, log_length, nullptr,
                           log.data());

        std::cerr << "Error al compilar "
                  << (type == GL_VERTEX_SHADER
                      ? "vertex shader" : "fragment shader")
                  << ":\n" << log << '\n';

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

static GLuint create_shader_program()
{
    // Vertex shader: posiciona cada vertice.
    const char* vertex_source = R"(
        #version 460 core

        layout (location = 0) in vec2 aPosition;

        void main()
        {
            gl_Position = vec4(aPosition, 0.0, 1.0);
        }
    )";

    // Fragment shader: asigna un color uniforme.
    const char* fragment_source = R"(
        #version 460 core

        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(1.0, 0.45, 0.10, 1.0);
        }
    )";

    GLuint vertex_shader =
        compile_shader(GL_VERTEX_SHADER, vertex_source);

    if (vertex_shader == 0) {
        return 0;
    }

    GLuint fragment_shader =
        compile_shader(GL_FRAGMENT_SHADER, fragment_source);

    if (fragment_shader == 0) {
        glDeleteShader(vertex_shader);
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLint success = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success != GL_TRUE) {
        GLint log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

        std::string log(
            static_cast<std::size_t>(log_length), '\0');

        glGetProgramInfoLog(program, log_length, nullptr,
                            log.data());

        std::cerr << "Error al enlazar el programa:\n"
                  << log << '\n';

        glDeleteProgram(program);
        program = 0;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

static void error_callback(int error, const char* description)
{
    glfw_error_code = error;
    glfw_error_str = description;
}

static void framebuffer_size_callback(
    [[maybe_unused]] GLFWwindow* window,
    int width, int height)
{
    glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

static void print_gl_version()
{
    std::cout << "OpenGL Vendor: "
              << glGetString(GL_VENDOR) << '\n';
    std::cout << "OpenGL Renderer: "
              << glGetString(GL_RENDERER) << '\n';
    std::cout << "OpenGL Version: "
              << glGetString(GL_VERSION) << '\n';
    std::cout << "GLSL Version: "
              << glGetString(GL_SHADING_LANGUAGE_VERSION)
              << '\n';
}
