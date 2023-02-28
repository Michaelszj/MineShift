#version 330 core
out vec4 FragColor;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

struct Light {
    vec3  Intensity;
    vec3  Direction;   // For spot and directional lights.
    vec3  Position;    // For point and spot lights.
    float CutOff;      // For spot lights.
    float OuterCutOff; // For spot lights.
    int   Type;
};

uniform samplerCube skybox;
uniform sampler2D shadow_map;
uniform mat4 lightSpaceMatrix;
uniform vec3 eye;







uniform Light light;

void main()
{
    vec3 eyeDir = normalize(eye-Position);
    vec3 refl = 2 * dot(eyeDir, Normal) * Normal - eyeDir;
    FragColor = vec4(texture(skybox, refl).rgb * vec3(0.5,0.5,0.9),0.6);
}