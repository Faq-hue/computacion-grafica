#pragma once

#include <string>

struct ShaderSource {
    std::string vs;
    std::string fs;
};

class ResourceManager {
public:
    explicit ResourceManager(const std::string& assets_path);

    ShaderSource load_shader_source(
        const std::string& name,
        const std::string& vertex_path,
        const std::string& fragment_path
    ) const;

private:
    std::string assets_path_;
};