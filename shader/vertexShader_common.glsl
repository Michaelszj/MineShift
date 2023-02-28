#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 model;
layout (location = 7) in mat4 rotation;


out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

struct Light {
    vec3  Intensity;
    vec3  Direction;   // For spot and directional lights.
    vec3  Position;    // For point and spot lights.
    float CutOff;      // For spot lights.
    float OuterCutOff; // For spot lights.
    int   Type;
};

uniform mat4 view;
uniform mat4 projection;
uniform Light light;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Position = (model * vec4(aPos, 1.0)).xyz;
    Normal = (rotation * vec4(aNormal, 1.0)).xyz;
    TexCoord = aTexCoord;
}