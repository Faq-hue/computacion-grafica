
#pragma once

#include "MeshData.h"

namespace primitives {

MeshData cube(
    float scale_x = 1.0f,
    float scale_y = 1.0f,
    float scale_z = 1.0f
);

MeshData cylinder(
    float radio,
    float largo,
    unsigned gajos,
    unsigned anillos = 1U
);

MeshData cone(
    float radio,
    float conicidad,
    unsigned gajos,
    unsigned anillos = 1U
);

} // namespace primitives
