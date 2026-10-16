
#include "Shader.h"

#include <iostream>
#include <string>
#include <utility>

#include <glad/gl.h>

namespace {

GLuint compile_stage(GLenum type, const std::string& source)
{
    GLuint shader = glCreateShader(type);

    const char* source_ptr = source.c_str();
    glShaderSource(shader, 1, &source_ptr, nullptr);
    glCompileShader(shader);

    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success != GL_TRUE) {
        GLint length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        std::string log(static_cast<std::size_t>(length), '\0');
        glGetShaderInfoLog(shader, length, nullptr, log.data());

        std::cerr << "Error al compilar "
                  << (type == GL_VERTEX_SHADER
                      ? "vertex shader" : "fragment shader")
                  << ":\n" << log << '\n';

        glDeleteShader(shader);
        return 0U;
    }

    return shader;
}

} // namespace

Shader::~Shader()
{
    clear();
}

Shader::Shader(Shader&& other) noexcept
    : id_(std::exchange(other.id_, 0U))
{
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other) {
        clear();
        id_ = std::exchange(other.id_, 0U);
    }

    return *this;
}

bool Shader::compile_from_source(
    const std::string& vs,
    const std::string& fs)
{
    clear();

    GLuint vertex = compile_stage(GL_VERTEX_SHADER, vs);

    if (vertex == 0U) {
        return false;
    }

    GLuint fragment = compile_stage(GL_FRAGMENT_SHADER, fs);

    if (fragment == 0U) {
        glDeleteShader(vertex);
        return false;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    GLint success = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success != GL_TRUE) {
        GLint length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

        std::string log(static_cast<std::size_t>(length), '\0');
        glGetProgramInfoLog(program, length, nullptr, log.data());

        std::cerr << "Error al enlazar el programa:\n"
                  << log << '\n';

        glDeleteProgram(program);
        program = 0U;
    }

    // Los shaders individuales ya no son necesarios
    // despues de intentar enlazar el programa.
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    id_ = program;
    return id_ != 0U;
}

void Shader::use() const
{
    glUseProgram(id_);
}

void Shader::clear()
{
    glDeleteProgram(id_);
    id_ = 0U;
}
