#version 330 core
layout (location = 0) in vec3 aPos;


out vec2 TexCoords;

void main()
{
    TexCoords = (aPos.xy + vec2(0.5,0.5)) ;
    gl_Position = vec4(aPos - vec3(-0.5,0.5,0.0), 1.0);
}