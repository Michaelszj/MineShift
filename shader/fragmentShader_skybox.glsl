#version 330 core

out vec4 FragColor;

in vec3 TexCoords;


uniform samplerCube skybox;


void main()
{   
    // float x=TexCoords.x,y=TexCoords.y,z=TexCoords.z;
    // if(x>0.0&&abs(x)>abs(y)&&abs(x)>abs(z)){
    //     FragColor=texture(skybox3,vec2(1.0-(z/x+1)/2.0,(y/x+1)/2.0));
    // }
    // else if(x<0.0&&abs(x)>abs(y)&&abs(x)>abs(z)){
    //     FragColor=texture(skybox4,vec2((z/-x+1)/2.0,(y/-x+1)/2.0));
    // }
    // else if(z>0.0&&abs(z)>abs(y)&&abs(z)>abs(x)){
    //     FragColor=texture(skybox6,vec2((x/z+1)/2.0,(y/z+1)/2.0));
    // }
    // else if(z<0.0&&abs(z)>abs(y)&&abs(z)>abs(x)){
    //     FragColor=texture(skybox5,vec2(1.0-(x/-z+1)/2.0,(y/-z+1)/2.0));
    // }
    // else if(y>0.0&&abs(y)>abs(z)&&abs(y)>abs(x)){
    //     FragColor=texture(skybox1,vec2(1.0-(x/y+1)/2.0,(z/y+1)/2.0));
    // }
    // else if(y<0.0&&abs(y)>abs(z)&&abs(y)>abs(x)){
    //     FragColor=texture(skybox2,vec2(1.0-(x/-y+1)/2.0,1.0-(z/-y+1)/2.0));
    // }
    FragColor = vec4(texture(skybox, TexCoords).rgb,1.0);
    //FragColor = texture(skybox1, TexCoords.xz);
    //FragColor = vec4(0.0,1.0,0.0,1.0);
    //FragColor = vec4(TexCoords,1.0);
    //FragColor = vec4(texture(skybox, vec3(1,1,1)).rgb,1.0);
}