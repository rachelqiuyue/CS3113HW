//
//  Level1.cpp
//  SDLProject
//
//  Created by QIUYUE WANG on 11/14/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include <stdio.h>

#include "menu.h"

void menu::Initialize() {
    state.nextScene = -1;
    state.enemies = NULL;
    state.map = NULL;
    state.player = new Entity();
}
void  menu::Update(float deltaTime) {

}
void menu::Render(ShaderProgram *program) {
        GLuint fontTexture = Util::LoadTexture("pixel_font.png");
    Util::DrawText(program, fontTexture, "Beat all monsters", 0.3,
                   0.05, glm::vec3(1,-3,0));
        Util::DrawText(program, fontTexture, "Press Enter to start", 0.2,
                       0.05, glm::vec3(1,-5,0));
}

