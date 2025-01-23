#include "raylib.h"

#define NUMBER_BLOCK 16

typedef enum GameScreen {HOME, MAIN} GameScreen;

typedef struct Ball{
    Vector2 position;
    Vector2 speed;
    float radius;
    bool moving;
} Ball;

typedef struct Block{
    Rectangle bounds;
    bool active;
} Block;

typedef struct Player{
    Rectangle bounds; 
    float speed;
} Player;

Vector2 Reflect(Vector2 speed,Vector2 normal){
    float dot = speed.x * normal.x + speed.y * normal.y;   
    return (Vector2){
        speed.x - 2 * dot * normal.x,
        speed.y - 2 * dot * normal.y
    };
}

int main(){
    const char* text = "Press Enter to start the Demo";
    const float screenWidth  = 1300.0f;
    const float screenHeight = 700.0f;
    GameScreen currentScreen = HOME;
    int textWidth;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "test"); 
    SetTargetFPS(60);

    Ball ball;
    Player player;
    Block block[100];

    ball.position = (Vector2) {1000.0f, 500.0f};
    ball.speed = (Vector2) {7.0f, 7.0f};
    ball.radius = 20.0f;
    ball.moving = true;

    player.speed = 20.0f;
    player.bounds.width = 70.0f;
    player.bounds.height = 35.0f;
    player.bounds.x =  screenWidth / 2 - player.bounds.width / 2;
    player.bounds.y =  600.0f;

    for(int i = 0; i < NUMBER_BLOCK; i++){
        if(i < NUMBER_BLOCK / 2){
            block[i].bounds.x = screenWidth / 2 + (i % (NUMBER_BLOCK / 2) * 70 + 30);
        }else{
            block[i].bounds.x = screenWidth / 2 - (i % (NUMBER_BLOCK / 2) * 70 + 30);
        }
        block[i].bounds.y = 50.0f;
        block[i].bounds.width = 50.0f;
        block[i].bounds.height = 50.0f;
        block[i].active = true;
    }

    while(!WindowShouldClose()){
        switch(currentScreen){
            case HOME: 
                if(IsKeyPressed(KEY_ENTER)){
                    currentScreen = MAIN;
                }
                break; 
            case MAIN: 
                if(ball.moving){
                    ball.position.x += ball.speed.x;
                    ball.position.y += ball.speed.y;
                }
                if(IsKeyDown('D') && player.bounds.x + player.speed + player.bounds.width < screenWidth){
                    player.bounds.x += player.speed;
                }else if(IsKeyDown('D') && player.bounds.x + player.speed + player.bounds.width >= screenWidth){
                    player.bounds.x += player.speed - ((player.bounds.x + player.bounds.width + player.speed) - screenWidth) - 2;
                }
                if(IsKeyDown('A') && player.bounds.x - player.speed > 0){
                    player.bounds.x -= player.speed;
                }else if(IsKeyDown('A') && player.bounds.x - player.speed <= 0){
                    player.bounds.x -= (player.speed + (player.bounds.x - player.speed)) - 2;
                }
                for(int i = 0; i < NUMBER_BLOCK; i++){
                    if(CheckCollisionCircleRec(ball.position, ball.radius, block[i].bounds)){
                        block[i].active = false;
                    }
                    if(IsKeyPressed(KEY_ENTER)){
                        block[i].active = true;
                    }
                }
                if(ball.position.x + ball.radius >= screenWidth || ball.position.x - ball.radius <= 0){
                    ball.speed.x = -ball.speed.x;
                }if(ball.position.y + ball.radius >= screenHeight || ball.position.y - ball.radius <= 0){
                    ball.speed.y = -ball.speed.y;
                }
                break;
        }
        BeginDrawing();        
        ClearBackground(RAYWHITE);  
        switch(currentScreen){
            case HOME: 
                textWidth = MeasureText(text, 20);
                DrawText(text ,screenWidth / 2 - (int)(textWidth / 2), screenHeight / 2, 20, BLACK);
                break;
            case MAIN:
                DrawCircleV(ball.position, ball.radius, RED);
                DrawRectangleRounded(player.bounds, 0.2f, 20, PURPLE);
                for(int i = 0; i < NUMBER_BLOCK; i++){
                    if(block[i].active){
                        DrawRectangleRec(block[i].bounds, BLACK); 
                    }
                }
                break;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
