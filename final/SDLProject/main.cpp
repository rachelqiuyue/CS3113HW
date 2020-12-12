#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"
#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "Level5.h"
#include "menu.h"

bool start = false;

Scene *currentScene;
Scene *sceneList[6];
void SwitchToScene(Scene *scene, int lives) {
    currentScene = scene;
    currentScene->Initialize();
    currentScene->state.player->lives= lives;
}

Mix_Music *music;
Mix_Chunk *shoot;

SDL_Window* displayWindow;
bool gameIsRunning = true;
int num = 0;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;
glm::mat4 uiViewMatrix, uiProjectionMatrix;
GLuint fontTextureID;

//struct GameState {
//    Entity *player;
//    Entity *objects;
//    Entity *enemies;
//    std::vector<Entity> ships;
//};
//
//GameState state;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("3D!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 720);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("Energizing.mp3");
    shoot = Mix_LoadWAV("shoot.wav");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    uiViewMatrix = glm::mat4(1.0);
    uiProjectionMatrix = glm::ortho(-6.4f, 6.4f, -3.6f, 3.6f, -1.0f, 1.0f);
    fontTextureID = Util::LoadTexture("font1.png");
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::perspective(glm::radians(45.0f), 1.777f, 0.1f, 100.0f);
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    sceneList[0] = new menu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    sceneList[4] = new Level4();
    sceneList[5] = new Level5();
    SwitchToScene(sceneList[0],3);
    
