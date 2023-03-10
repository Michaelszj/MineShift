#pragma once
#include "display.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <vector>
#include <map>
#include <set>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};



// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
int operate_distance = 5;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float ScrollSensitivity;
    float Zoom;
    float Aspect = 800.0f/600.0f;
    float Near = 0.1f;
    float Far = 100.0f;
    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), ScrollSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), ScrollSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }
    // returns the projection matrix
    glm::mat4 GetProjectionMatrix() {
        return glm::perspective(Zoom, Aspect, Near, Far);
    }
    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch -= yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset * ScrollSensitivity;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

    void updateInput(GLFWwindow* window) {

        //KeyBoard
        float velocity = MovementSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            Position += Front * velocity;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            Position -= Front * velocity;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            Position -= Right * velocity;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            Position += Right * velocity;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            Position -= Up * velocity;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            Position += Up * velocity;

        //Mouse
        ProcessMouseMovement(xoffset, yoffset);
        xoffset = yoffset = 0.0f;
        
        //Scroll
        ProcessMouseScroll(scroll);
        scroll = 0.0f;

        if (windowChange) {
            Aspect = (float)windowWidth / (float)windowHeight;
            windowChange = false;
        }
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};

class Ray {
public:
    glm::vec3 position, direction;
    Ray(glm::vec3 p, glm::vec3 d) :position(p), direction(d){}
};

struct Light {
    glm::vec3  Intensity;
    glm::vec3  Direction;   // For spot and directional lights.
    glm::vec3  Position;    // For point and spot lights.
    float CutOff;      // For spot lights.
    float OuterCutOff; // For spot lights.
    int   Type;
    GLuint depthMapFBO;  //深度帧缓冲
    GLuint depthMap;     //深度贴图
    static const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    Light(glm::vec3 Int, glm::vec3 pos, glm::vec3 dir = glm::vec3(-0.5f, -1.0f,- 1.0f), int type = 0, float ctof = 0.0f, float octof = 0.0f) :Intensity(Int), Direction(dir), Position(pos), CutOff(ctof), OuterCutOff(octof), Type(type) {}
    Light() {}
    void generate_depthmap() {
        glGenFramebuffers(1, &depthMapFBO);
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
            SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
};

class Location {
public:
    int x, y, z;
    Location(float x1, float y1, float z1) {
        x = round(x1);
        y = round(y1);
        z = round(z1);
    }
    Location(glm::vec3& v) {
        x = round(v.x);
        y = round(v.y);
        z = round(v.z);
    }
    Location() {}
    bool operator< (const Location& b) const {
        if (x > b.x) return true;
        if (x < b.x) return false;
        if (y > b.y) return true;
        if (y < b.y) return false; 
        if (z > b.z) return true;
        else return false;

    }
    bool operator== (const Location& b) const {
        if (x == b.x && y == b.y && z == b.z) return true;
        return false;

    }
};

class Texture {
public:
    std::string name;
    std::string path;
    GLuint Texture_ID;
    

    Texture(const char* path,  std::string Name ="") {
        name = Name;
        glGenTextures(1, &Texture_ID);
        generate_texture(Texture_ID, path);
    }

    void Bind(int n) {
        glActiveTexture(GL_TEXTURE0 + n);
        glBindTexture(GL_TEXTURE_2D, Texture_ID);
    }

    ~Texture() {
        //glDeleteTextures(1, &Texture_ID);
    }
};

class Texture_Cube {
public:
    std::string path;
    GLuint Texture_ID;

    Texture_Cube(const char* path) {
        glGenTextures(1, &Texture_ID);
        generate_texture_cube(Texture_ID, path);
    }
    Texture_Cube() {}
    void Bind(int n) {
        glActiveTexture(GL_TEXTURE0 + n);
        glBindTexture(GL_TEXTURE_CUBE_MAP, Texture_ID);
    }
};

class Mesh {
public:
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    
    GLuint VAO, VBO, EBO;
    std::vector<float> data;
    std::string directory;
    Shader* shader;
    Shader* single_shader;
    Mesh() {}
    Mesh(std::vector<float>& v, std::vector<float>& n, std::vector<float>& t) {
        vertices = v;
        normals = n;
        texCoords = t;
        generate_buffer();
    }
    Mesh(std::vector<float>& v, std::vector<float>& n, std::vector<float>& t, std::vector<GLuint>& i) {
        vertices = v;
        normals = n;
        texCoords = t;
        indices = i;
        generate_buffer_EBO();
    }
    ~Mesh() {
        //glDeleteVertexArrays(1, &VAO);
        //glDeleteBuffers(1, &VBO);
    }

