#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "display.h"
#include "stb_image.h"
#include "game.h"
#include "model.h"
#include <random>





int main() {
    

    glInit();
    
    //Camera camera(glm::vec3(0.0f, 0.0f, 4.0f));
    //GLFW窗口创建
    GLFWwindow* window = create_window(800, 600, "MineShit", NULL, NULL);
    Scene scene(window);

    //GLAD初始化
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(0);
    }

    //绘制窗口创建
    glViewport(0, 0, 800, 600);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    std::vector<float> cube_v = {
     0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,  
    -0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  

    -0.5f, -0.5f,  0.5f,  
     0.5f, -0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  
    -0.5f, -0.5f,  0.5f,  

    -0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f, 
    -0.5f, -0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  

     0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f,  0.5f,  
     0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f,  0.5f,  
     0.5f, -0.5f, -0.5f,  
     0.5f,  0.5f,  0.5f,  

    -0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f,  0.5f,  
     0.5f, -0.5f,  0.5f,  
    -0.5f, -0.5f,  0.5f,  
    -0.5f, -0.5f, -0.5f,  

     0.5f,  0.5f, -0.5f, 
    -0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f, -0.5f,  
    };

    std::vector<float> cube_n = {
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    std::vector<float> cube_t = {
     1.0f, 0.0f,
     0.0f, 0.0f,
     1.0f, 1.0f,
     0.0f, 1.0f,
     1.0f, 1.0f,
     0.0f, 0.0f,

     0.0f, 0.0f,
     1.0f, 0.0f,
     1.0f, 1.0f,
     1.0f, 1.0f,
     0.0f, 1.0f,
     0.0f, 0.0f,

     0.0f, 1.0f,
     1.0f, 1.0f,
     1.0f, 0.0f,
     1.0f, 0.0f,
     0.0f, 0.0f,
     0.0f, 1.0f,

     1.0f, 1.0f,
     0.0f, 1.0f,
     1.0f, 0.0f,
     0.0f, 0.0f,
     1.0f, 0.0f,
     0.0f, 1.0f,

     0.0f, 1.0f,
     1.0f, 1.0f,
     1.0f, 0.0f,
     1.0f, 0.0f,
     0.0f, 0.0f,
     0.0f, 1.0f,

     1.0f, 1.0f,
     0.0f, 1.0f,
     1.0f, 0.0f,
     0.0f, 0.0f,
     1.0f, 0.0f,
     0.0f, 1.0f
    };

    std::vector<float> flower_v = {
        -0.5f, -0.5f,  0.0f,
         0.0f, -0.5f,  0.0f,
         0.0f,  0.5f,  0.0f,

         0.0f,  0.5f,  0.0f,
        -0.5f,  0.5f,  0.0f,
        -0.5f, -0.5f,  0.0f,

         0.0f, -0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
         0.5f,  0.5f,  0.0f,

         0.5f,  0.5f,  0.0f,
         0.0f,  0.5f,  0.0f,
         0.0f, -0.5f,  0.0f,

         0.0f, -0.5f, -0.5f,
         0.0f, -0.5f,  0.0f,
         0.0f,  0.5f,  0.0f,

         0.0f,  0.5f,  0.0f,
         0.0f,  0.5f, -0.5f,
         0.0f, -0.5f, -0.5f,

         0.0f, -0.5f,  0.0f,
         0.0f, -0.5f,  0.5f,
         0.0f,  0.5f,  0.5f,

         0.0f,  0.5f,  0.5f,
         0.0f,  0.5f,  0.0f,
         0.0f, -0.5f,  0.0f,
    };

    std::vector<float> flower_n{
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
    };

    std::vector<float> flower_t{
        0.0f,0.0f,
        0.5f,0.0f,
        0.5f,1.0f,

        0.5f,1.0f,
        0.0f,1.0f,
        0.0f,0.0f,

        0.5f,0.0f,
        1.0f,0.0f,
        1.0f,1.0f,

        1.0f,1.0f,
        0.5f,1.0f,
        0.5f,0.0f,

        0.0f,0.0f,
        0.5f,0.0f,
        0.5f,1.0f,

        0.5f,1.0f,
        0.0f,1.0f,
        0.0f,0.0f,

        0.5f,0.0f,
        1.0f,0.0f,
        1.0f,1.0f,

        1.0f,1.0f,
        0.5f,1.0f,
        0.5f,0.0f,
    };

    std::vector<float> cube_else_v = {
     0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,

    
    };

    std::vector<float> cube_else_n = {
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

       
    };

    std::vector<float> cube_else_t = {
     1.0f, 0.0f,
     0.0f, 0.0f,
     1.0f, 1.0f,
     0.0f, 1.0f,
     1.0f, 1.0f,
     0.0f, 0.0f,

     0.0f, 0.0f,
     1.0f, 0.0f,
     1.0f, 1.0f,
     1.0f, 1.0f,
     0.0f, 1.0f,
     0.0f, 0.0f,

     0.0f, 1.0f,
     1.0f, 1.0f,
     1.0f, 0.0f,
     1.0f, 0.0f,
     0.0f, 0.0f,
     0.0f, 1.0f,

     1.0f, 1.0f,
     0.0f, 1.0f,
     1.0f, 0.0f,
     0.0f, 0.0f,
     1.0f, 0.0f,
     0.0f, 1.0f,

    
    };

    std::vector<float> cube_top_v = {
    
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    };

    std::vector<float> cube_top_n = {
        

        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    std::vector<float> cube_top_t = {
    
     0.0f, 1.0f,
     1.0f, 1.0f,
     1.0f, 0.0f,
     1.0f, 0.0f,
     0.0f, 0.0f,
     0.0f, 1.0f,

     1.0f, 1.0f,
     0.0f, 1.0f,
     1.0f, 0.0f,
     0.0f, 0.0f,
     1.0f, 0.0f,
     0.0f, 1.0f
    };

    std::vector<float> skyboxVertices = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    std::vector<float> debug_v = {
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f
    };

    unsigned int indices[] = {

        0, 1, 3, // 第一个三角形
        1, 2, 3,
    };

    



    ////缓冲对象创建
    //unsigned int VBO, VAO, EBO;
    ////顶点数组属性
    //glGenVertexArrays(1, &VAO);
    //glBindVertexArray(VAO);
    ////元素缓冲
    //glGenBuffers(1, &EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    ////顶点缓冲
    //glGenBuffers(1, &VBO);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //// 位置属性
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    //// 颜色属性
    ////glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    ////glEnableVertexAttribArray(1);
    ////纹理坐标属性
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    //glBindVertexArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    Shader aShader("shader/vertexShader_common.glsl", "shader/fragmentShader.glsl");
    Shader bShader("shader/vertexShader.glsl", "shader/fragmentShader.glsl");
    Shader cShader("shader/vertexShader_common.glsl", "shader/fragmentShader_common.glsl");
    Shader dShader("shader/vertexShader_skybox.glsl", "shader/fragmentShader_skybox.glsl");
    Shader waterShader("shader/vertexShader_water.glsl", "shader/fragmentShader_water.glsl");
    Shader waterDepthShader("shader/vertexShader_water.glsl", "shader/depthShader_fs.glsl");
    Shader depthShader("shader/depthShader_vs.glsl", "shader/depthShader_fs.glsl");
    Shader debugShader("shader/debugShader_vs.glsl", "shader/debugShader_fs.glsl");
    scene.depthShader = &depthShader;
    aShader.textures.push_back("texture1");
    aShader.textures.push_back("destroy");
    aShader.textures.push_back("shadow_map");
    cShader.textures.push_back("diffuse_map");
    cShader.textures.push_back("specular_map");
    cShader.textures.push_back("shadow_map");

    

    //纹理操作
    Texture_Cube sky_tex("texture/skybox/sky1_");
    Texture stone_texture("texture/blocks/stone.png");
    Texture dirt_texture("texture/blocks/dirt.png");
    Texture flower_texture("texture/blocks/flower_rose.png");
    Texture diamond_texture("texture/blocks/diamond_block.png");
    Texture gold_texture("texture/blocks/gold_block.png");
    Texture glass_texture("texture/blocks/glass.png");
    Texture log_texture("texture/blocks/log_oak.png");
    Texture wood_texture("texture/blocks/log_oak_top.png");
    Texture leaf_texture("texture/blocks/leaves_oak_green.png");
    Texture destroy[10]{
        Texture("texture/blocks/destroy_stage_0.png"),
        Texture("texture/blocks/destroy_stage_1.png"),
        Texture("texture/blocks/destroy_stage_2.png"),
        Texture("texture/blocks/destroy_stage_3.png"),
        Texture("texture/blocks/destroy_stage_4.png"),
        Texture("texture/blocks/destroy_stage_5.png"),
        Texture("texture/blocks/destroy_stage_6.png"),
        Texture("texture/blocks/destroy_stage_7.png"),
        Texture("texture/blocks/destroy_stage_8.png"),
        Texture("texture/blocks/destroy_stage_9.png")
    };
    scene.destroy = destroy;

#pragma region MyRegion



    Model stone(Mesh(cube_v, cube_n, cube_t));
    stone.destroy_time = 0.2f;
    stone.meshes[0].textures.push_back(stone_texture);
    stone.shader = &aShader;
    stone.single_shader = &bShader;
    stone.bind_shader();
    objList.push_back(&stone);

    Model dirt(Mesh(cube_v, cube_n, cube_t));
    dirt.meshes[0].textures.push_back(dirt_texture);
    dirt.shader = &aShader;
    dirt.single_shader = &bShader;
    dirt.bind_shader();
    objList.push_back(&dirt);

    Model flower(Mesh(flower_v, flower_n, flower_t));
    flower.destroy_time = 0.4f;
    flower.meshes[0].textures.push_back(flower_texture);
    flower.shader = &aShader;
    flower.single_shader = &bShader;
    flower.bind_shader();
    objList.push_back(&flower);

    Model diamond(Mesh(cube_v, cube_n, cube_t));
    diamond.destroy_time = 10.0f;
    diamond.meshes[0].textures.push_back(diamond_texture);
    diamond.shader = &aShader;
    diamond.single_shader = &bShader;
    diamond.bind_shader();
    objList.push_back(&diamond);

    Model gold(Mesh(cube_v, cube_n, cube_t));
    gold.destroy_time = 7.0f;
    gold.meshes[0].textures.push_back(gold_texture);
    gold.shader = &aShader;
    gold.single_shader = &bShader;
    gold.bind_shader();
    objList.push_back(&gold);

    Model glass(Mesh(cube_v, cube_n, cube_t));
    glass.destroy_time = 0.3f;
    glass.meshes[0].textures.push_back(glass_texture);
    glass.shader = &aShader;
    glass.single_shader = &bShader;
    glass.bind_shader();
    glass.disable_cull = true;
    glass.generate_shadow = false;
    objList.push_back(&glass);

    Model log(Mesh(cube_else_v, cube_else_n, cube_else_t));
    log.meshes.push_back(Mesh(cube_top_v, cube_top_n, cube_top_t));
    log.destroy_time = 0.3f;
    log.meshes[0].textures.push_back(log_texture);
    log.meshes[1].textures.push_back(wood_texture);
    log.shader = &aShader;
    log.single_shader = &bShader;
    log.bind_shader();
    objList.push_back(&log);

    Model leaf(Mesh(cube_v, cube_n, cube_t));
    leaf.destroy_time = 0.3f;
    leaf.meshes[0].textures.push_back(leaf_texture);
    leaf.shader = &aShader;
    leaf.single_shader = &bShader;
    leaf.bind_shader();
    leaf.disable_cull = true;
    objList.push_back(&leaf);



    Model water(Mesh(cube_v, cube_n, cube_t));
    water.destroy_time = 0.3f;
    water.skybox = sky_tex;
    water.useSkybox = true;
    water.shader = &waterShader;
    water.single_shader = &bShader;
    water.bind_shader();
    objList.push_back(&water);

#pragma endregion
    /*Model robot;
    load_model(robot, "model/nanosuit/nanosuit.obj");
    robot.shader = &cShader;
    robot.bind_shader();
    objList.push_back(&robot);*/

    Mesh debug;
    debug.vertices = cube_v;
    debug.shader = &debugShader;
    debug.shader->use();
    debug.shader->setInt("depthMap", 0);
    debug.generate_buffer_pos();

    Mesh sky;
    sky.vertices = skyboxVertices;
    sky.shader = &dShader;
    sky.shader->use();
    
    sky.shader->setInt("skybox", 0);
    sky.generate_buffer_pos();
    scene.sky = &sky;
    scene.sky_tex = &sky_tex;

    

    scene.theModel = &stone;
    for (int i = 0; i <= 100; i++) {
        for (int j = 0; j <= 100; j++) {
            scene.addItem(&stone, glm::vec3(i - 50.0f, 0.0f, j - 50.0f));
        }
    }
    
    stone.regenerate_buffer();
    scene.light.generate_depthmap();
    //dirt.meshes[0].textures[0].Texture_ID=scene.light.depthMap;

    //robot.regenerate_buffer();
    //scene.addItem_reGen(&robot, glm::vec3(1.0f, 1.5f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f));

    //scene.addItem(&robot, glm::vec3(2.0f, 1.5f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f));
    
    //std::cout << robot.vertices[0] << ' ' << robot.vertices[1] << ' ' << robot.vertices[2] << std::endl;
    float posy = 1.0f;
    
    glEnable(GL_BLEND);
    
    
    

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    //主循环
    while (!glfwWindowShouldClose(window))
    {


        processInput(window);
        
        scene.interact();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        
        

        scene.compute_depthmap();
        scene.render_skybox();
        scene.render_by_obj();
        water.shader = &waterDepthShader;
        water.shader->use();
        float temptime = (float)glfwGetTime();
        water.shader->setFloat("time", temptime);
        scene.render_single_obj(&water);
        water.shader = &waterShader;
        water.shader->use();
        water.shader->setFloat("time", temptime);
        scene.render_single_obj(&water);
        /*glClear(GL_DEPTH_BUFFER_BIT);
        glDepthMask(GL_FALSE);
        debug.shader->use();
        glBindVertexArray(debug.VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, scene.light.depthMap);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);*/

        
        //scene.render_single_obj(&water);
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /*glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);*/
    aShader.terminate();
    //终止窗口
    glfwTerminate();
    return 0;
}