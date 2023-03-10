#version 330 core
 
uniform samplerCube skybox;
uniform sampler2D shadow_map;
uniform mat4 lightSpaceMatrix;
uniform vec3 eye;
void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
}