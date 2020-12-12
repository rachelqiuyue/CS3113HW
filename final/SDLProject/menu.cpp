#include <stdio.h>

#include "menu.h"

void menu::Initialize() {
    state.nextScene = -1;
    state.objects = NULL;
    state.player = new Entity();
    
    
}
void  menu::Update(float deltaTime) {
    
}
void menu::Render(ShaderProgram *program) {
    glm::mat4 uiViewMatrix = glm::mat4(1.0);
    glm::mat4 uiProjectionMatrix = glm::ortho(-6.4f, 6.4f, -3.6f, 3.6f, -1.0f, 1.0f);
    program->SetProjectionMatrix(uiProjectionMatrix);
    program->SetViewMatrix(uiViewMatrix);
    GLuint fontTexture = Util::LoadTexture("font1.png");
    Util::DrawText(program, fontTexture, "Killing enemies", 0.3,
                   0.05, glm::vec3(-5,3,0));
    Util::DrawText(program, fontTexture, "Press Enter to start", 0.3,
                   0.05, glm::vec3(-3,1,0));
    Util::DrawText(program, fontTexture, "Press UP to shoot", 0.3,
                   0.05, glm::vec3(-1,-1,0));
}
