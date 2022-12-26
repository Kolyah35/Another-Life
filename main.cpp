#include "includes.h"
#include "Player.h"
#include "Laser.h"
#include "Enemy.h"

int score;

struct Animation {
    uint32_t animTime = 0, animDuration;
    float animStart, animFinal, animCurrent;
    bool played = false; 
};

struct Path {
    uint16_t pathwidth = screenWidth / 15;
    Rectangle path_rec = {(float)((screenWidth / 2) - (pathwidth / 2)), 0, (float)pathwidth, (float)screenHeight}; // Хитбокс дороги
};


//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
bool showhitbox = false; // Показывать хитбоксы?
Color bgcolor = DARKGREEN; // Цвет бг
vector<Laser*> lasers; // Массив из лазеров
vector<Enemy> enemys; // Массив из врагов

//----------------------------------------------------------------------------------
// Main Enry Point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    raylib::Window window(screenWidth, screenHeight, "Another Life");
    //raylib::Image icon("resources/icon.png");
    //window.SetIcon(icon);
    //SetWindowIcon(icon);
    InitAudioDevice();

    //Textures
    raylib::Texture2D bg("resources/bg.png");
    raylib::Texture2D path("resources/path.png");
    raylib::Texture2D laserturrel("resources/laserturrel.png");
    raylib::Texture2D laser("resources/laser.png");
    raylib::Texture2D playerspr("resources/player.png");
    raylib::Texture2D shield("resources/shield.png");
    raylib::Texture2D overlay("resources/overlay.png");
    raylib::Texture2D sword("resources/sword.png");
    raylib::Texture2D enemy("resources/enemy.png");
    raylib::Sound death("resources/crash.ogg");
    raylib::Sound click("resources/click.ogg");
    Music music = LoadMusicStream("resources/AnotherLife.mp3");

    PlayMusicStream(music);

    //Vars
    score = 0;
    Player* player = new Player();
    Path path_s;
    uint32_t frames = 0, frames1 = 0, attackCount = 0, random = GetRandomValue(60, 120); // Счетчик кадров
    Animation ydanim;
    ydanim.animCurrent = 1.0f;
    ydanim.animDuration = 50;
    ydanim.animStart = 1.0f;
    ydanim.animFinal = 0.0f;
    string ydtext = "You dead";
    player->updateShield(); // Обновляем щит

    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    playerspr.SetFilter(TEXTURE_FILTER_TRILINEAR);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        frames++; frames1++; // Увеличиваем счетчик (1 сек = 60 кадров)
        UpdateMusicStream(music);
        if(frames >= random){ // Каждые 1-2 сек спавним лазер или врага (рандом)
            switch(GetRandomValue(0, 1)){
                case 0: {
                    Laser* temp = new Laser();
                    lasers.push_back(temp);
                    cout << "laser created with id " << lasers.size() << endl;
                }

                case 1: {
                    Enemy temp_enemy;
                    enemys.push_back(temp_enemy);
                    cout << "Enemy created with id " << enemys.size() << endl;
                }
            }
            frames = 0;
            random = GetRandomValue(60, 120);
        }

        switch (GetKeyPressed()){
            case KEY_LEFT: {
                player->shield.side = Side::LEFT;
                player->shield.hitbox.x = player->hitbox.x - 160;
                player->updateShield();
                break;
            }
            
            case KEY_RIGHT: {
                player->shield.side = Side::RIGHT;
                player->shield.hitbox.x = player->hitbox.x + 160 + player->hitbox.width;
                player->updateShield();
                break;
            }

            case KEY_SPACE: {
                if(!player->attacking){
                    player->attacking = true;
                    PlaySound(click);
                }
                attackCount++;
                cout << attackCount << endl;
                break;
            }

            case KEY_F3: {
                showhitbox = !showhitbox;
            }
            default: {
                break;
            }
        }

        if(frames1 > 60 * 2){
            frames1 = 0;
            attackCount = 0;
        }

        if(frames1 < 60 * 2 && attackCount > 5){
            attackCount = 0;
            frames1 = 0;
            ydtext = "Spam detected!!!";
            player->isDead = true;
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);
            //BG
            DrawTexturePro(bg, {0, 0, (float)window.GetWidth(), (float)window.GetHeight()}, {0, 0, (float)window.GetWidth(), (float)window.GetHeight()}, {0, 0}, 0, bgcolor);
            DrawTexturePro(shield, {0, 100, (float)shield.GetWidth(), 16}, {path_s.path_rec.x - 30, 0, path_s.path_rec.width + 60, path_s.path_rec.height}, {0, 0}, 0, Fade(DARKGREEN, 0.75));
            DrawTexturePro(path, {0, 0, (float)path.GetWidth(), (float)path.GetHeight()}, path_s.path_rec, {0, 0}, 0, WHITE); //Рисуем дорогу
            DrawTexturePro(sword, {0, 0, (float)sword.GetWidth(), (float)sword.GetHeight()}, {player->sword.hitbox.x + (player->sword.hitbox.width / 2), player->sword.hitbox.y, player->sword.hitbox.height / 2, player->sword.hitbox.height}, {(player->sword.hitbox.height / 2) / 2, 0}, 0, WHITE);

            if(!player->isDead){
                // Enemyes
                for(int i = 0; i < enemys.size(); i++){
                    //printf("i: %i | size: %i\n", i, enemys.size());
                    DrawTexturePro(enemy, {0, 0, (float)enemy.GetWidth(), (float)enemy.GetHeight()}, enemys[i].hitbox, {0, 0}, 0, Fade(WHITE, enemys[i].alpha));
                    
                    if(showhitbox) DrawRectangleLinesEx(enemys[i].hitbox, 2, GREEN);

                    enemys[i].update(*&player);

                    if(enemys[i].hitbox.y > screenHeight + 100 || (enemys[i].dead && enemys[i].alpha == 0.0f)){
                        enemys.erase(enemys.begin() + i);
                        cout << "Deleted enemy with id " << i << endl;
                    }
                }

                //Lasers
                for(int i = 0; i < lasers.size(); i++){
                    //printf("i: %i | size: %i\n", i, lasers.size());
                    DrawTexturePro(laser, {0, 0, (float)laser.GetWidth(), (float)laser.GetHeight()}, {lasers[i]->hitbox.x, lasers[i]->hitbox.y + (float)(lasers[i]->hitbox.height / 2), lasers[i]->hitbox.width, lasers[i]->hitbox.height * 4}, {0, (float)(lasers[i]->hitbox.height * 2)}, 0, Fade(WHITE, 0.5));
                    DrawTexturePro(laser, {0, 0, (float)laser.GetWidth(), (float)laser.GetHeight()}, {lasers[i]->hitbox.x, lasers[i]->hitbox.y, lasers[i]->hitbox.width, lasers[i]->hitbox.height * 2}, {0, lasers[i]->hitbox.height / 2}, 0, WHITE);
                    
                    //Draw laser
                    if(lasers[i]->fliped){
                        DrawTexturePro(laserturrel, {0, 0, -(float)laserturrel.GetWidth(), (float)laserturrel.GetHeight()}, {screenWidth - 225, lasers[i]->hitbox.y + (lasers[i]->hitbox.height / 2), (float)laserturrel.GetWidth() / 2, (float)laserturrel.GetHeight() / 2}, {10, ((float)laserturrel.GetHeight() / 2) / 2}, 0, WHITE);
                    }else{
                        DrawTexturePro(laserturrel, {0, 0, (float)laserturrel.GetWidth(), (float)laserturrel.GetHeight()}, {lasers[i]->hitbox.x, lasers[i]->hitbox.y + (lasers[i]->hitbox.height / 2), (float)laserturrel.GetWidth() / 2, (float)laserturrel.GetHeight() / 2}, {10, ((float)laserturrel.GetHeight() / 2) / 2}, 0, WHITE);
                    }

                    if(showhitbox) DrawRectangleLinesEx(lasers[i]->hitbox, 2, GREEN);

                    lasers[i]->update(player);

                    if(lasers[i]->hitbox.y > screenHeight + 100){
                        lasers.erase(lasers.begin() + i);
                        cout << "Deleted laser with id " << i << endl;
                    }
                }
                //Player
                DrawTexturePro(playerspr, {0, 0, (float)playerspr.GetWidth(), (float)playerspr.GetHeight()}, player->hitbox, {0, 0}, 0, WHITE);
                DrawTexturePro(shield, {0, 0, (float)shield.GetWidth(), (float)shield.GetHeight()}, player->shield.texture, {0, 0}, 0, Fade(DARKGREEN, 0.75));
                DrawRectangleRec(player->shield.hitbox, GRAY);
                if(showhitbox){
                    DrawRectangleLinesEx(player->hitbox, 2, GREEN);
                    DrawRectangleLinesEx(player->shield.hitbox, 2, GREEN);
                    DrawRectangleLinesEx(player->sword.hitbox, 2, GREEN);
                }
                //------
                player->updateSword();
            }else{
                PlaySound(death);
                lasers.clear();
                enemys.clear();
                score = 0;
                ydanim.animCurrent = 1.0f;
                ydanim.played = true;
                player->isDead = false;
            }
            DrawTexturePro(overlay, {0, 0, (float)overlay.GetWidth(), (float)overlay.GetHeight()}, {0, 0, (float)window.GetWidth(), (float)window.GetHeight()}, {0, 0}, 0, WHITE);
            //Анимация текста
            if(ydanim.played){
                if(ydanim.animCurrent > ydanim.animFinal){
                    ydanim.animCurrent = EaseLinearIn(ydanim.animTime, ydanim.animStart, ydanim.animFinal - ydanim.animStart, ydanim.animDuration);
                    if(ydanim.animTime >= ydanim.animDuration){
                        ydanim.animTime = 0;
                        ydtext = "You dead";
                        ydanim.played = false;
                    }else{
                        ydanim.animTime++;
                    }
                }
                Vector2 ydtextsize = MeasureTextEx(GetFontDefault(), ydtext.c_str(), 96, 1);
                DrawText(ydtext.c_str(), (screenWidth / 2) - (ydtextsize.x / 2), screenHeight / 3, 96, Fade(WHITE, ydanim.animCurrent));
            }
            if(showhitbox) DrawFPS(0, 0);
            DrawText(TextFormat("Score: %i", score), 5, 30, 30, WHITE);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    UnloadTexture(bg);
    UnloadTexture(path);
    UnloadTexture(laserturrel);
    UnloadTexture(laser);
    UnloadTexture(playerspr);
    UnloadTexture(shield);
    UnloadTexture(overlay);
    UnloadTexture(sword);
    UnloadTexture(enemy);
    UnloadSound(death);
    UnloadSound(click);
    UnloadMusicStream(music);

    return 0;
}
