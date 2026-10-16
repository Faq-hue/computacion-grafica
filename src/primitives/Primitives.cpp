
#include "Primitives.h"

namespace primitives {

MeshData cube(float scale_x, float scale_y, float scale_z)
{
    MeshData mesh;

    const float x = scale_x * 0.5f;
    const float y = scale_y * 0.5f;
    const float z = scale_z * 0.5f;

    // Cada cara tiene cuatro vertices propios y un color uniforme.
    // El orden es antihorario mirando cada cara desde afuera.

    // Cara frontal: rojo.
    mesh.vertices.insert(mesh.vertices.end(), {
        {-x, -y,  z, 1.0f, 0.0f, 0.0f},
        { x, -y,  z, 1.0f, 0.0f, 0.0f},
        { x,  y,  z, 1.0f, 0.0f, 0.0f},
        {-x,  y,  z, 1.0f, 0.0f, 0.0f}
    });

    // Cara trasera: verde.
    mesh.vertices.insert(mesh.vertices.end(), {
        { x, -y, -z, 0.0f, 1.0f, 0.0f},
        {-x, -y, -z, 0.0f, 1.0f, 0.0f},
        {-x,  y, -z, 0.0f, 1.0f, 0.0f},
        { x,  y, -z, 0.0f, 1.0f, 0.0f}
    });

    // Cara derecha: azul.
    mesh.vertices.insert(mesh.vertices.end(), {
        { x, -y,  z, 0.0f, 0.0f, 1.0f},
        { x, -y, -z, 0.0f, 0.0f, 1.0f},
        { x,  y, -z, 0.0f, 0.0f, 1.0f},
        { x,  y,  z, 0.0f, 0.0f, 1.0f}
    });

    // Cara izquierda: amarillo.
    mesh.vertices.insert(mesh.vertices.end(), {
        {-x, -y, -z, 1.0f, 1.0f, 0.0f},
        {-x, -y,  z, 1.0f, 1.0f, 0.0f},
        {-x,  y,  z, 1.0f, 1.0f, 0.0f},
        {-x,  y, -z, 1.0f, 1.0f, 0.0f}
    });

    // Cara superior: magenta.
    mesh.vertices.insert(mesh.vertices.end(), {
        {-x,  y,  z, 1.0f, 0.0f, 1.0f},
        { x,  y,  z, 1.0f, 0.0f, 1.0f},
        { x,  y, -z, 1.0f, 0.0f, 1.0f},
        {-x,  y, -z, 1.0f, 0.0f, 1.0f}
    });

    // Cara inferior: cian.
    mesh.vertices.insert(mesh.vertices.end(), {
        {-x, -y, -z, 0.0f, 1.0f, 1.0f},
        { x, -y, -z, 0.0f, 1.0f, 1.0f},
        { x, -y,  z, 0.0f, 1.0f, 1.0f},
        {-x, -y,  z, 0.0f, 1.0f, 1.0f}
    });

    // Cada cara genera dos triangulos.
    for (unsigned int face = 0; face < 6; ++face) {
        const unsigned int base = face * 4;

        mesh.indices.insert(mesh.indices.end(), {
            base, base + 1, base + 2,
            base + 2, base + 3, base
        });
    }

    return mesh;
}

} // namespace primitives
