#include <stdlib.h>
#include <raylib.h>
#include <string.h>
#include <time.h>

#define ROWS 36
#define COLS 36

typedef struct Cell{
    bool active;
    Color color;
    Rectangle bounds;
}Cell;

bool underpopulated(Cell grid[ROWS][COLS], int row, int col){
    int count = 0; 
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(i == 0 && j == 0) continue;
            if (row + i >= 0 && row + i < ROWS && col + j>= 0 && col + j < COLS){
                if(grid[row + i][col + j].active){
                    count++;
                }
            }
        }    
    } 
    return count < 2;
}

bool overpopulated(Cell grid[ROWS][COLS], int row, int col){
    int count = 0;
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(i == 0 && j == 0) continue; 
            if(row + i >= 0 && row + i < ROWS && col + j >= 0 && col + j < COLS){
                if(grid[row + i][col + j].active){
                    count ++;
                }
            }
        }
    }
    return count > 3;
}

bool reproduce(Cell grid[ROWS][COLS], int row, int col){
    int count = 0;
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(i == 0 && j == 0) continue;
            if(row + i  >= 0 && row + i < ROWS && col + j >= 0 && col + j < COLS){
                if(grid[row + i][col + j].active)
                   count++;
            }
        }
    }
    return count == 3;
}

int main(){
    SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_MSAA_4X_HINT);
    InitWindow(0, 0,"grid");
    SetTargetFPS(60);

    const float screenWidth = GetScreenWidth();
    const float screenHeight= GetScreenHeight();
    const float cellWidth = screenWidth / COLS;
    const float cellHeight= screenHeight/ ROWS;
    Cell grid[ROWS][COLS];
    Cell nextGrid[ROWS][COLS];
    bool state; 
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
        fpsCounter++;
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            int posX = GetMouseX() / cellWidth;
            int posY = GetMouseY() / cellHeight;
            if(posX >= 0 && posX <  COLS && posY >= 0 && posY < ROWS){
                grid[posY][posX].active = !grid[posY][posX].active;
            }
        }
        if(fpsCounter >= 0){
            for(int row = 0; row < ROWS; row++){
                for(int col = 0; col < COLS; col++){
                    nextGrid[row][col] = grid[row][col];
                    if(grid[row][col].active){
                        if(underpopulated(grid, row, col) || overpopulated(grid, row, col))
                            nextGrid[row][col].active = false;
                    }else{
                        if(reproduce(grid, row, col))
                           nextGrid[row][col].active = true;
                    }
                }
            }
            memcpy(grid, nextGrid, sizeof(grid));
            fpsCounter = 0;
        }
        for(int row = 0; row < ROWS; row++){
            for(int col = 0; col < COLS; col++){
                Color cellColor = grid[row][col].active ? BLACK : RAYWHITE;
                DrawRectangleRec(grid[row][col].bounds, cellColor);
                DrawRectangleLinesEx(grid[row][col].bounds, 1.0f, GRAY);
            }
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
