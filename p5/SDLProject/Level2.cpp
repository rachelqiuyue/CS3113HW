//
//  Level1.cpp
//  SDLProject
//
//  Created by QIUYUE WANG on 11/14/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include <stdio.h>

#include "Level2.h"
#define LEVEL2_WIDTH 14
#define LEVEL2_HEIGHT 8
#define LEVEL2_ENERMY_COUNT 1
unsigned int level2_data[] =
{
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};




void Level2::Initialize() {
    state.nextScene = -1;
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 4, 1);
    // Move over all of the player and enemy code from initialization.
    state.player = new Entity();
    //    state.player->h = 2;
    state.player->position = glm::vec3(2,-3,0);
    state.player->movement = glm::vec3(0);
    state.player->entityType = PLAYER;
    state.player->acceleration = glm::vec3(0,-10.0f,0);
    state.player->speed = 3.0f;
    state.player->textureID = Util::LoadTexture("george_0.png");
    state.player->height = 0.8f;
    state.player->width = 0.8f;
    state.player->animRight = new int[4] {3, 7, 11, 15};
    state.player->animLeft = new int[4] {1, 5, 9, 13};
    state.player->animUp = new int[4] {2, 6, 10, 14};
    state.player->animDown = new int[4] {0, 4, 8, 12};
    
    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;
    
    
    
    
    state.enemies = new Entity[1];
    state.enemies[0].position = glm::vec3(10,-2,0);
    state.enemies[0].entityType = ENERMY;
    state.enemies[0].movement = glm::vec3(0);
    state.enemies[0].acceleration = glm::vec3(0,-10,0);
    state.enemies[0].speed = 1.0f;
    state.enemies[0].textureID = Util::LoadTexture("character_robot_idle.png");
    state.enemies[0].height = 0.8f;
    state.enemies[0].width = 0.8f;
    //        state.enemies[0].Update(0,state.enemies,NULL,0, state.map);
}
void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL2_ENERMY_COUNT, state.map);
    //   state.enemies[0].Update(deltaTime, state.player, NULL, 0, state.map);
    //    if (state.enemies[0].checkCollision(state.player)){
    //        state.player->isActive = false;
    //    }
    //    if (state.enemies[0].collidedLeft){
    //        //        GLuint fontTexture = Util::LoadTexture("pixel_font.png");
    //        //        Util::DrawText(program, fontTexture, "You lost", 0.2,
    //        //                       0.05, glm::vec3(state.player->position.x,-2,0));
    //        state.enemies[0].isActive = false;
    //    }
    state.enemies[0].Update(deltaTime, state.player, NULL, 0, state.map);
//    if (state.enemies[0].checkCollision(state.player)&& state.enemies[0].isActive ){
//        state.player->isActive = false;
//    }
    

}
void Level2::Render(ShaderProgram *program) {
    if (state.enemies[0].checkCollision(state.player)&& state.enemies[0].isActive ){
        state.player->lives -= 1;
        state.player->position =glm::vec3(2,-3,0);
        if (state.player->lives == 0){
            state.player->isActive = false;
        }
    }
    if (!state.enemies[0].isActive ){
        state.nextScene = 3;
    }
    state.enemies[0].Render(program);
    state.map->Render(program);
    state.player->Render(program);
}
