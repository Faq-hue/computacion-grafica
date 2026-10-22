
#pragma once

#include <glm/glm.hpp>
#include <vector>

// Datos que describen un vértice de la malla.
struct Vertex {
    glm::vec3 position;   // Posición en el espacio local
    glm::vec3 normal;     // Perpendicular a la superficie
    glm::vec2 tex_coords; // Coordenadas de textura (u, v)
};

// Datos necesarios para construir una malla.
struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};
