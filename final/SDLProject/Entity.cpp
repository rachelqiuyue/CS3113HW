#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    rotation = glm::vec3(0);
    modelMatrix = glm::mat4(1.0f);
    scale = glm::vec3(1);
    speed = 0.0f;
    width = 1.0f;
    height = 1.0f;
    depth = 1.0f;
    billboard = false;
    lives = 1;
    numShips = 0;
    alive = true;
}

bool Entity::CheckCollision(Entity *other)
{
    if (!alive || !other->alive)return false;
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    float zdist = fabs(position.z - other->position.z) - ((depth + other->depth) / 2.0f);
    if (xdist < 0 && ydist < 0 && zdist < 0) return true;
    return false;
}

void Entity::Update(float deltaTime, Entity *player, Entity *objects, int objectCount)
{
    
    glm::vec3 previousPosition = position;
    if (entityType == CRATE){
        float directionX = player->position.x - position.x;
        float directionZ = player->position.z - position.z;
        rotation.y = glm::degrees(atan2f(directionX,directionZ));
        velocity.z = cos(glm::radians(rotation.y))*1.0f;
        velocity.x = sin(glm::radians(rotation.y))*1.0f;
    }
    if (entityType == SHIP){
//        rotation = glm::vec3(0);
//        velocity.z = velocity.z;
//        velocity.x = sin(glm::radians(player->rotation.y))*-10;
//        acceleration = glm::vec3(0,0,-1);
//        acceleration.x = -sin(glm::radians(player->rotation.y));
        for (int i = 0; i < objectCount; i++)
        {
            // Ignore collisions with the floor
            if (objects[i].entityType == FLOOR || objects[i].entityType == SHIP) continue;
            if (CheckCollision(&objects[i])) {
                objects[i].alive = false;
            }
        }
    }
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    if (entityType == PLAYER){
        for (int i = 0; i < objectCount; i++)
        {
            // Ignore collisions with the floor
            if (objects[i].entityType == FLOOR || objects[i].entityType == SHIP) continue;
            if (CheckCollision(&objects[i])) {
                if (player->lives > 0){
                    position = glm::vec3(0, 0.5, 0);
                    player->lives -= 1;
                    objects[i].position = glm::vec3(rand() % 20 - 10, 0, rand() % 20 - 10);
                }
                break;
            }
        }
    }
    if (entityType == CUBE){
        rotation.y += 45 * deltaTime;
        rotation.z += 45 * deltaTime;
    } else if (entityType == ENEMY){
        rotation.y +=30 * deltaTime;
    }
    
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, scale);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x),glm::vec3(1,0,0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y),glm::vec3(0,1,0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z),glm::vec3(0,0,1));
}

void Entity::DrawBillboard(ShaderProgram *program) {
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram *program) {
    if (!alive) return;
    program->SetModelMatrix(modelMatrix);
    

    glBindTexture(GL_TEXTURE_2D, textureID);
    
//    glVertexAttribPointer(program->positionAttribute, 3, GL_FLOAT, false, 0, vertices);
//    glEnableVertexAttribArray(program->positionAttribute);
//
//    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
//    glEnableVertexAttribArray(program->texCoordAttribute);
//
//    glDrawArrays(GL_TRIANGLES, 0, numVertices);
//
//    glDisableVertexAttribArray(program->positionAttribute);
//    glDisableVertexAttribArray(program->texCoordAttribute);
    glBindTexture(GL_TEXTURE_2D, textureID);
//    if (billboard) {
//        DrawBillboard(program);
//    } else {
        mesh->Render(program);
//    }
}

