#pragma once
#include "includes.h"
#include "Player.h"

class Enemy {
public:
    Rectangle hitbox;
    uint16_t colidedtime = 0, animationTime = 0, animationDuration = 30;
    float alpha = 1.0f, animationStart = 1.0f, animationFinal = 0.0f;
    bool colided = false, dead = false, scoregiven = false;

    Enemy();
    void update(Player* player);
};