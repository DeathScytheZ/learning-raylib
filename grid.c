#include <stdlib.h>
#include <raylib.h>
#include <time.h>

#define ROWS 30
#define COLS 40

typedef struct Cell{
    bool active;
    Color color;
    Rectangle bounds;
}Cell;

int main(){
    SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_MSAA_4X_HINT);
    InitWindow(0, 0,"grid");
    SetTargetFPS(60);

    const float screenWidth = GetScreenWidth();
    const float screenHeight= GetScreenHeight();
    const float cellWidth = screenWidth / COLS;
    const float cellHeight= screenHeight/ ROWS;
    Cell grid[ROWS][COLS];

    bool state; 
    bool randomize = false;
    int fpsCounter = 0;
    srand(time(NULL));
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLS; col++){
            state = true; 
            state = (rand() % 2) ? true : false;
            grid[row][col].active = state;    
            grid[row][col].bounds.width = cellWidth;    
            grid[row][col].bounds.height= cellHeight;    
            grid[row][col].bounds.x = col * grid[row][col].bounds.width;    
            grid[row][col].bounds.y = row * grid[row][col].bounds.height;    
        }
    }
    while(!WindowShouldClose()){
        ClearBackground(RAYWHITE);
        BeginDrawing();
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            int posX = GetMouseX() / cellWidth;
            int posY = GetMouseY() / cellHeight;
            if(posX >= 0 && posX <=  COLS && posY >= 0 && posY <= ROWS){
                grid[posY][posX].active = !grid[posY][posX].active;
            }
        }
        fpsCounter++;
        if(fpsCounter == 30){
            randomize = true;    
        }
        for(int row = 0; row < ROWS; row++){
            for(int col = 0; col < COLS; col++){
                if(randomize){
                    grid[row][col].active = (rand() % 2);
                    fpsCounter = 0;
                }
                Color cellColor = grid[row][col].active ? RAYWHITE : BLACK;
                DrawRectangleRec(grid[row][col].bounds, cellColor);
                DrawRectangleLinesEx(grid[row][col].bounds, 1.0f, BEIGE);
            }
        }
        randomize = false;
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
