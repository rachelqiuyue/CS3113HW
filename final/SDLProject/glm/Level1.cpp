#include <stdio.h>

#include "Level1.h"
#define OBJECT_COUNT 4

void Level1::Initialize() {
    state.nextScene = -1;
        state.player = new Entity();
        state.player->entityType = PLAYER;
        state.player->position = glm::vec3(0, 0.75, 0);
        state.player->acceleration = glm::vec3(0, 0, 0);
        state.player->speed = 1.0f;
        state.player->lives = 3;
        state.player->numShips = 0;
    
        state.objects = new Entity[OBJECT_COUNT];
        GLuint floorTextureID = Util::LoadTexture("grass.jpg");
        state.objects[0].textureID = floorTextureID;
        state.objects[0].position = glm::vec3(0,-0.25f,0);
        state.objects[0].entityType = FLOOR;
        Mesh* cubeMesh = new Mesh();
        cubeMesh->LoadOBJ("cube.obj", 50);
        state.objects[0].mesh = cubeMesh;
        state.objects[0].rotation = glm::vec3(0,0,0);
        state.objects[0].acceleration = glm::vec3(0,0,0);
        state.objects[0].scale = glm::vec3(50,0.5f,50);
    
        Mesh *crateMesh = new Mesh();
        crateMesh->LoadOBJ("advancedCharacter.obj", 1);
        GLuint enemyTextureID = Util::LoadTexture("skin_adventurer.png");
        for (int i = 1; i < OBJECT_COUNT; i++) {
    
            state.objects[i].position = glm::vec3(rand() % 20 , 0, rand() % 20 -20);
            state.objects[i].textureID = enemyTextureID;
            state.objects[i].entityType = CRATE;
            state.objects[i].mesh = crateMesh;
            state.objects[i].rotation = glm::vec3(0,0,0);
            state.objects[i].scale = glm::vec3(0.1f,0.1f,0.1f);
        }
}

void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime,state.player,state.objects,OBJECT_COUNT);
    for (int i = 0; i < OBJECT_COUNT;i++){
        state.objects[i].Update(deltaTime,state.player,state.objects,OBJECT_COUNT);
    }
    
    for (int i = 0; i < state.ships.size();i++){
        state.ships[i].Update(deltaTime,state.player,state.objects,OBJECT_COUNT);
    }
}
void Level1::Render(ShaderProgram *program) {
    if (state.player->lives <= 0)return;
    int enemyCount = 0;
    for (int i = 0; i < OBJECT_COUNT;i++){
        state.objects[i].Render(program);
        if (state.objects[i].entityType == CRATE && state.objects[i].alive){
            enemyCount += 1;
        }
    }
    for (int i = 0; i < state.ships.size();i++){
        state.ships[i].Render(program);
    }
    GLuint fontTextureID = Util::LoadTexture("font1.png");
    if (enemyCount == 0){
        state.nextScene = 2;
    }
    glm::mat4 uiViewMatrix = glm::mat4(1.0);
    glm::mat4 uiProjectionMatrix = glm::ortho(-6.4f, 6.4f, -3.6f, 3.6f, -1.0f, 1.0f);
            program->SetProjectionMatrix(uiProjectionMatrix);
            program->SetViewMatrix(uiViewMatrix);
    Util::DrawText(program, fontTextureID, "lives: "+std::to_string(state.player->lives), 0.5, -0.3f,
                   glm::vec3(-6, 3.2, 0));
    Util::DrawText(program, fontTextureID, "enemies: "+std::to_string(enemyCount), 0.5, -0.3f,
                   glm::vec3(3, 3.2, 0));
}
