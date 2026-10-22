
#pragma once

#include "../MeshData.h"

class Mesh
{
public:
    Mesh() = default;
    ~Mesh();

    // La malla administra recursos de OpenGL:
    // no se permite copiarla.
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // Sí se permite transferir sus recursos.
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    // Carga los datos de la malla en la GPU.
    void load(const MeshData& data);

    // Dibuja la malla.
    void draw() const;

    // Libera los recursos de OpenGL.
    void clear();

    unsigned int vao() const { return vao_; }
    int count() const { return count_; }

private:
    unsigned int vao_ = 0U;
    unsigned int vbo_ = 0U;
    unsigned int ebo_ = 0U;
    int count_ = 0;
};
