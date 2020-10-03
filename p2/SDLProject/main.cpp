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


SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix,modelMatrix2,modelMatrix3, projectionMatrix;


float player_speed = 2.0f;

float direction_x = 1.0f;
float direction_y = 1.0f;

bool start = false;

glm::vec3 player_position = glm::vec3(4.8, 0, 0);
// Don’t go anywhere (yet).
glm::vec3 player_movement = glm::vec3(0, 0, 0);

glm::vec3 player_position2 = glm::vec3(-4.8, 0, 0);
// Don’t go anywhere (yet).
glm::vec3 player_movement2 = glm::vec3(0, 0, 0);

glm::vec3 ball_position = glm::vec3(0, 0, 0);
// Don’t go anywhere (yet).
glm::vec3 ball_movement = glm::vec3(0, 0, 0);



void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("hw2!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}


void ProcessInput() {
    player_movement = glm::vec3(0);
    player_movement2 = glm::vec3(0);
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // Move the player left
                        break;
                    case SDLK_RIGHT:
                        // Move the player right
                        break;
                    case SDLK_SPACE:
                        // Some sort of action
                        start = true;
                        break;
                }
                break;
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_UP]) {
        player_movement.y = 1.0f;
    }
    if (keys[SDL_SCANCODE_DOWN]) {
        player_movement.y = -1.0f;
    }
    if (keys[SDL_SCANCODE_W]) {
        player_movement2.y = 1.0f;
    }
    if (keys[SDL_SCANCODE_S]) {
        player_movement2.y = -1.0f;
    }
    if (glm::length(ball_movement) > 1.0f) {
        ball_movement = glm::normalize(ball_movement);
    }
}
float lastTicks = 0.0f;

bool isCollision(){
    float xdist = fabs(player_position.x - ball_position.x) - ((0.4f + 0.4f) / 2.0f);
    float ydist = fabs(player_position.y - ball_position.y) - ((2.0f + 0.4f) / 2.0f);
    float xdist2 = fabs(player_position2.x - ball_position.x) - ((0.4f + 0.4f) / 2.0f);
    float ydist2 = fabs(player_position2.y - ball_position.y) - ((2.0f + 0.4f) / 2.0f);
    return (xdist<=0 && ydist<=0) || (xdist2 <=0 && ydist2 <=0);
}

bool outOfBox(){
    return ball_position.x >= 4.8f || ball_position.x <= -4.8f;
}

bool ishit(){
    return ball_position.y >= 3.55f || ball_position.y <= -3.55f;
}


void Update() {
    if (!outOfBox()){
        float ticks = (float)SDL_GetTicks() / 1000.0f;
        float deltaTime = ticks - lastTicks;
        lastTicks = ticks;
        // Add (direction * units per second * elapsed time)
        player_position += player_movement * player_speed * deltaTime;
        player_position2 += player_movement2 * player_speed * deltaTime;
        if (player_position.y < -2.75f ){
            player_position.y = -2.75f;
        }
        if (player_position.y > 2.75f){
            player_position.y = 2.75f;
        }
        if (player_position2.y < -2.75f ){
            player_position2.y = -2.75f;
        }
        if (player_position2.y > 2.75f){
            player_position2.y = 2.75f;
        }
        
        if (start){
            ball_position.x += direction_x * player_speed * deltaTime;
            ball_position.y += direction_y * player_speed * deltaTime;
        }
        if (isCollision()){
            direction_x = -direction_x;
        }
        if (ishit()){
            direction_y = -direction_y;
        }
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, ball_position);
    }
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetModelMatrix(modelMatrix);
    float vertices_s[] = {  -0.2f, -0.2f,   0.2f, -0.2f,   0.2f, 0.2f,   -0.2f, -0.2f,   0.2f, 0.2f,   -0.2f, 0.2f};
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices_s);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
    
    float vertices_p[] = { 0.2f, 1.0f, 0.2f, -1.0f, -0.2f, -1.0f,0.2f, 1.0f,-0.2f, -1.0f,-0.2f,1.0f };
    
    modelMatrix2 = glm::mat4(1.0f);
    modelMatrix2 = glm::translate(modelMatrix2,glm::vec3(player_position.x,player_position.y,0.0f));

    program.SetModelMatrix(modelMatrix2);

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices_p);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
    

    modelMatrix3 = glm::mat4(1.0f);
        modelMatrix3 = glm::translate(modelMatrix3,glm::vec3(player_position2.x,player_position2.y,0.0f));
    program.SetModelMatrix(modelMatrix3);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices_p);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
    
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}

