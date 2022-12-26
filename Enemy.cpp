#include "Enemy.h"

Enemy::Enemy(){
    this->hitbox = {(screenWidth / 2) - 32, -64, 64, 64};
}

void Enemy::update(Player* player){
    if(!this->dead){
        if(CheckCollisionRecs(this->hitbox, player->sword.hitbox)){
            this->colided = true;
        }

        if(this->colided && player->attacking){
            this->dead = true;
        }

        if(this->colided){
            if(this->colidedtime > 30) player->isDead = true;
            this->hitbox.y = player->sword.hitbox.y - 64;
            this->colidedtime++;
        }else{
            this->hitbox.y += speed;
        }
    }else{
        if(this->alpha > this->animationFinal){
            this->alpha = EaseLinearIn(this->animationTime, this->animationStart, this->animationFinal - this->animationStart, this->animationDuration);
            this->animationTime++;
        }

        if(!this->scoregiven){
            score += 2;
            this->scoregiven = true;
        }
    }
}