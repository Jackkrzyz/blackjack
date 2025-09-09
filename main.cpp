#include <iostream>
#include <raylib.h>
#include <algorithm>
#include <vector>

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
                DrawRectangle(x, y, 70, 100, WHITE);
                DrawRectangleLines(x, y, 70, 100, BLACK);
                if (suit == 1) {
                    DrawText("Hearts", x + 10, y + 10, 5, RED);
                } else if (suit == 2) {
                    DrawText("Diamonds", x + 10, y + 10, 5, RED);
                } else if (suit == 3) {
                    DrawText("Clubs", x + 10, y + 10, 5, BLACK);
                } else if (suit == 4) {
                    DrawText("Spades", x + 10, y + 10, 5, BLACK);
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
                DrawRectangle(x, y, 70, 100, DARKGRAY);
                DrawRectangleLines(x, y, 70, 100, BLACK);
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


std::vector<int> interpolatePosition(int currentX, int currentY, int targetX, int targetY, int increment) {
    std::vector<int> newPosition;
    if(targetX > currentX)
    {
        newPosition.push_back(currentX += increment);
    }
    else if(targetX < currentX)
    {
        newPosition.push_back(currentX -= increment);
    }
    else {
        newPosition.push_back(currentX);
    }

    if(targetY > currentY)
    {
        newPosition.push_back(currentY += increment);
    }
    else if(targetY < currentY)
    {
        newPosition.push_back(currentY -= increment);
    }
    else {
        newPosition.push_back(currentY);
    }
    return newPosition;
}

int main () {
    
    const int SCREEN_WIDTH = 500;
    const int SCREEN_HEIGHT = 500;

    int minimumBet = 15;
    int betIncrement = 10;
    int balance = 2000;
    int bet = 15;
    int gameState = 0;
    bool bust = false;
    bool blackjack = false;
    // 0 = betting
    // 1 = add cards
    // 2 = deal
    // 3 = player's turn
    int animationState = 0;
    bool allowDrawCards = false;
    

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Blackjack");
    SetTargetFPS(60);

    Player player;
    Player dealer;
    Deck deck;
    Card* cardsToDraw[22];
    int cardsInPlay;
    deck.shuffle();
    

    // Main Game loop
    while (WindowShouldClose() == false && balance > 0 && !IsKeyDown(KEY_DELETE)){
        BeginDrawing();
        ClearBackground(DARKGREEN);
        

        if (gameState == 0) {
            DrawText("Place Bet", 125, 125, 50, WHITE);
            if (IsKeyPressed(KEY_DOWN)) {
                    bet -= betIncrement;
                    if (bet < minimumBet) {bet = minimumBet; }
                }
            if (IsKeyPressed(KEY_UP)) {
                    bet += betIncrement;
                    if (bet > balance) {bet = balance; }
            }
            if (IsKeyPressed(KEY_SPACE)) {
                gameState = 1;
                balance -= bet;
            }
            

        } else if (gameState == 1) {
            player.addCard(deck.cards[0]);
            dealer.addCard(deck.cards[1]);
            player.addCard(deck.cards[2]);
            dealer.addCard(deck.cards[3]);
            for (int i = 0; i < 4; i++)
            {
                cardsToDraw[i] = deck.cards[i];
                cardsToDraw[i]->x = 250;
                cardsToDraw[i]->y = 0;
            }
            animationState = 0;
            gameState = 2;
            allowDrawCards = true;
            cardsInPlay = 4;
        } else if (gameState == 2) {
            if (animationState < 30) {
                std::vector<int> newPos = interpolatePosition(player.hand[0]->x, player.hand[0]->y, 180, 300, 10);
                player.hand[0]->x = newPos[0];
                player.hand[0]->y = newPos[1];
                animationState++;
            }
            if (animationState >= 30 && animationState < 60) {
                std::vector<int> newPos = interpolatePosition(player.hand[1]->x, player.hand[1]->y, 260,300, 10);
                player.hand[1]->x = newPos[0];
                player.hand[1]->y = newPos[1];
                animationState++;
            }
            if (animationState >= 60 && animationState < 90)
            {
                dealer.hand[0]->faceUp = false;
                std::vector<int> newPos = interpolatePosition(dealer.hand[0]->x, dealer.hand[0]->y, 180, 50, 10);
                dealer.hand[0]->x = newPos[0];
                dealer.hand[0]->y = newPos[1];
                animationState++;
            }
            if (animationState >= 90 && animationState < 120)
            {
                std::vector<int> newPos = interpolatePosition(dealer.hand[1]->x,dealer.hand[1]->y, 260, 50, 10);
                dealer.hand[1]->x = newPos[0];
                dealer.hand[1]->y = newPos[1];
                animationState++;
            }
            if (animationState >= 120)
            {
                animationState = 0;
                gameState = 3;
            }

        } else if (gameState == 3)
        {
            if (!bust && !blackjack)
            {
                if (animationState == 0)
                {
                    if (IsKeyPressed(KEY_SPACE))
                    {
                        gameState = 4;
                    }
                    else if (IsKeyPressed(KEY_ENTER))
                    {
                        cardsInPlay++;
                        player.addCard(deck.cards[cardsInPlay - 1]);
                        cardsToDraw[cardsInPlay - 1] = deck.cards[cardsInPlay - 1];
                        cardsToDraw[cardsInPlay - 1]->x=250;
                        cardsToDraw[cardsInPlay - 1]->y=0;
                        animationState++;

                    }
                } else if (animationState < 31)
                {
                    std::vector<int> newPos = interpolatePosition(player.hand[player.cardsReceived - 1]->x,player.hand[player.cardsReceived - 1]->y, 180 + ((player.cardsReceived - 1) * 80), 300, 10);
                    player.hand[player.cardsReceived - 1]->x = newPos[0];
                    player.hand[player.cardsReceived - 1]->y = newPos[1];
                    animationState++;
                } else if (animationState == 31)
                {
                    if (player.score > 21) {
                        bust = true;
                    } else if (player.score == 21)
                    {
                        blackjack = true;
                    }
                    animationState = 0;
                }
            }
            
        }

        if (allowDrawCards)
        {
            for (int i = 0; i < cardsInPlay; i++){
                cardsToDraw[i]->draw();
            }
            DrawText(TextFormat("Your Score: %i", player.score), 170, 400, 20, WHITE);
            DrawText("Enter - Hit", 170, 420, 20, WHITE);
            DrawText("Space - Stand", 170, 440, 20, WHITE);

        }
        

        DrawText(TextFormat("Balance: %i", balance), 0, 0, 20, WHITE);
        DrawText(TextFormat("Bet: %i", bet), 0, 20, 20, WHITE);
        
        EndDrawing();
    }

    CloseWindow();
}