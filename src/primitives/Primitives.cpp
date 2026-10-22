
#include "Primitives.h"
#include <cmath>
#include <stdexcept>

namespace primitives
{

    MeshData cube(float scale_x, float scale_y, float scale_z)
    {
        MeshData mesh;

        const float x = scale_x * 0.5f;
        const float y = scale_y * 0.5f;
        const float z = scale_z * 0.5f;

        // Cada cara tiene cuatro vertices propios porque sus normales
        // son diferentes de las normales de las caras adyacentes.
        // El orden de los vertices es antihorario mirando desde afuera.

        // Cara frontal: normal +Z.
        mesh.vertices.insert(mesh.vertices.end(), {{{-x, -y, z}, {0, 0, 1}, {0, 0}},
                                                   {{x, -y, z}, {0, 0, 1}, {1, 0}},
                                                   {{x, y, z}, {0, 0, 1}, {1, 1}},
                                                   {{-x, y, z}, {0, 0, 1}, {0, 1}}});

        // Cara trasera: normal -Z.
        mesh.vertices.insert(mesh.vertices.end(), {{{x, -y, -z}, {0, 0, -1}, {0, 0}},
                                                   {{-x, -y, -z}, {0, 0, -1}, {1, 0}},
                                                   {{-x, y, -z}, {0, 0, -1}, {1, 1}},
                                                   {{x, y, -z}, {0, 0, -1}, {0, 1}}});

        // Cara derecha: normal +X.
        mesh.vertices.insert(mesh.vertices.end(), {{{x, -y, z}, {1, 0, 0}, {0, 0}},
                                                   {{x, -y, -z}, {1, 0, 0}, {1, 0}},
                                                   {{x, y, -z}, {1, 0, 0}, {1, 1}},
                                                   {{x, y, z}, {1, 0, 0}, {0, 1}}});

        // Cara izquierda: normal -X.
        mesh.vertices.insert(mesh.vertices.end(), {{{-x, -y, -z}, {-1, 0, 0}, {0, 0}},
                                                   {{-x, -y, z}, {-1, 0, 0}, {1, 0}},
                                                   {{-x, y, z}, {-1, 0, 0}, {1, 1}},
                                                   {{-x, y, -z}, {-1, 0, 0}, {0, 1}}});

        // Cara superior: normal +Y.
        mesh.vertices.insert(mesh.vertices.end(), {{{-x, y, z}, {0, 1, 0}, {0, 0}},
                                                   {{x, y, z}, {0, 1, 0}, {1, 0}},
                                                   {{x, y, -z}, {0, 1, 0}, {1, 1}},
                                                   {{-x, y, -z}, {0, 1, 0}, {0, 1}}});

        // Cara inferior: normal -Y.
        mesh.vertices.insert(mesh.vertices.end(), {{{-x, -y, -z}, {0, -1, 0}, {0, 0}},
                                                   {{x, -y, -z}, {0, -1, 0}, {1, 0}},
                                                   {{x, -y, z}, {0, -1, 0}, {1, 1}},
                                                   {{-x, -y, z}, {0, -1, 0}, {0, 1}}});

        // Dos triangulos por cara, con winding antihorario exterior.
        for (unsigned int face = 0; face < 6; ++face)
        {
            const unsigned int base = face * 4;

            mesh.indices.insert(mesh.indices.end(), {base, base + 1, base + 2,
                                                     base + 2, base + 3, base});
        }

        return mesh;
    }

