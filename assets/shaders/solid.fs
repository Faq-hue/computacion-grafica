
#version 460 core

in vec3 vNormal;
out vec4 FragColor;

uniform vec3 uColor;

void main()
{
    vec3 normal = normalize(vNormal);
    vec3 luz = normalize(vec3(-0.5, 1.0, 0.8));

    float difusa = max(dot(normal, luz), 0.0);
    float iluminacion = 0.25 + 0.75 * difusa;

    FragColor = vec4(uColor * iluminacion, 1.0);
}
