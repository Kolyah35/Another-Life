#include "Laser.h"

Laser::Laser(){
    if(GetRandomValue(0, 1) == 0){
        this->hitbox = {225, -105, screenWidth - 225, 10};
        this->side = LEFT;
    }else{
        this->hitbox = {0, -105, screenWidth - 225, 10};
        this->side = RIGHT;
        this->fliped = true;
    }
}

void Laser::update(Player* player){
    //Player collisions
    if(CheckCollisionRecs(this->hitbox, player->hitbox)){
        cout << "his dead\n";
        player->isDead = true;
    }

    // Shield collisions
    if(CheckCollisionRecs(this->hitbox, player->shield.hitbox)){
        if(this->side == LEFT){
            this->hitbox.width = player->shield.hitbox.x - (this->hitbox.x - player->shield.hitbox.width);
        }else{
            this->hitbox.x = player->shield.hitbox.x;
            this->hitbox.width = screenWidth - player->shield.hitbox.x - 220;
        }
    }else{
        if(this->side == LEFT){
            this->hitbox.width = screenWidth - this->hitbox.x;
        }else{
            this->hitbox.x = 0;
            this->hitbox.width = screenWidth - 225;
        }
    }

    if(this->hitbox.y > player->hitbox.y && !this->skiped){
        score++;
        this->skiped = true;
    }
    this->hitbox.y += speed;
}