//    state.player = new Entity();
//    state.player->entityType = PLAYER;
//    state.player->position = glm::vec3(0, 0.25, 0);
//    state.player->acceleration = glm::vec3(0, 0, 0);
//    state.player->speed = 1.0f;
//    state.player->lives = 3;
//    state.player->numShips = 0;
//
//    state.objects = new Entity[OBJECT_COUNT];
//    GLuint floorTextureID = Util::LoadTexture("grass.jpg");
//    state.objects[0].textureID = floorTextureID;
//    state.objects[0].position = glm::vec3(0,-0.25f,0);
//    state.objects[0].entityType = FLOOR;
//    Mesh* cubeMesh = new Mesh();
//    cubeMesh->LoadOBJ("cube.obj", 50);
//    state.objects[0].mesh = cubeMesh;
//    state.objects[0].rotation = glm::vec3(0,0,0);
//    state.objects[0].acceleration = glm::vec3(0,0,0);
//    state.objects[0].scale = glm::vec3(50,0.5f,50);
//
//    Mesh *crateMesh = new Mesh();
//    crateMesh->LoadOBJ("pikachu.obj", 1);
//    GLuint enemyTextureID = Util::LoadTexture("pikachu.png");
//    for (int i = 1; i < OBJECT_COUNT; i++) {
//
//        state.objects[i].position = glm::vec3(rand() % 20 , 0, rand() % 20 -20);
//        state.objects[i].textureID = enemyTextureID;
//        state.objects[i].entityType = CRATE;
//        state.objects[i].mesh = crateMesh;
//        state.objects[i].rotation = glm::vec3(0,0,0);
//    }
//

}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        // Some sort of action
                        break;
                    case SDLK_RETURN:
                        std::cout<<"ok";
                        if (!start){
                            SwitchToScene(sceneList[1],3);
                        }
                        start = true;
                        break;
                }
                break;
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_A]) {
        currentScene->state.player->rotation.y += 1.0f;
    } else if (keys[SDL_SCANCODE_D]) {
        currentScene->state.player->rotation.y -= 1.0f;
    }
    currentScene->state.player->velocity.x = 0;
    currentScene->state.player->velocity.z = 0;
    if (keys[SDL_SCANCODE_W]) {
        currentScene->state.player->velocity.z = cos(glm::radians(currentScene->state.player->rotation.y)) * -2.0f;
        currentScene->state.player->velocity.x = sin(glm::radians(currentScene->state.player->rotation.y)) * -2.0f;
    } else if (keys[SDL_SCANCODE_S]) {
        currentScene->state.player->velocity.z = cos(glm::radians(currentScene->state.player->rotation.y)) * 2.0f;
        currentScene->state.player->velocity.x = sin(glm::radians(currentScene->state.player->rotation.y)) * 2.0f;
    }
    if (keys[SDL_SCANCODE_UP]) {
        if (currentScene->state.player->lives >0){
        Mix_PlayChannel(-1,shoot,0);
        Entity ship = Entity();
        GLuint shipTextureID = Util::LoadTexture("foamBulletA.png");
        Mesh *shipMesh = new Mesh();
        shipMesh->LoadOBJ("foamBulletA.obj", 1);
        ship.position.x = currentScene->state.player->position.x;
        ship.position.y = currentScene->state.player->position.y-0.5;
        ship.position.z = currentScene->state.player->position.z;
        ship.rotation = currentScene->state.player->rotation;
        ship.velocity.z = cos(glm::radians(currentScene->state.player->rotation.y))*-10 ;
        ship.velocity.x = sin(glm::radians(currentScene->state.player->rotation.y))*-10;
        ship.textureID = shipTextureID;
        ship.mesh = shipMesh;
            ship.scale = glm::vec3(2,2,2);
        ship.entityType = SHIP;
        currentScene->state.ships.push_back(ship);
        }
        }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    
    while (deltaTime >= FIXED_TIMESTEP) {
//        state.player->Update(FIXED_TIMESTEP,state.player,state.objects,OBJECT_COUNT);
//        for (int i = 0; i < OBJECT_COUNT;i++){
//            state.objects[i].Update(FIXED_TIMESTEP,state.player,state.objects,OBJECT_COUNT);
//        }
//        for (int i = 0; i < state.ships.size();i++){
//            state.ships[i].Update(FIXED_TIMESTEP,state.player,state.objects,OBJECT_COUNT);
//        }
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;
    
    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::rotate(viewMatrix,
                             glm::radians(currentScene->state.player->rotation.y), glm::vec3(0, -1.0f, 0));
    viewMatrix = glm::translate(viewMatrix, -currentScene->state.player->position);
//    viewMatrix = glm::rotate(viewMatrix,
//                             glm::radians(state.player->rotation.y), glm::vec3(0, -1.0f, 0));
//    viewMatrix = glm::translate(viewMatrix, -state.player->position);
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    if (currentScene->state.player->lives == 0){
        glm::mat4 uiViewMatrix = glm::mat4(1.0);
        glm::mat4 uiProjectionMatrix = glm::ortho(-6.4f, 6.4f, -3.6f, 3.6f, -1.0f, 1.0f);
        program.SetProjectionMatrix(uiProjectionMatrix);
        program.SetViewMatrix(uiViewMatrix);
        Util::DrawText(&program, fontTextureID, "you lose!", 0.5, -0.3f,
                       glm::vec3(0, 0, 0));
    }

//    int enemyCount = 0;
//    for (int i = 0; i < OBJECT_COUNT;i++){
//        state.objects[i].Render(&program);
//        if (state.objects[i].entityType == CRATE && state.objects[i].alive){
//            enemyCount += 1;
//        }
//    }
//    for (int i = 0; i < state.ships.size();i++){
//        state.ships[i].Render(&program);
//    }
//    program.SetProjectionMatrix(uiProjectionMatrix);
//    program.SetViewMatrix(uiViewMatrix);
//    Util::DrawText(&program, fontTextureID, "lives: "+std::to_string(state.player->lives), 0.5, -0.3f,
//                   glm::vec3(-6, 3.2, 0));
//    Util::DrawText(&program, fontTextureID, "enemies: "+std::to_string(enemyCount), 0.5, -0.3f,
//                   glm::vec3(3, 3.2, 0));
    currentScene->Render(&program);
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
        if (currentScene->state.nextScene >= 0){ SwitchToScene(sceneList[currentScene->state.nextScene],currentScene->state.player->lives);
        }
        Render();
    }
    
    Shutdown();
    return 0;
}
