#include <iostream>
#include <raylib.h>

using namespace std;

int main () {

    const int SCREEN_WIDTH = 500;
    const int SCREEN_HEIGHT = 500;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Blackjack");
    SetTargetFPS(60);

    while (WindowShouldClose() == false){
   
       BeginDrawing();
       ClearBackground(DARKGREEN);
       EndDrawing();
    
    }

    CloseWindow();
}