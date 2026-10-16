
#pragma once

#include <vector>

// Datos de un vertice: posicion y color.
struct Vertex {
    float px, py, pz;
    float r, g, b;
};

// Geometria almacenada del lado de la CPU.
struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};
