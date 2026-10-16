
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <stdexcept>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Mesh.h"
#include "Primitives.h"
#include "ResourceManager.h"
#include "Shader.h"

static constexpr int kWindowWidth = 800;
static constexpr int kWindowHeight = 600;

static int glfw_error_code{};
static std::string glfw_error_str{};

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

int main()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cerr << "Error de GLFW: " << glfw_error_code
                  << " - " << glfw_error_str << '\n';
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        kWindowWidth, kWindowHeight,
        "Practico 2 - Cubo indexado", nullptr, nullptr);

    if (window == nullptr) {
        std::cerr << "No se pudo crear la ventana.\n";
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

    glfwSetFramebufferSizeCallback(
        window, framebuffer_size_callback);

    glViewport(0, 0, kWindowWidth, kWindowHeight);
    glfwSwapInterval(1);

    // Activar el test de profundidad una sola vez.
    glEnable(GL_DEPTH_TEST);

    int result = EXIT_SUCCESS;

    // Los recursos OpenGL deben destruirse antes de cerrar GLFW.
    {
        try {
            ResourceManager resources("assets");

            const ShaderSource& source =
                resources.load_shader_source(
                    "solid",
                    "shaders/solid.vs",
                    "shaders/solid.fs");

            Shader shader;

            if (!shader.compile_from_source(
                    source.vs, source.fs)) {
                throw std::runtime_error(
                    "No se pudo crear el programa de shaders.");
            }

            MeshData cube_data = primitives::cube();

            std::cout << "Vertices: "
                      << cube_data.vertices.size() << '\n';
            std::cout << "Indices: "
                      << cube_data.indices.size() << '\n';

            Mesh cube;
            cube.load(cube_data);

            if (cube.vao() == 0U || cube.count() == 0) {
                throw std::runtime_error(
                    "No se pudo cargar la malla del cubo.");
            }

            while (!glfwWindowShouldClose(window)) {
                processInput(window);

                glClearColor(
                    51.0f / 256.0f,
                    55.0f / 256.0f,
                    76.0f / 256.0f,
                    1.0f);

                // Limpiar color Y profundidad.
                glClear(
                    GL_COLOR_BUFFER_BIT |
                    GL_DEPTH_BUFFER_BIT);

                shader.use();

                glBindVertexArray(cube.vao());

                glDrawElements(
                    GL_TRIANGLES,
                    cube.count(),
                    GL_UNSIGNED_INT,
                    nullptr);

                glBindVertexArray(0);

                glfwSwapBuffers(window);
                glfwPollEvents();
            }
        }
        catch (const std::exception& error) {
            std::cerr << "Error: " << error.what() << '\n';
            result = EXIT_FAILURE;
        }
    }

    // Al llegar aqui, Mesh y Shader ya se destruyeron.
    glfwDestroyWindow(window);
    glfwTerminate();

    return result;
}