    //包含整个mesh的顶点，法线，纹理坐标
    void generate_buffer() {
        
        for (int i = 0; i < vertices.size() / 3; i++) {
            data.insert(data.end(), vertices.begin() + i * 3, vertices.begin() + i * 3 + 3);
            data.insert(data.end(), normals.begin() + i * 3, normals.begin() + i * 3 + 3);
            data.insert(data.end(), texCoords.begin() + i * 2, texCoords.begin() + i * 2 + 2);
        }
        
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), &data[0], GL_STATIC_DRAW);
        data.clear();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    //只包含顶点位置
    void generate_buffer_pos() {
        
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    //包含EBO，用于非cube类模型
    void generate_buffer_EBO() {
        //std::cout << "!\n";
        for (int i = 0; i < vertices.size() / 3; i++) {
            data.insert(data.end(), vertices.begin() + i * 3, vertices.begin() + i * 3 + 3);
            data.insert(data.end(), normals.begin() + i * 3, normals.begin() + i * 3 + 3);
            data.insert(data.end(), texCoords.begin() + i * 2, texCoords.begin() + i * 2 + 2);
        }
       
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &EBO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        data.clear();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    

    void draw(glm::mat4& m, glm::mat4& v, glm::mat4& p, glm::mat4 r) {
        single_shader->use();
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);

        single_shader->setInt("texture1", 0);
        single_shader->setInt("destroy", 1);
;
        for (int i = 0; i < textures.size(); i++) {
            textures[i].Bind(i);
        }

        single_shader->setMat4("model", m);
        single_shader->setMat4("view", v);
        single_shader->setMat4("projection", p);
        single_shader->setMat4("rotation", r);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }
    void draw_EBO(glm::mat4& m, glm::mat4& v, glm::mat4& p, glm::mat4 r) {
        single_shader->use();
        glBindVertexArray(VAO);
        //glBindBuffer(GL_ARRAY_BUFFER, VBO);

        single_shader->setInt("diffuse_map", 0);
        single_shader->setInt("specualr_map", 1);
        
        for (int i = 0; i < textures.size(); i++) {
            textures[i].Bind(i);
        }

        single_shader->setMat4("model", m);
        single_shader->setMat4("view", v);
        single_shader->setMat4("projection", p);
        single_shader->setMat4("rotation", r);

        //std::cout << "drawing\n";
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }

};

class Model;

class Instance {
public:
    Model* proto;
    glm::vec3 position, rotation, scale;