    MeshData cylinder(float radio, float largo,
                      unsigned gajos, unsigned anillos)
    {
        if (radio <= 0.0f || largo <= 0.0f ||
            gajos < 3 || anillos < 1)
        {
            throw std::invalid_argument(
                "Parametros invalidos para cylinder()");
        }

        MeshData mesh;
        constexpr float pi = 3.14159265358979323846f;

        // Superficie lateral.
        for (unsigned i = 0; i <= anillos; ++i)
        {
            const float t = static_cast<float>(i) / anillos;
            const float y = -largo * 0.5f + t * largo;

            for (unsigned j = 0; j <= gajos; ++j)
            {
                const float u = static_cast<float>(j) / gajos;
                const float theta = 2.0f * pi * u;

                const float c = std::cos(theta);
                const float s = std::sin(theta);

                mesh.vertices.push_back({{radio * c, y, radio * s},
                                         {c, 0.0f, s},
                                         {u, t}});
            }
        }

        // Triangulos de la superficie lateral.
        const unsigned stride = gajos + 1;

        for (unsigned i = 0; i < anillos; ++i)
        {
            for (unsigned j = 0; j < gajos; ++j)
            {
                const unsigned a = i * stride + j;
                const unsigned b = a + stride;

                mesh.indices.insert(mesh.indices.end(), {a, b, b + 1,
                                                         a, b + 1, a + 1});
            }
        }

        // Bases superior e inferior, con vertices propios
        // para mantener las normales perpendiculares a cada base.
        for (int tapa = 0; tapa < 2; ++tapa)
        {
            const bool superior = (tapa == 1);
            const float y = superior ? largo * 0.5f
                                     : -largo * 0.5f;
            const float ny = superior ? 1.0f : -1.0f;

            const unsigned centro =
                static_cast<unsigned>(mesh.vertices.size());

            mesh.vertices.push_back({{0.0f, y, 0.0f},
                                     {0.0f, ny, 0.0f},
                                     {0.5f, 0.5f}});

            const unsigned inicio =
                static_cast<unsigned>(mesh.vertices.size());

            for (unsigned j = 0; j <= gajos; ++j)
            {
                const float u = static_cast<float>(j) / gajos;
                const float theta = 2.0f * pi * u;
                const float c = std::cos(theta);
                const float s = std::sin(theta);

                mesh.vertices.push_back({{radio * c, y, radio * s},
                                         {0.0f, ny, 0.0f},
                                         {0.5f + 0.5f * c, 0.5f + 0.5f * s}});
            }

            for (unsigned j = 0; j < gajos; ++j)
            {
                if (superior)
                {
                    mesh.indices.insert(mesh.indices.end(), {centro, inicio + j + 1, inicio + j});
                }
                else
                {
                    mesh.indices.insert(mesh.indices.end(), {centro, inicio + j, inicio + j + 1});
                }
            }
        }

        return mesh;
    }

    MeshData cone(float radio, float conicidad,
                  unsigned gajos, unsigned anillos)
    {
        constexpr float pi = 3.14159265358979323846f;

        if (radio <= 0.0f ||
            conicidad <= 0.0f || conicidad >= pi * 0.5f ||
            gajos < 3 || anillos < 1)
        {
            throw std::invalid_argument(
                "Parametros invalidos para cone()");
        }

        MeshData mesh;

        // Conicidad: angulo entre el eje y la superficie lateral.
        const float altura = radio / std::tan(conicidad);

        // Superficie lateral: el radio disminuye hasta el vertice.
        const unsigned stride = gajos + 1;

        for (unsigned i = 0; i <= anillos; ++i)
        {
            const float t = static_cast<float>(i) / anillos;
            const float r = radio * (1.0f - t);
            const float y = -altura * 0.5f + t * altura;

            for (unsigned j = 0; j <= gajos; ++j)
            {
                const float u = static_cast<float>(j) / gajos;
                const float theta = 2.0f * pi * u;

                const float c = std::cos(theta);
                const float s = std::sin(theta);

                // Normal perpendicular a la superficie inclinada.
                const glm::vec3 normal = glm::normalize(
                    glm::vec3(
                        std::cos(conicidad) * c,
                        std::sin(conicidad),
                        std::cos(conicidad) * s));

                mesh.vertices.push_back({{r * c, y, r * s},
                                         normal,
                                         {u, t}});
            }
        }

        // Triangulos de la superficie lateral.
        for (unsigned i = 0; i < anillos; ++i)
        {
            for (unsigned j = 0; j < gajos; ++j)
            {
                const unsigned a = i * stride + j;
                const unsigned b = a + stride;

                mesh.indices.insert(mesh.indices.end(), {a, b, b + 1,
                                                         a, b + 1, a + 1});
            }
        }

        // Tapa circular inferior.
        const unsigned centro =
            static_cast<unsigned>(mesh.vertices.size());

        mesh.vertices.push_back({{0.0f, -altura * 0.5f, 0.0f},
                                 {0.0f, -1.0f, 0.0f},
                                 {0.5f, 0.5f}});

        const unsigned inicio =
            static_cast<unsigned>(mesh.vertices.size());

        for (unsigned j = 0; j <= gajos; ++j)
        {
            const float u = static_cast<float>(j) / gajos;
            const float theta = 2.0f * pi * u;
            const float c = std::cos(theta);
            const float s = std::sin(theta);

            mesh.vertices.push_back({{radio * c, -altura * 0.5f, radio * s},
                                     {0.0f, -1.0f, 0.0f},
                                     {0.5f + 0.5f * c, 0.5f + 0.5f * s}});
        }

        for (unsigned j = 0; j < gajos; ++j)
        {
            mesh.indices.insert(mesh.indices.end(), {centro, inicio + j, inicio + j + 1});
        }

        return mesh;
    }

} // namespace primitives
