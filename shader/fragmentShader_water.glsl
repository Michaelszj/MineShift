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
uniform float time;







uniform Light light;

void main()
{
    vec3 eyeDir = normalize(eye-Position);
    vec3 newNormal = normalize(vec3(sin(time + Position.z * 3 + Position.x)/6,Normal.y,sin(time + Position.z * 3 + Position.x)/8));
    vec3 refl = 2 * dot(eyeDir, newNormal) * newNormal - eyeDir;
    FragColor = vec4(texture(skybox, refl).rgb * vec3(0.5,0.5,0.9),0.6);
    //FragColor = vec4(vec3(0.5,0.5,0.9) * ((sin(time + Position.x * 5 + Position.z * 4) + 1)) / 2,0.6);
}