#pragma once
#include "includes.h"
#include "Player.h"

class Laser {
    public:
    Side side;
    Rectangle hitbox;
    bool fliped = false;
    bool skiped = false;

    Laser();
    void update(Player* player);
};