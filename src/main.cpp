
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <stdexcept>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "Primitives.h"
#include "ResourceManager.h"
#include "Shader.h"

static constexpr int kWindowWidth = 800;
static constexpr int kWindowHeight = 600;

static int glfw_error_code{};
static std::string glfw_error_str{};

static void error_callback(int error, const char *description)
{
    glfw_error_code = error;
    glfw_error_str = description;
}

static void framebuffer_size_callback(
    [[maybe_unused]] GLFWwindow *window,
    int width,
    int height)
{
    glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        std::cerr << "Error de GLFW: " << glfw_error_code
                  << " - " << glfw_error_str << '\n';
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(
        kWindowWidth,
        kWindowHeight,
        "Practico 3 - Primitivas parametrica",
        nullptr,
        nullptr);

    if (window == nullptr)
    {
        std::cerr << "No se pudo crear la ventana.\n";
        std::cerr << "Error de GLFW: " << glfw_error_code
                  << " - " << glfw_error_str << '\n';
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Error al inicializar GLAD.\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetFramebufferSizeCallback(
        window, framebuffer_size_callback);

    int framebufferWidth = 0;
    int framebufferHeight = 0;

    glfwGetFramebufferSize(
        window, &framebufferWidth, &framebufferHeight);

    glViewport(0, 0, framebufferWidth, framebufferHeight);
    glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST);

    int result = EXIT_SUCCESS;

    {
        try
        {
            ResourceManager resources("assets");

            const ShaderSource &source =
                resources.load_shader_source(
                    "solid",
                    "shaders/solid.vs",
                    "shaders/solid.fs");

            Shader shader;

            if (!shader.compile_from_source(source.vs, source.fs))
            {
                throw std::runtime_error(
                    "No se pudo crear el programa de shaders.");
            }

            // -------------------------------------------------
            // CREACION DE LAS MALLAS
            // -------------------------------------------------

            MeshData cube_data = primitives::cube();

            MeshData cylinder_data =
                primitives::cylinder(0.3f, 0.8f, 32, 4);

            MeshData cone_data =
                primitives::cone(
                    0.3f,
                    glm::radians(25.0f),
                    32,
                    4);

            Mesh cube;
            cube.load(cube_data);

            Mesh cylinder;
            cylinder.load(cylinder_data);

            Mesh cone;
            cone.load(cone_data);

            if (cube.vao() == 0U || cube.count() == 0 ||
                cylinder.vao() == 0U || cylinder.count() == 0 ||
                cone.vao() == 0U || cone.count() == 0)
            {
                throw std::runtime_error(
                    "No se pudo cargar una de las mallas.");
            }

            std::cout << "Cubo: " << cube_data.vertices.size()
                      << " vertices, " << cube_data.indices.size()
                      << " indices\n";

            std::cout << "Cilindro: "
                      << cylinder_data.vertices.size()
                      << " vertices, " << cylinder_data.indices.size()
                      << " indices\n";

            std::cout << "Cono: " << cone_data.vertices.size()
                      << " vertices, " << cone_data.indices.size()
                      << " indices\n";

            // -------------------------------------------------
            // MATRIZ DE AJUSTE
            // -------------------------------------------------

            const float ancho = 800.0f;
            const float alto = 600.0f;

            const glm::mat4 ajuste =
                glm::scale(
                    glm::mat4(1.0f),
                    glm::vec3(alto / ancho, 1.0f, -1.0f));

            // -------------------------------------------------
            // MATRICES DE MODELO DE LOS CUBOS
            // -------------------------------------------------

            const glm::mat4 modeloIzquierdo =
                glm::translate(
                    glm::mat4(1.0f),
                    glm::vec3(-0.8f, 0.35f, 0.0f)) *
                glm::rotate(
                    glm::mat4(1.0f),
                    glm::radians(30.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate(
                    glm::mat4(1.0f),
                    glm::radians(20.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::scale(
                    glm::mat4(1.0f),
                    glm::vec3(0.25f));

            const glm::mat4 modeloCentral =
                glm::translate(
                    glm::mat4(1.0f),
                    glm::vec3(0.0f, 0.35f, 0.0f)) *
                glm::rotate(
                    glm::mat4(1.0f),
                    glm::radians(30.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate(
                    glm::mat4(1.0f),
                    glm::radians(20.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::scale(
                    glm::mat4(1.0f),
                    glm::vec3(0.30f));

            const glm::mat4 modeloDerecho =
                glm::translate(
                    glm::mat4(1.0f),
                    glm::vec3(0.8f, 0.35f, 0.0f)) *
                glm::rotate(
                    glm::mat4(1.0f),
                    glm::radians(30.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate(
                    glm::mat4(1.0f),
                    glm::radians(20.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::scale(
                    glm::mat4(1.0f),
                    glm::vec3(0.25f));

            // -------------------------------------------------
            // MATRICES DE MODELO DEL CILINDRO Y EL CONO
            // -------------------------------------------------

            const glm::mat4 modeloCilindro =
                glm::translate(
                    glm::mat4(1.0f),
                    glm::vec3(-0.4f, -0.45f, 0.0f)) *
                glm::rotate(
                    glm::mat4(1.0f),
                    glm::radians(20.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::scale(
                    glm::mat4(1.0f),
                    glm::vec3(0.45f));

            const glm::mat4 modeloCono =
                glm::translate(
                    glm::mat4(1.0f),
                    glm::vec3(0.4f, -0.45f, 0.0f)) *
                glm::rotate(
                    glm::mat4(1.0f),
                    glm::radians(20.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::scale(
                    glm::mat4(1.0f),
                    glm::vec3(0.45f));

            // -------------------------------------------------
            // BUCLE PRINCIPAL
            // -------------------------------------------------

            while (!glfwWindowShouldClose(window))
            {
                processInput(window);

                glClearColor(
                    51.0f / 256.0f,
                    55.0f / 256.0f,
                    76.0f / 256.0f,
                    1.0f);

                glClear(
                    GL_COLOR_BUFFER_BIT |
                    GL_DEPTH_BUFFER_BIT);

                shader.use();
                shader.set_uniform("uAjuste", ajuste);

                glEnable(GL_DEPTH_TEST);
                glDisable(GL_CULL_FACE);

                // Cubo izquierdo: rojo.
                glBindVertexArray(cube.vao());

                shader.set_uniform("uModel", modeloIzquierdo);
                shader.set_uniform(
                    "uColor", glm::vec3(1.0f, 0.1f, 0.1f));

                glDrawElements(
                    GL_TRIANGLES,
                    cube.count(),
                    GL_UNSIGNED_INT,
                    nullptr);

                // Cubo central: verde.
                shader.set_uniform("uModel", modeloCentral);
                shader.set_uniform(
                    "uColor", glm::vec3(0.1f, 1.0f, 0.1f));

                glDrawElements(
                    GL_TRIANGLES,
                    cube.count(),
                    GL_UNSIGNED_INT,
                    nullptr);

                // Cubo derecho: azul.
                shader.set_uniform("uModel", modeloDerecho);
                shader.set_uniform(
                    "uColor", glm::vec3(0.1f, 0.3f, 1.0f));

                glDrawElements(
                    GL_TRIANGLES,
                    cube.count(),
                    GL_UNSIGNED_INT,
                    nullptr);

                // Cilindro: amarillo.
                glBindVertexArray(cylinder.vao());

                shader.set_uniform("uModel", modeloCilindro);
                shader.set_uniform(
                    "uColor", glm::vec3(1.0f, 0.8f, 0.1f));

                glDrawElements(
                    GL_TRIANGLES,
                    cylinder.count(),
                    GL_UNSIGNED_INT,
                    nullptr);

                // Cono: cian.
                glBindVertexArray(cone.vao());

                shader.set_uniform("uModel", modeloCono);
                shader.set_uniform(
                    "uColor", glm::vec3(0.1f, 0.8f, 1.0f));

                glDrawElements(
                    GL_TRIANGLES,
                    cone.count(),
                    GL_UNSIGNED_INT,
                    nullptr);

                glBindVertexArray(0);

                glfwSwapBuffers(window);
                glfwPollEvents();
            }
        }
        catch (const std::exception &error)
        {
            std::cerr << "Error: " << error.what() << '\n';
            result = EXIT_FAILURE;
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return result;
}
