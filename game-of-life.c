#include <stdlib.h>
#include <raylib.h>
#include <string.h>
#include <time.h>

#define ROWS 100
#define COLS 120 

typedef enum Window{START, MAIN} Window;

typedef struct Cell{
    bool active;
    Rectangle bounds;
}Cell;


bool* evaluate(Cell grid[ROWS][COLS], int row, int col){
    bool* result = malloc(3 * sizeof(bool));
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
    result[0] = count < 2;
    result[1] = count > 3;
    result[2] = count == 3;
    return result;
}

int main(){
    SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_MSAA_4X_HINT);
    InitWindow(0, 0,"grid");
    SetTargetFPS(60);

    Window window = START;
    const float screenWidth = GetScreenWidth();
    const float screenHeight= GetScreenHeight();
    const float cellWidth = screenWidth / COLS;
    const float cellHeight= screenHeight/ ROWS;
    Cell grid[ROWS][COLS];
    Cell nextGrid[ROWS][COLS];
    bool state; 
    bool pause = true;
    bool reset = false;
    int fpsCounter = 0;
    srand(time(NULL));
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLS; col++){
            state = false; 
            //state = (rand() % 2) ? true : false;
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
        if(IsKeyPressed('R')){
            reset = true;
            window = START;
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            int posX = GetMouseX() / cellWidth;
            int posY = GetMouseY() / cellHeight;
            if(posX >= 0 && posX <  COLS && posY >= 0 && posY < ROWS){
                grid[posY][posX].active = !grid[posY][posX].active;
            }
        }
        if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
            int posX = GetMouseX() / cellWidth;
            int posY = GetMouseY() / cellHeight;
            if(posX >= 0 && posX <  COLS && posY >= 0 && posY < ROWS){
                grid[posY][posX].active = !grid[posY][posX].active;
            }
        }
        if(IsKeyPressed(KEY_SPACE)){
            pause = true;
        }
        if(pause){
            window = START;
            pause = false;
        }
        switch(window){
            case START:
                if(IsKeyPressed(KEY_ENTER)){
                    window = MAIN;
                }
                break;
            case MAIN:
                if(fpsCounter >= 20){
                    for(int row = 0; row < ROWS; row++){
                        for(int col = 0; col < COLS; col++){
                            nextGrid[row][col] = grid[row][col];
                            if(grid[row][col].active){
                                if(evaluate(grid, row, col)[0] || evaluate(grid, row, col)[1])
                                    nextGrid[row][col].active = false;
                            }else{
                                if(evaluate(grid, row, col)[2])
                                    nextGrid[row][col].active = true;
                            }
                        }
                    }
                    memcpy(grid, nextGrid, sizeof(grid));
                    fpsCounter = 0;
                }
                break;
        }
        for(int row = 0; row < ROWS; row++){
            for(int col = 0; col < COLS; col++){
                if(reset)
                    grid[row][col].active = false;
                Color cellColor = grid[row][col].active ? BLACK: BEIGE;
                DrawRectangleRec(grid[row][col].bounds, cellColor);
                DrawRectangleLinesEx(grid[row][col].bounds, 0.3f, BLACK);
            }
        }
        reset = false;
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