    Instance(Model* p) {
        proto = p;
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        scale = glm::vec3(1.0f, 1.0f, 1.0f);
    }
    Instance() {}
    glm::mat4 get_model() {
        glm::mat4 trans(1.0f);
        trans = glm::translate(trans, position);
        trans = glm::scale(trans, scale);
        trans = glm::rotate(trans, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::rotate(trans, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        trans = glm::rotate(trans, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));

        return trans;
    }
    glm::mat4 get_rot() {
        glm::mat4 trans(1.0f);
        trans = glm::rotate(trans, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::rotate(trans, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        trans = glm::rotate(trans, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));

        return trans;
    }
    float intersect(Ray ray, glm::vec3& intersectP) {
        glm::vec3 way = position - ray.position;
        float ddistance = glm::length(way);
        float adistance = glm::dot(way, ray.direction);
        float vdistance = std::sqrt(ddistance * ddistance - adistance * adistance);
        if (vdistance > 0.866 || adistance < 0.0f) return -1.0f;

    }

};

class Model {
public:
    std::vector<Mesh> meshes;
    std::string directory;
    float destroy_time = 1.0f;
    Shader* shader;
    Shader* single_shader;
    Texture_Cube skybox;
    bool useSkybox = false;
    std::set<Instance*> instances;
    unsigned int inst_num;
    unsigned int instance_buffer;
    bool disable_cull = false;
    bool generate_shadow = true;

    Model() { glGenBuffers(1, &instance_buffer); }
    Model(Mesh m) {
        meshes.push_back(m);
        glGenBuffers(1, &instance_buffer);
    }

    void bind_shader() {
        for (int i = 0; i < meshes.size(); i++) {
            meshes[i].shader = this->shader;
            meshes[i].single_shader = this->single_shader;
        }
    }
    void draw(glm::mat4& m, glm::mat4& v, glm::mat4& p, glm::mat4 r) {
        for (auto i : meshes) i.draw(m, v, p, r);
    }
    void draw_EBO(glm::mat4& m, glm::mat4& v, glm::mat4& p, glm::mat4 r) {
        for (auto i : meshes) i.draw_EBO(m, v, p, r);
    }
    void regenerate_buffer() {
        auto model = this;
        auto locs = instances;
        int length = locs.size();
        glm::mat4* mats = new glm::mat4[2 * length];
        int temp = 0;
        for (auto j : locs) {
            
            mats[temp * 2] = j->get_model();
            mats[temp * 2 + 1] = j->get_rot();
            temp++;
        }
        if (temp == 0) return;
        unsigned int buffer = model->instance_buffer;
        //glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, 2 * temp * sizeof(glm::mat4), &mats[0], GL_STATIC_DRAW);
        //std::cout << length << std::endl;
        for (unsigned int j = 0; j < model->meshes.size(); j++) {

            unsigned int VAO = model->meshes[j].VAO;
            glBindVertexArray(VAO);

            GLsizei vec4Size = sizeof(glm::vec4);

            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 8 * vec4Size, (void*)0);
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 8 * vec4Size, (void*)(vec4Size));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 8 * vec4Size, (void*)(2 * vec4Size));
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 8 * vec4Size, (void*)(3 * vec4Size));
            glEnableVertexAttribArray(7);
            glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 8 * vec4Size, (void*)(4 * vec4Size));
            glEnableVertexAttribArray(8);
            glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 8 * vec4Size, (void*)(5 * vec4Size));
            glEnableVertexAttribArray(9);
            glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, 8 * vec4Size, (void*)(6 * vec4Size));
            glEnableVertexAttribArray(10);
            glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, 8 * vec4Size, (void*)(7 * vec4Size));

            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);
            glVertexAttribDivisor(7, 1);
            glVertexAttribDivisor(8, 1);
            glVertexAttribDivisor(9, 1);
            glVertexAttribDivisor(10, 1);


            glBindVertexArray(0);
        }

    }
};

std::vector<Model*> objList;



class Player {
public:
    Model* selectItem;
    Location facingItem;
    Player() {
        selectItem = nullptr;
        facingItem = Location(10000.0f,10000.0f,10000.0f);
    }
    void interact() {
        //对玩家使用的方块的切换处理
        if (changeItem == true) {
            if (sItem <= objList.size() - 1) {
                //std::cout << "select a new item\n";
                selectItem = objList[sItem];
            }
            changeItem = false;
        }
    }
};

class Scene {
public:
    GLFWwindow* window;
    
    Model* theModel;
    std::set<Model*> models;
    std::map<Location,Instance> spatialMap;
    Camera camera;
    Light light;
    Shader* depthShader;
    Player player;
    Texture* destroy;
    Mesh* sky;
    Texture_Cube* sky_tex;
    Scene(GLFWwindow* w) : camera(glm::vec3(0.0f, 0.0f, 4.0f)),light(glm::vec3(1.0f,1.0f,1.0f),glm::vec3(450.0f,450.0f,450.0f)){
        window = w;
    }

