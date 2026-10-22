
#include "Mesh.h"

#include <cstddef>
#include <iostream>
#include <limits>
#include <utility>

#include <glad/gl.h>

Mesh::~Mesh()
{
    clear();
}

Mesh::Mesh(Mesh&& other) noexcept
    : vao_(std::exchange(other.vao_, 0U)),
      vbo_(std::exchange(other.vbo_, 0U)),
      ebo_(std::exchange(other.ebo_, 0U)),
      count_(std::exchange(other.count_, 0))
{
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other) {
        clear();

        vao_ = std::exchange(other.vao_, 0U);
        vbo_ = std::exchange(other.vbo_, 0U);
        ebo_ = std::exchange(other.ebo_, 0U);
        count_ = std::exchange(other.count_, 0);
    }

    return *this;
}

void Mesh::load(const MeshData& data)
{
    clear();

    if (data.vertices.empty() || data.indices.empty()) {
        std::cerr << "Mesh::load: la malla esta vacia.\n";
        return;
    }

    if (data.indices.size() >
        static_cast<std::size_t>(
            std::numeric_limits<int>::max())) {
        std::cerr << "Mesh::load: demasiados indices.\n";
        return;
    }

    // Verificar que los indices apunten a vertices existentes.
    for (unsigned int index : data.indices) {
        if (index >= data.vertices.size()) {
            std::cerr << "Mesh::load: indice fuera de rango.\n";
            return;
        }
    }

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    // Subir los vertices a la GPU.
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(
            data.vertices.size() * sizeof(Vertex)),
        data.vertices.data(),
        GL_STATIC_DRAW);

    // Subir los indices a la GPU.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(
            data.indices.size() * sizeof(unsigned int)),
        data.indices.data(),
        GL_STATIC_DRAW);

    
    // Distancia entre el comienzo de dos vertices consecutivos.
    const GLsizei stride = static_cast<GLsizei>(sizeof(Vertex));

    // Atributo 0: posicion (x, y, z).
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        stride,
        reinterpret_cast<void*>(
            offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);

    // Atributo 1: normal (x, y, z).
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        stride,
        reinterpret_cast<void*>(
            offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);

    // Atributo 2: coordenadas de textura (u, v).
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE,
        stride,
        reinterpret_cast<void*>(
            offsetof(Vertex, tex_coords)));
    glEnableVertexAttribArray(2);

    // Desasociar el VAO. El EBO queda registrado en el VAO.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    count_ = static_cast<int>(data.indices.size());
}

void Mesh::clear()
{
    if (ebo_ != 0U) {
        glDeleteBuffers(1, &ebo_);
        ebo_ = 0U;
    }

    if (vbo_ != 0U) {
        glDeleteBuffers(1, &vbo_);
        vbo_ = 0U;
    }

    if (vao_ != 0U) {
        glDeleteVertexArrays(1, &vao_);
        vao_ = 0U;
    }

    count_ = 0;
}
