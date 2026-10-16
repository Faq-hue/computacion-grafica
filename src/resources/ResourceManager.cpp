#include "ResourceManager.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

ResourceManager::ResourceManager(const std::string& assets_path)
    : assets_path_(assets_path) {
}

ShaderSource ResourceManager::load_shader_source(
    const std::string& name,
    const std::string& vertex_path,
    const std::string& fragment_path
) const {
    (void)name; // El nombre queda disponible para identificar el shader.

    

    auto read_file = [](const std::string& path) -> std::string {
        std::ifstream file(path);

        if (!file.is_open()) {
            throw std::runtime_error(
                "No se pudo abrir el archivo: " + path
            );
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        
        return buffer.str();
    };

    const std::string vertex_full_path =
        assets_path_ + "/" + vertex_path;

    const std::string fragment_full_path =
        assets_path_ + "/" + fragment_path;

    ShaderSource source;
    source.vs = read_file(vertex_full_path);
    source.fs = read_file(fragment_full_path);

    return source;
}