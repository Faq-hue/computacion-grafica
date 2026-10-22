
#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    Shader() = default;
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    bool compile_from_source(
        const std::string& vs,
        const std::string& fs);

    void use() const;
    void clear();

    unsigned int id() const { return id_; }

    // Obtiene la ubicación de un uniform por su nombre.
    int loc(const std::string& nombre) const;

    // Envía una matriz 4x4 a un uniform.
    void set_uniform(
        const std::string& nombre,
        const glm::mat4& m) const;

    // Envía un vector de tres componentes a un uniform.
    void set_uniform(
        const std::string& nombre,
        const glm::vec3& v) const;

    // Envía una matriz 4x4 usando una ubicación ya obtenida.
    void set_uniform(
        int ubicacion,
        const glm::mat4& m) const;

private:
    unsigned int id_{0U};
};
