#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 model;
layout (location = 7) in mat4 rotation;
 
uniform mat4 lightSpaceMatrix;

 
void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(position, 1.0f);
}