    void interact() {
        if (enableScene == true)
            camera.updateInput(window);

        //放置方块的处理
        if (place == true) {
            glm::vec3 loc;
            //std::cout << "!\n";
            if (getTheBlock(loc)) {
                //std::cout << "!\n";
                float dx = loc.x - camera.Position.x;
                float dz = loc.z - camera.Position.z;
                int temp;
                if (fabs(dz) > fabs(dx)) {
                    if (dz < 0.0f)
                        temp = 0;
                    else temp = 2;
                }
                else {
                    if (dx > 0.0f)
                        temp = 3;
                    else temp = 1;
                }
                addItem_reGen(player.selectItem, loc ,temp);
            }

            place = false;
        }

        player.interact();

        //判断方块的拆除
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            glm::vec3 loc;
            if (getTheBlock_delete(loc)) {
                Location Loc(loc);
                if (Loc == player.facingItem) {
                    eraseTime += deltaTime;
                    if (eraseTime >= spatialMap[Loc].proto->destroy_time) {
                        eraseTime = 0.0f;
                        deleteItem(Loc);
                    }
                }
                else {
                    eraseTime = deltaTime;
                    player.facingItem = Loc;
                }
            }
            else {
                eraseTime = 0.0f;
                player.facingItem = Location(10000.0f, 10000.0f, 10000.0f);
            }
        }
        else {
            eraseTime = 0.0f;
            player.facingItem = Location();
        }
    }

    void compute_depthmap() {
        glViewport(0, 0, Light::SHADOW_WIDTH, Light::SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, light.depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        GLfloat near_plane = 1.0f, far_plane = 300.0f;
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(camera.Position - 100.0f * light.Direction, camera.Position, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        depthShader->use();
        depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

        for (auto x : models) {
            auto model = x;
            auto locs = x->instances;
            for (unsigned int j = 0; j < model->meshes.size(); j++) {
                glBindVertexArray(model->meshes[j].VAO);
                if (model->meshes.size() >= 3) {
                    glDrawElementsInstanced(
                        GL_TRIANGLES, model->meshes[j].indices.size(), GL_UNSIGNED_INT, 0, model->inst_num
                    );
                }
                else {
                    glDrawArraysInstanced(GL_TRIANGLES, 0, model->meshes[j].vertices.size(), model->inst_num);
                }

            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, windowWidth, windowHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void render() {
        
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix();
       
        for (auto i : spatialMap) {
            glm::mat4 model = i.second.get_model();
            glm::mat4 rot = i.second.get_rot();
            i.second.proto->shader->use();
            i.second.proto->shader->setVec3("eye",camera.Position);
            //对于正在拆除的方块特殊处理
            if (i.first == player.facingItem) {
                //std::cout << "!\n";
                i.second.proto->shader->setBool("useD", true);
                destroy[int(floor(eraseTime * 10.0f / i.second.proto->destroy_time))].Bind(1);
            }
            setLight(i.second.proto->shader);
            if (i.second.proto->meshes.size() == 1) {
                i.second.proto->meshes[0].draw(model, view, projection, rot);
            }
            else {
                i.second.proto->draw_EBO(model, view, projection, rot);
            }
            if (i.first == player.facingItem) {
                i.second.proto->shader->setBool("useD", false);
            }
        }
        glm::vec3 loc;

        //对将要放置的方块用线框模式渲染
        if (getTheBlock(loc) && eraseTime == 0.0f) {
            //std::cout << "!\n";
            
            Instance instance(theModel);
            instance.position = loc;
            glm::mat4 model = instance.get_model();
            instance.proto->shader->use();
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            setLight(instance.proto->shader);
            instance.proto->draw(model, view, projection, glm::mat4(1));
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        
    }

    void render_by_obj() {
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix();

        GLfloat near_plane = 1.0f, far_plane = 300.0f;
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(camera.Position - 100.0f * light.Direction, camera.Position, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        for (auto x : models) {
            
            auto model = x;
            auto locs = x->instances;
            //x->regenerate_buffer();
            if (model->useSkybox == true) continue;
            model->shader->use();

            model->shader->setTex();
            model->shader->setVec3("eye", camera.Position);
            
            model->shader->setMat4("view", view);
            model->shader->setMat4("projection", projection);
            model->shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
            setLight(model->shader);

            if (model->disable_cull == true) {
                glDisable(GL_CULL_FACE);
            }
            for (unsigned int j = 0; j < model->meshes.size(); j++) {
                glBindVertexArray(model->meshes[j].VAO);
                for (int i = 0; i < model->meshes[j].textures.size(); i++) {
                    model->meshes[j].textures[i].Bind(i);
                    
                }
                glActiveTexture(GL_TEXTURE0 + 2);
                glBindTexture(GL_TEXTURE_2D, light.depthMap);
                if (model->useSkybox == true) {
                    model->shader->setInt("skybox", model->meshes[j].textures.size());
                    model->skybox.Bind(model->meshes[j].textures.size());
                }
                if (model->meshes.size() >= 3) {
                    glDrawElementsInstanced(
                        GL_TRIANGLES, model->meshes[j].indices.size(), GL_UNSIGNED_INT, 0, model->inst_num
                    );
                }
                else {
                    glDrawArraysInstanced(GL_TRIANGLES, 0, model->meshes[j].vertices.size(),model->inst_num);
                }
                
            }
            glEnable(GL_CULL_FACE);
        }


        //对将要放置的方块用线框模式渲染
        glm::vec3 loc;
        if (getTheBlock(loc) && eraseTime == 0.0f) {
            Instance instance(theModel);
            instance.position = loc;
            glm::mat4 model = instance.get_model();
            instance.proto->single_shader->use();
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            setLight(instance.proto->single_shader);
            instance.proto->draw(model, view, projection, glm::mat4(1));
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        //对于正在拆除的方块特殊处理
        if (spatialMap.find(player.facingItem) != spatialMap.end()) {

            auto& destroying = spatialMap[player.facingItem];
            glm::mat4 model = destroying.get_model();
            glm::mat4 rot = destroying.get_rot();
            destroying.proto->single_shader->use();
            destroying.proto->single_shader->setVec3("eye", camera.Position);
            destroying.proto->single_shader->setBool("useD", true);
            destroy[int(floor(eraseTime * 10.0f / destroying.proto->destroy_time))].Bind(1);
            setLight(destroying.proto->single_shader);
            for (auto mesh : destroying.proto->meshes) {
                mesh.draw(model, view, projection, rot);
            }
            //destroying.proto->meshes[0].draw(model, view, projection, rot);
            destroying.proto->single_shader->setBool("useD", false);
        }

    }

    void render_single_obj(Model* model) {
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix();

        auto locs = model->instances;

        model->shader->use();

        model->shader->setTex();
        model->shader->setVec3("eye", camera.Position);

        model->shader->setMat4("view", view);
        model->shader->setMat4("projection", projection);

        setLight(model->shader);
        for (unsigned int j = 0; j < model->meshes.size(); j++) {
            glBindVertexArray(model->meshes[j].VAO);
            for (int i = 0; i < model->meshes[j].textures.size(); i++) {
                model->meshes[j].textures[i].Bind(i);

            }
            if (model->useSkybox == true) {
                model->shader->setInt("skybox", model->meshes[j].textures.size());
                model->skybox.Bind(model->meshes[j].textures.size());
            }
            if (model->meshes.size() >= 3) {
                glDrawElementsInstanced(
                    GL_TRIANGLES, model->meshes[j].indices.size(), GL_UNSIGNED_INT, 0, model->inst_num
                );
            }
            else {
                glDrawArraysInstanced(GL_TRIANGLES, 0, model->meshes[j].vertices.size(), model->inst_num);
            }

        }
    }

    void render_skybox() {
        glDepthMask(GL_FALSE);
        sky->shader->use();
        auto projection = camera.GetProjectionMatrix();
        auto view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        
        sky->shader->setMat4("projection", projection);
        sky->shader->setMat4("view", view);


        glBindVertexArray(sky->VAO);


        sky_tex->Bind(0);


        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);
    }


    bool intersectRay(Ray ray,Location& hitPos) {
        float t = 100000.0f;
        //std::cout << operate_distance << std::endl;
        if (ray.direction.x > 0.001f) {
            float expect = std::ceilf(ray.position.x - 0.5f) + 0.5f;
            for (int i = 0; i < operate_distance; i++) {
                float time = (expect - ray.position.x) / ray.direction.x;
                glm::vec3 dest = ray.direction * time + ray.position;
                dest.x += 0.5f;
                Location loc(dest);
                //std::cout << "checking " << loc.x << ' ' << loc.y << ' ' << loc.z << std::endl;
                if (spatialMap.find(loc) != spatialMap.end()) {
                    if (time < t) {
                        t = time;
                        loc.x -= 1;
                        hitPos = loc;
                       
                    }
                    break;
                }
                expect += 1.0f;
            }
        }
        else if (ray.direction.x < -0.001f) {
            float expect = std::floorf(ray.position.x - 0.5f) + 0.5f;
            for (int i = 0; i < operate_distance; i++) {
                float time = (expect - ray.position.x) / ray.direction.x;
                glm::vec3 dest = ray.direction * time + ray.position;
                dest.x -= 0.5f;
                Location loc(dest);
                //std::cout << "checking " << loc.x << ' ' << loc.y << ' ' << loc.z << std::endl;
                //std::cout << "checking " << dest.x << ' ' << dest.y << ' ' << dest.z << std::endl;
                if (spatialMap.find(loc) != spatialMap.end()) {
                    if (time < t) {
                        t = time;
                        loc.x += 1;
                        hitPos = loc;
                        
                    }
                    break;
                }
                expect -= 1.0f;
            }
        }
        if (ray.direction.y > 0.001f) {
            float expect = std::ceilf(ray.position.y - 0.5f) + 0.5f;
            for (int i = 0; i < operate_distance; i++) {
                float time = (expect - ray.position.y) / ray.direction.y;
                glm::vec3 dest = ray.direction * time + ray.position;
                dest.y += 0.5f;
                Location loc(dest);
                //std::cout << "checking " << loc.x << ' ' << loc.y << ' ' << loc.z << std::endl;
                if (spatialMap.find(loc) != spatialMap.end()) {
                    if (time < t) {
                        t = time;
                        loc.y -= 1;
                        hitPos = loc;
                    }
                    break;
                }
                expect += 1.0f;
            }
        }
        else if (ray.direction.y < -0.001f) {
            float expect = std::floorf(ray.position.y - 0.5f) + 0.5f;
            for (int i = 0; i < operate_distance; i++) {
                float time = (expect - ray.position.y) / ray.direction.y;
                glm::vec3 dest = ray.direction * time + ray.position;
                dest.y -= 0.5f;
                Location loc(dest);
                //std::cout << "checking " << loc.x << ' ' << loc.y << ' ' << loc.z << std::endl;
                if (spatialMap.find(loc) != spatialMap.end()) {
                    if (time < t) {
                        t = time;
                        loc.y += 1;
                        hitPos = loc;
                    }
                    break;
                }
                expect -= 1.0f;
            }
        }
        if (ray.direction.z > 0.001f) {
            float expect = std::ceilf(ray.position.z - 0.5f) + 0.5f;
            for (int i = 0; i < operate_distance; i++) {
                float time = (expect - ray.position.z) / ray.direction.z;
                glm::vec3 dest = ray.direction * time + ray.position;
                dest.z += 0.5f;
                Location loc(dest);
                //std::cout << "checking " << loc.x << ' ' << loc.y << ' ' << loc.z << std::endl;
                if (spatialMap.find(loc) != spatialMap.end()) {
                    if (time < t) {
                        t = time;
                        loc.z -= 1;
                        hitPos = loc;
                    }
                    break;
                }
                expect += 1.0f;
            }
        }
        else if (ray.direction.z < -0.001f) {
            float expect = std::floorf(ray.position.z - 0.5f) + 0.5f;
            for (int i = 0; i < operate_distance; i++) {
                float time = (expect - ray.position.z) / ray.direction.z;
                glm::vec3 dest = ray.direction * time + ray.position;
                dest.z -= 0.5f;
                Location loc(dest);
                //std::cout << "checking " << loc.x << ' ' << loc.y << ' ' << loc.z << std::endl;
                //std::cout << "checking " << dest.x << ' ' << dest.y << ' ' << dest.z << std::endl;
                if (spatialMap.find(loc) != spatialMap.end()) {
                    if (time < t) {
                        t = time;
                        loc.z += 1;
                        hitPos = loc;
                    }
                    break;
                }
                expect -= 1.0f;
            }
        }
        if (t == 100000.0f) return false;
        if (glm::length(ray.position - glm::vec3(hitPos.x, hitPos.y, hitPos.z)) > operate_distance * std::sqrt(3.0)) return false;
        else return true;
    }

    bool intersectRay_delete(Ray ray, Location& hitPos) {
        float t = 100000.0f;
        if (ray.direction.x > 0.001f) {
            float expect = std::ceilf(ray.position.x - 0.5f) + 0.5f;
            for (int i = 0; i < operate_distance; i++) {
                float time = (expect - ray.position.x) / ray.direction.x;
                glm::vec3 dest = ray.direction * time + ray.position;
                dest.x += 0.5f;
                Location loc(dest);
                //std::cout << "checking " << loc.x << ' ' << loc.y << ' ' << loc.z << std::endl;
                if (spatialMap.find(loc) != spatialMap.end()) {
                    if (time < t) {
                        t = time;
                        
                        hitPos = loc;

                    }
                    break;
                }
                expect += 1.0f;
            }
        }
        else if (ray.direction.x < -0.001f) {
            float expect = std::floorf(ray.position.x - 0.5f) + 0.5f;
            for (int i = 0; i < operate_distance; i++) {
                float time = (expect - ray.position.x) / ray.direction.x;
                glm::vec3 dest = ray.direction * time + ray.position;
                dest.x -= 0.5f;
                Location loc(dest);
                //std::cout << "checking " << loc.x << ' ' << loc.y << ' ' << loc.z << std::endl;
                //std::cout << "checking " << dest.x << ' ' << dest.y << ' ' << dest.z << std::endl;
                if (spatialMap.find(loc) != spatialMap.end()) {
                    if (time < t) {
                        t = time;
                        
                        hitPos = loc;

                    }
                    break;
                }
                expect -= 1.0f;
            }
        }
        if (ray.direction.y > 0.001f) {
            float expect = std::ceilf(ray.position.y - 0.5f) + 0.5f;
            for (int i = 0; i < operate_distance; i++) {
                float time = (expect - ray.position.y) / ray.direction.y;
                glm::vec3 dest = ray.direction * time + ray.position;
                dest.y += 0.5f;
                Location loc(dest);
                //std::cout << "checking " << loc.x << ' ' << loc.y << ' ' << loc.z << std::endl;
                if (spatialMap.find(loc) != spatialMap.end()) {
                    if (time < t) {
                        t = time;
                      
                        hitPos = loc;
                    }
                    break;
                }
                expect += 1.0f;
            }
        }
        else if (ray.direction.y < -0.001f) {
            float expect = std::floorf(ray.position.y - 0.5f) + 0.5f;
            for (int i = 0; i < operate_distance; i++) {
                float time = (expect - ray.position.y) / ray.direction.y;
                glm::vec3 dest = ray.direction * time + ray.position;
                dest.y -= 0.5f;
                Location loc(dest);
                //std::cout << "checking " << loc.x << ' ' << loc.y << ' ' << loc.z << std::endl;
                if (spatialMap.find(loc) != spatialMap.end()) {
                    if (time < t) {
                        t = time;
                        
                        hitPos = loc;
                    }
                    break;
                }
                expect -= 1.0f;
            }
        }
        if (ray.direction.z > 0.001f) {
            float expect = std::ceilf(ray.position.z - 0.5f) + 0.5f;
            for (int i = 0; i < operate_distance; i++) {
                float time = (expect - ray.position.z) / ray.direction.z;
                glm::vec3 dest = ray.direction * time + ray.position;
                dest.z += 0.5f;
                Location loc(dest);
                //std::cout << "checking " << loc.x << ' ' << loc.y << ' ' << loc.z << std::endl;
                if (spatialMap.find(loc) != spatialMap.end()) {
                    if (time < t) {
                        t = time;
                        
                        hitPos = loc;
                    }
                    break;
                }
                expect += 1.0f;
            }
        }
        else if (ray.direction.z < -0.001f) {
            float expect = std::floorf(ray.position.z - 0.5f) + 0.5f;
            for (int i = 0; i < operate_distance; i++) {
                float time = (expect - ray.position.z) / ray.direction.z;
                glm::vec3 dest = ray.direction * time + ray.position;
                dest.z -= 0.5f;
                Location loc(dest);
                //std::cout << "checking " << loc.x << ' ' << loc.y << ' ' << loc.z << std::endl;
                //std::cout << "checking " << dest.x << ' ' << dest.y << ' ' << dest.z << std::endl;
                if (spatialMap.find(loc) != spatialMap.end()) {
                    if (time < t) {
                        t = time;
                        
                        hitPos = loc;
                    }
                    break;
                }
                expect -= 1.0f;
            }
        }
        if (t == 100000.0f) return false;
        if (glm::length(ray.position - glm::vec3(hitPos.x, hitPos.y, hitPos.z)) > operate_distance * std::sqrt(2.0)) return false;
        else return true;
    }

    //找到视线中央将要被放置的方块
    bool getTheBlock(glm::vec3& locf) {
        Ray cameraRay(camera.Position, camera.Front);
        Location loc;
        if (intersectRay(cameraRay, loc) == false) return false;
        locf = glm::vec3((float)loc.x, (float)loc.y, (float)loc.z);
        return true;
    }

    //找到视线中央将要被拆除的方块
    bool getTheBlock_delete(glm::vec3& locf) {
        Ray cameraRay(camera.Position, camera.Front);
        Location loc;
        if (intersectRay_delete(cameraRay, loc) == false) return false;
        locf = glm::vec3((float)loc.x, (float)loc.y, (float)loc.z);
        return true;
    }


    void addItem(Model* proto, glm::vec3 position) {
        if (proto == nullptr) {
            std::cout << "can't find prototype of object\n";
        }
        Instance instance(proto);
        instance.position = position;
        proto->inst_num++;
        spatialMap.insert(std::pair<Location, Instance>(Location(position), instance));
        models.insert(proto);
        proto->instances.insert(&spatialMap.find(Location(position))->second);
    }

    void addItem_reGen(Model* proto, glm::vec3 position) {
        addItem(proto, position);
        proto->regenerate_buffer();
    }

    void addItem(Model* proto, glm::vec3 position, int dir) {
        if (proto == nullptr) {
            std::cout << "can't find prototype of object\n";
        }
        Instance instance(proto);
        instance.position = position;
        instance.rotation = glm::vec3(0.0f, glm::pi<float>(), 0.0f) * (float(dir) / 2.0f);
        //std::cout << dir << std::endl;
        proto->inst_num++;
        spatialMap.insert(std::pair<Location, Instance>(Location(position), instance));
        models.insert(proto);
        proto->instances.insert(&spatialMap.find(Location(position))->second);
        
    }

    void addItem_reGen(Model* proto, glm::vec3 position, int dir) {
        addItem(proto, position, dir);
        proto->regenerate_buffer();
    }

    void addItem(Model* proto, glm::vec3 position, glm::vec3 scale) {
        if (proto == nullptr) {
            std::cout << "can't find prototype of object\n";
        }
        Instance instance(proto);
        instance.position = position;
        instance.scale = scale;
        proto->inst_num++;
        spatialMap.insert(std::pair<Location, Instance>(Location(position), instance));
        models.insert(proto);
        proto->instances.insert(&spatialMap.find(Location(position))->second);
    }

    void addItem_reGen(Model* proto, glm::vec3 position, glm::vec3 scale) {
        addItem(proto, position, scale);
        proto->regenerate_buffer();
    }

    void deleteItem(Location loc) {
        auto proto = spatialMap[loc].proto;
        proto->instances.erase(&spatialMap[loc]);
        spatialMap.erase(loc);
        proto->inst_num--;
        proto->regenerate_buffer();
    }

    void setLight(Shader* shader) {
        shader->setVec3("light.Intensity", light.Intensity);
        shader->setVec3("light.Direction", light.Direction); 
        shader->setVec3("light.Position",   light.Position);
        shader->setFloat("light.CutOff",      light.CutOff);
        shader->setFloat("light.OuterCutOff", light.OuterCutOff);
        shader->setInt("light.Type", light.Type);
    }
};


