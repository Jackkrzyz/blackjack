#include <iostream>
#include <raylib.h>
#include <algorithm> 
using namespace std;

class Card {
    public:
        int value; // A = 1, 2 = 2..., 11 = J, 12 = Q, 13 = K
        int suit; // 1 = Hearts, 2= diamonds, 3 = clubs, 4 = spades
        int x;
        int y;
        bool faceUp;
        Card(int v, int s, bool f) {
            value = v;
            suit = s;
            faceUp = f;
        }
        void draw() {
            if (faceUp) {
                DrawRectangle(x, y, 50, 100, WHITE);
                if (suit == 1) {
                    DrawText("♥", x + 10, y + 10, 20, RED);
                } else if (suit == 2) {
                    DrawText("♦", x + 10, y + 10, 20, RED);
                } else if (suit == 3) {
                    DrawText("♣", x + 10, y + 10, 20, BLACK);
                } else if (suit == 4) {
                    DrawText("♠", x + 10, y + 10, 20, BLACK);
                }
                if (value == 13) {
                    DrawText("K", x + 10, y + 20, 20, BLACK);
                }
                else if(value == 12)
                {
                    DrawText("Q", x + 10, y + 20, 20, BLACK);
                }
                else if(value == 11)
                {
                    DrawText("J", x + 10, y + 20, 20, BLACK);
                }
                else {
                    DrawText(TextFormat("%i", value), x + 10, y + 20, 20, BLACK);
                }
            } else {
                DrawRectangle(x, y, 50, 100, DARKGRAY);
            }
        }

};

class Deck {
    public:
        Card* cards[52];
        Deck() {
            int index = 0;
            for (int i = 1; i <= 4; i++) { // suit
                for (int j = 1; j <= 13; j++) { // value
                    cards[index] = new Card(j, i, true);
                    index++;
                }
            }
        }
        void shuffle() {
            std::random_shuffle(cards, cards + 52);
        }
        
};

class Player {
    public:
        Card* hand[11];
        int score = 0;
        int cardsReceived = 0;
        bool bust = false;
        void addCard(Card* c) {
            hand[cardsReceived] = c;
            switch (c->value)
            {
                case 1:
                    if (score + 11 <= 21) {
                        score += 11;
                    }
                    else {
                        score += 1;
                    }
                    break;
                case 2:
                    score += 2;
                    break;
                case 3:
                    score += 3;
                    break;
                case 4:
                    score += 4;
                    break;
                case 5:
                    score += 5;
                    break;
                case 6:
                    score += 6;
                    break;
                case 7:
                    score += 7;
                    break;
                case 8:
                    score += 8;
                    break;
                case 9:
                    score += 9;
                    break;                          
                case 10:
                    score += 10;
                    break;
                case 11:
                    score += 10;
                    break;
                case 12:
                    score += 10;
                    break;
                case 13:
                    score += 10;
                    break;
            }
            if (score > 21) {
                bust = true;
            }
            cardsReceived++;
        }
};




int main () {
    
    const int SCREEN_WIDTH = 500;
    const int SCREEN_HEIGHT = 500;

    int minimumBet = 15;
    int betIncrement = 10;
    int balance = 2000;
    int bet = 15;
    bool betting = true;
    

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Blackjack");
    SetTargetFPS(60);

    Player player;
    Player dealer;
    Deck deck;
    Card* cardsInPlay[22];
    deck.shuffle();
    

    // Main Game loop
    while (WindowShouldClose() == false && balance > 0 && !IsKeyDown(KEY_DELETE)){
        BeginDrawing();
        ClearBackground(DARKGREEN);
        

        if (!betting) {
            int i = 1;
        } else {
            if (IsKeyPressed(KEY_DOWN)) {
                    bet -= betIncrement;
                    if (bet < minimumBet) {bet = minimumBet; }
                }
            if (IsKeyPressed(KEY_UP)) {
                    bet += betIncrement;
                    if (bet > balance) {bet = balance; }
            }
            if (IsKeyPressed(KEY_SPACE)) {
                betting = false;
                balance -= bet;
            }
        }
        DrawText(TextFormat("Balance: %i", balance), 0, 0, 20, WHITE);
        DrawText(TextFormat("Bet: %i", bet), 0, 20, 20, WHITE);
        
        


        

        
        

        
        EndDrawing();
    }

    CloseWindow();
}