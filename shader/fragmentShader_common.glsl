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

uniform sampler2D diffuse_map;
uniform sampler2D specualr_map;
uniform sampler2D normal_map;
uniform sampler2D height_map;
uniform sampler2D shadow_map;
uniform mat4 lightSpaceMatrix;
uniform vec3 eye;






uniform Light light;

void main()
{
    //if(texture(diffuse_map, TexCoord).a < 0.05) discard;
    vec3 dir = normalize(light.Direction);
    //float dis = length(light.Position - Position);
    float attenuation = dot(-dir,Normal);
    if (attenuation < 0.0) attenuation = 0.0;
    
    vec3 diffuse = texture(diffuse_map, TexCoord).rgb * (light.Intensity * attenuation );
    vec3 ambient = texture(diffuse_map, TexCoord).rgb * 0.3;
    vec3 specualr = texture(specualr_map, TexCoord).rgb * pow(dot(Normal, normalize(-dir+ normalize(eye-Position))),80) * light.Intensity * 50.0;
    //vec3 ambient = vec3(0.0,Position.z/100.0,0.0) ;

    vec4 light_pos = lightSpaceMatrix * vec4(Position, 1.0);
    vec3 projCoords = light_pos.xyz / light_pos.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadow_map, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth + 0.0002 ? 1.0 : 0.0;


    FragColor = vec4(ambient + (diffuse + specualr) * (1.0 - shadow),1.0);
    //FragColor = vec4(texture(diffuse_map, TexCoord).rgb,1.0);
    //FragColor = vec4(1.0,1.0,1.0,1.0);
    //FragColor = vec4(TexCoord.xy,0.0,1.0);
}