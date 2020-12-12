#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Mesh.h"

enum EntityType { PLAYER, PLATFORM, ENEMY, CUBE, SHIP, FLOOR, CRATE };

class Entity {
public:
    EntityType entityType;
    bool billboard;
    float width;
    float height;
    float depth;
    int lives;
    int numShips;
    bool alive;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 rotation;
    glm::vec3 scale;
    
    float speed;
    
    GLuint textureID;
//    float *vertices;
//    float *texCoords;
//    int numVertices;
    Mesh *mesh;
    
    
    glm::mat4 modelMatrix;
    void DrawBillboard(ShaderProgram *program);
    Entity();
    bool CheckCollision(Entity *other);
    void Update(float deltaTime, Entity *player, Entity *objects, int objectCount);
//    void Update(float deltaTime);
    void Render(ShaderProgram *program);
};



