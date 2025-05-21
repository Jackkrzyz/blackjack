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
                DrawRectangle(x, y, 75, 100, WHITE);
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
                DrawRectangle(x, y, 75, 100, DARKGRAY);
                
            }
            DrawRectangleLines(x, y, 75, 100, BLACK);
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

            if (c->value == 1) {
                if (score + 11 <= 21) {
                        score += 11;
                } 
                else {
                    score += 1;
                }
            } else if (c-> value >= 10) {
                score += 10;
            } else {
                score += c->value;
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
    int cardsDealt = 0;
    deck.shuffle();
    

    // Main Game loop
    while (WindowShouldClose() == false && balance > 0 && !IsKeyDown(KEY_DELETE)){
        BeginDrawing();
        ClearBackground(DARKGREEN);
        

        if (!betting) {
            // Initial Cards
            if (cardsDealt == 0) {
                player.addCard(deck.cards[cardsDealt]);
                deck.cards[cardsDealt]->x=SCREEN_WIDTH / 2 - 80;
                deck.cards[cardsDealt]->y=300;
                cardsDealt++;

                player.addCard(deck.cards[cardsDealt]);
                deck.cards[cardsDealt]->x=SCREEN_WIDTH / 2 + 5;
                deck.cards[cardsDealt]->y=300;
                cardsDealt++;

                dealer.addCard(deck.cards[cardsDealt]);
                deck.cards[cardsDealt]->x=SCREEN_WIDTH / 2 - 80;
                deck.cards[cardsDealt]->y=100;
                cardsDealt++;
            }

            if (cardsDealt == 3) {
                DrawText("Space - HIT\nBackspace - STAND", 25, SCREEN_HEIGHT / 2, 20, WHITE);
            }
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


        DrawText(TextFormat("Bet: %i", bet), 0, SCREEN_HEIGHT - 40, 20, WHITE);
        DrawText(TextFormat("Balance: %i", balance), 0, SCREEN_HEIGHT - 20, 20, WHITE);
        DrawText(TextFormat("Score: %i", player.score),  SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT - 30, 20, WHITE);
        
        
        for (int i = 0; i < 52; i++) {
            deck.cards[i]->draw();
        }

        

        
        

        
        EndDrawing();
    }

    CloseWindow();
}