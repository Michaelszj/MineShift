#pragma warning(disable : 4996)
#pragma once

#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f; // ��һ֡��ʱ��
float lastX = 400, lastY = 300;
float xoffset = 0.0f, yoffset = 0.0f;
bool enableMouse = false;
float scroll = 0.0f;
bool enableScene = true;
bool place = false;     //�Ƿ����
bool windowChange = false;
int windowWidth = 800, windowHeight = 600;
float eraseTime = 0.0f;

int sItem = 0;            //��ǰ����
bool changeItem = false;  //�Ƿ�ı�����

//��ʼ��
void glInit() {
    //��ʼ��
    glfwInit();
    //�汾����
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //ģʽ����
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    

    
}


//�ı䴰�ڴ�С�Ļص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;
    windowChange = true;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!enableMouse) {
        enableMouse = true;
        lastX = xpos;
        lastY = ypos;
    }
    else {
        xoffset = xpos - lastX;
        yoffset = ypos - lastY;
        lastX = xpos;
        lastY = ypos;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        place = true;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    scroll = yoffset;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        
        //std::cout << "!\n";
        if (enableScene) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            enableScene = false;
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            enableScene = true;
        }
    }
    if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9 && action == GLFW_PRESS) {
        sItem = key - GLFW_KEY_1;
        changeItem = true;
    }
}

//��������
GLFWwindow* create_window(int length, int width, const char* name, GLFWmonitor* monitor, GLFWwindow* share) {
    GLFWwindow* window = glfwCreateWindow(length, width, name, monitor, share);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return window;
    }
    glfwMakeContextCurrent(window);

    //ע��ص�����
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    return window;
}


//��������
void processInput(GLFWwindow* window)
{
    float present = glfwGetTime();
    deltaTime = present - lastFrame;
    lastFrame = present;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
        

}





//���ļ�������ɫ��
unsigned int load_vertex_shader(const char* filePath, char* shaderString) {
    //��ʼ��
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //���ļ�
    std::string content;
    std::ifstream fileStream;
    fileStream.open(filePath);
    std::stringstream shaderStream;
    shaderStream << fileStream.rdbuf();
    fileStream.close();
    content = shaderStream.str();
    shaderString = new char[content.length()]{};
    strcpy(shaderString, content.c_str());

    //����
    glShaderSource(vertexShader, 1, &shaderString, NULL);
    glCompileShader(vertexShader);
    
    //������
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return vertexShader;
}

unsigned int load_fragment_shader(const char* filePath, char* shaderString) {
    //��ʼ��
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    //���ļ�
    std::string content;
    std::ifstream fileStream;
    fileStream.open(filePath);
    std::stringstream shaderStream;
    shaderStream << fileStream.rdbuf();
    fileStream.close();
    content = shaderStream.str();
    shaderString = new char[content.length()]{};
    strcpy(shaderString, content.c_str());

    //����
    glShaderSource(fragmentShader, 1, &shaderString, NULL);
    glCompileShader(fragmentShader);

    //������
    int  success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return fragmentShader;
}


class Shader {
public:

    unsigned int ID;
    std::vector<std::string> textures;

    // ��������ȡ��������ɫ��
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        // 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ��֤ifstream��������׳��쳣��
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // ���ļ�
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // ��ȡ�ļ��Ļ������ݵ���������
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // �ر��ļ�������
            vShaderFile.close();
            fShaderFile.close();
            // ת����������string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. ������ɫ��
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // ������ɫ��
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // ��ӡ�����������еĻ���
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        };
        // Ƭ����ɫ��
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // ��ӡ�����������еĻ���
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        };

        // ��ɫ������
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        // ��ӡ���Ӵ�������еĻ���
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        // ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    // ʹ��/�������
    void use() {
        glUseProgram(ID);
    }
    // uniform���ߺ���
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setVec3(const std::string& name, glm::vec3 value) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
    }
    void setMat4(const std::string& name, glm::mat4 value) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }
    void setTex() {
        for (int i = 0; i < textures.size(); i++) {
            setInt(textures[i], i);
        }
        
    }
    void terminate() const {
        glDeleteProgram(ID);
    }
};


void generate_texture(unsigned int texture_ID, const char* file_path) {
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    // Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // ���ز���������
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file_path, &width, &height, &nrChannels, 0);
    if (data)
    {
        if (nrChannels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        if (nrChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}


std::string cube_post[6]{
    "right.png","left.png","top.png","bottom.png","front.png","back.png"
};

void generate_texture_cube(unsigned int texture_ID, const char* file_path) {
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_ID);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    stbi_set_flip_vertically_on_load(false);
    for (unsigned int i = 0; i < 6; i++) {
        int width, height, nrChannels;
        unsigned char* data = stbi_load((std::string(file_path)+cube_post[i]).c_str(), &width, &height, &nrChannels, 0);

        if (data)
        {
            if (nrChannels == 3) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            if (nrChannels == 4) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
            
        }
        else
        {
            std::cout << "Failed to load texture in" << std::string(file_path) + cube_post[i] <<std::endl;
        }
        stbi_image_free(data);
        
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

