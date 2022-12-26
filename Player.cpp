#include "Player.h"

Player::Player(){

    this->hitbox = {
        (float)((screenWidth / 2) - (64 / 2)),
        (float)((screenHeight - 200) - (64 / 2)),
        64,
        64
    };

    this->shield.side = LEFT;
    this->shield.hitbox = {this->hitbox.x - 160, (float)(this->hitbox.y - (this->hitbox.height / 1.5) + (this->hitbox.height / 3)), 10, (float)(this->hitbox.height * 1.5)};
    this->sword.hitbox = {this->hitbox.x + (this->hitbox.width / 2) - 6, this->hitbox.y - 25, 12, 50};
    this->sword.startPositionY = this->hitbox.y - 25;
    this->sword.finalPositionY = this->hitbox.y - 60;
}

void Player::updateShield(){
    this->shield.texture.x = this->shield.hitbox.x - (this->shield.shadowoffset * 1.1);
    this->shield.texture.y = this->shield.hitbox.y - (this->shield.shadowoffset * 1.1);
    this->shield.texture.width = this->shield.hitbox.width + (this->shield.shadowoffset * 2.1);
    this->shield.texture.height = this->shield.hitbox.height + (this->shield.shadowoffset * 2.1);
}

void Player::updateSword(){
    if(this->attacking){
        if(this->hitbox.y > this->sword.finalPositionY){
            this->sword.hitbox.y = EaseSineInOut(this->sword.attackTime, this->sword.startPositionY, this->sword.finalPositionY - this->sword.startPositionY, this->sword.duration);
            if(this->sword.attackTime >= this->sword.duration * 2){
                this->attacking = false;
                this->sword.attackTime = 0;
            }
            this->sword.attackTime++;
        }
    }
}

Rectangle Player::getShieldHitbox(){
    return this->shield.hitbox;
}