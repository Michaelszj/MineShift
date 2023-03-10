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

uniform sampler2D texture1;
uniform sampler2D destroy;
uniform sampler2D shadow_map;
uniform mat4 lightSpaceMatrix;
uniform vec3 eye;

uniform bool useD = false;





uniform Light light;

void main()
{
    if(texture(texture1, TexCoord).a < 0.05) discard;
    vec3 dir = normalize(light.Direction);
    //float dis = length(light.Position - Position);
    float attenuation = dot(-dir,Normal);
    if (attenuation < 0.0) attenuation = 0.0;
    vec3 color = texture(texture1, TexCoord).rgb;
    if(useD == true && texture(destroy, TexCoord).a > 0.8){
        color *= texture(destroy, TexCoord).rgb;
    }
    vec3 diffuse = color * (light.Intensity * attenuation );
    vec3 ambient = color * 0.3;

    vec4 light_pos = lightSpaceMatrix * vec4(Position, 1.0);
    vec3 projCoords = light_pos.xyz / light_pos.w;
    projCoords = projCoords * 0.5 + 0.5;

    float currentDepth = projCoords.z;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadow_map, 0);

    int s = 4;
    float eye_dis = length(eye - Position);
    if(eye_dis > 0.3) eye_dis = 0.3;
    s = int(2 / eye_dis);
    if(s == 0) s = 1;
    float stepl = 0.06 / s;
    vec3 tempx = normalize(dFdx(Position));
    vec3 tempy = normalize(dFdy(Position));
    for(int x = -s; x <= s; ++x)
    {
        for(int y = -s; y <= s; ++y)
        {
            //float pcfDepth = texture(shadow_map, projCoords.xy + vec2(x, y) * texelSize).r; 
            //float temp = currentDepth - pcfDepth;
            
            //if(temp > 0.0) shadow += temp / (temp + 0.0001); 
            //shadow += currentDepth - 0.0002 > pcfDepth ? 1.0 : 0.0;        

            vec4 newPos = lightSpaceMatrix * vec4(Position + x * tempx * stepl+ y * tempy * stepl, 1.0);
            vec3 newCoords = newPos.xyz / newPos.w;
            newCoords = newCoords * 0.5 + 0.5;
            float pcfDepth = texture(shadow_map, newCoords.xy).r;
            float temp = newCoords.z - pcfDepth;
            if(temp > 0.0002) shadow += 1.0;

        }    
    }
    shadow /= (2 * s + 1) * (2 * s + 1);


    float closestDepth = texture(shadow_map, projCoords.xy).r;
    
    // float temp = currentDepth - closestDepth;
    // if(temp < 0.0001) shadow *= 0.0;
    // else if (temp < 0.0002) shadow *= (temp - 0.0001) * 10000; 
    //shadow = currentDepth > closestDepth + 0.0002  ? shadow : 0.0;

    
    FragColor = vec4(diffuse * (1.0 - shadow) + ambient,texture(texture1, TexCoord).a);
    //FragColor = vec4(1.0,1.0,1.0,1.0);
}