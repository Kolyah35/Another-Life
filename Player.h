#pragma once
#include "includes.h"

struct Shield {
    Side side;
    Rectangle hitbox, texture;
    uint8_t shadowoffset = 16;
};

struct Sword {
    Rectangle hitbox;
    int16_t startPositionY, finalPositionY;
    uint16_t attackTime = 0, duration = 30;
};

class Player {
public:
    Vector2 playerhw;
    Rectangle hitbox;
    bool isDead = false;
    bool attacking = false;

    Shield shield;
    Sword sword;

    Player();
    void updateShield();
    void updateSword();
    Rectangle getShieldHitbox();
};