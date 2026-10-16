
#pragma once

#include "MeshData.h"

class Mesh {
public:
    Mesh() = default;
    ~Mesh();

    // Los recursos OpenGL tienen un unico propietario.
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // Mover transfiere la propiedad de los recursos.
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void load(const MeshData& data);
    void clear();

    unsigned int vao() const { return vao_; }
    int count() const { return count_; }

private:
    unsigned int vao_{0U};
    unsigned int vbo_{0U};
    unsigned int ebo_{0U};
    int count_{0};
};
