#include <iostream>
#include <raylib.h>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

class Card {
    public:
        int value; // A = 1, 2 = 2..., 11 = J, 12 = Q, 13 = K
        int suit; // 1 = Hearts, 2= diamonds, 3 = clubs, 4 = spades
        int x;
        int y;
        int baseX;
        int baseY;
        bool faceUp;
        Card(int v, int s, bool f) {
            value = v;
            suit = s;
            faceUp = f;
            baseX = 0;
            baseY = 0;
        }
        void draw(float scale) {
            int drawX = (int)(baseX * scale);
            int drawY = (int)(baseY * scale);
            if (faceUp) {
                DrawRectangle(drawX, drawY, (int)(70 * scale), (int)(100 * scale), WHITE);
                DrawRectangleLines(drawX, drawY, (int)(70 * scale), (int)(100 * scale), BLACK);
                if (suit == 1) {
                    DrawText("Hearts", drawX + (int)(10 * scale), drawY + (int)(10 * scale), (int)(5 * scale), RED);
                } else if (suit == 2) {
                    DrawText("Diamonds", drawX + (int)(10 * scale), drawY + (int)(10 * scale), (int)(5 * scale), RED);
                } else if (suit == 3) {
                    DrawText("Clubs", drawX + (int)(10 * scale), drawY + (int)(10 * scale), (int)(5 * scale), BLACK);
                } else if (suit == 4) {
                    DrawText("Spades", drawX + (int)(10 * scale), drawY + (int)(10 * scale), (int)(5 * scale), BLACK);
                }
                if (value == 13) {
                    DrawText("K", drawX + (int)(10 * scale), drawY + (int)(20 * scale), (int)(20 * scale), BLACK);
                }
                else if(value == 12)
                {
                    DrawText("Q", drawX + (int)(10 * scale), drawY + (int)(20 * scale), (int)(20 * scale), BLACK);
                }
                else if(value == 11)
                {
                    DrawText("J", drawX + (int)(10 * scale), drawY + (int)(20 * scale), (int)(20 * scale), BLACK);
                }
                else {
                    DrawText(TextFormat("%i", value), drawX + (int)(10 * scale), drawY + (int)(20 * scale), (int)(20 * scale), BLACK);
                }
            } else {
                DrawRectangle(drawX, drawY, (int)(70 * scale), (int)(100 * scale), DARKGRAY);
                DrawRectangleLines(drawX, drawY, (int)(70 * scale), (int)(100 * scale), BLACK);
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
        if (c->value == 1) {
            if (score + 11 <= 21) {
                score += 11;
            } else {
                score += 1;
            }
        } else if (c->value >= 10) {
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
    float scale = 1.0f;

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
    SetWindowMinSize(500,500);

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

        if (IsKeyPressed(KEY_Q))
        {
            SetWindowSize(GetScreenWidth() + 50, GetScreenHeight() + 50);
        } else if (IsKeyPressed(KEY_A) && GetScreenWidth() >= 550)
        {
            SetWindowSize(GetScreenWidth() - 50, GetScreenHeight() - 50);
        } 
        scale = (float)GetScreenWidth() / (float)SCREEN_WIDTH;

        

        if (gameState == 0) {
            DrawText("Place Bet", (int)(125 * scale), (int)(125 * scale), (int)(50 * scale), WHITE);
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
                cardsToDraw[i]->baseX = 250;
                cardsToDraw[i]->baseY = 0;
            }
            animationState = 0;
            gameState = 2;
            allowDrawCards = true;
            cardsInPlay = 4;
        } else if (gameState == 2) {
            if (animationState < 30) {
                std::vector<int> newPos = interpolatePosition(player.hand[0]->baseX, player.hand[0]->baseY, 180, 300, 10);
                player.hand[0]->baseX = newPos[0];
                player.hand[0]->baseY = newPos[1];
                animationState++;
            }
            if (animationState >= 30 && animationState < 60) {
                std::vector<int> newPos = interpolatePosition(player.hand[1]->baseX, player.hand[1]->baseY, 260, 300, 10);
                player.hand[1]->baseX = newPos[0];
                player.hand[1]->baseY = newPos[1];
                animationState++;
            }
            if (animationState >= 60 && animationState < 90)
            {
                dealer.hand[0]->faceUp = false;
                std::vector<int> newPos = interpolatePosition(dealer.hand[0]->baseX, dealer.hand[0]->baseY, 180, 50, 10);
                dealer.hand[0]->baseX = newPos[0];
                dealer.hand[0]->baseY = newPos[1];
                animationState++;
            }
            if (animationState >= 90 && animationState < 120)
            {
                std::vector<int> newPos = interpolatePosition(dealer.hand[1]->baseX, dealer.hand[1]->baseY, 260, 50, 10);
                dealer.hand[1]->baseX = newPos[0];
                dealer.hand[1]->baseY = newPos[1];
                animationState++;
            }
            if (animationState >= 120)
            {
                animationState = 0;
                gameState = 3;
            }
            if (player.score == 21)
            {
                blackjack = true;
                gameState = 4;
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
                        cardsToDraw[cardsInPlay - 1]->baseX = 250;
                        cardsToDraw[cardsInPlay - 1]->baseY = 0;
                        animationState++;

                    }
                } else if (animationState < 31)
                {
                    std::vector<int> newPos = interpolatePosition(player.hand[player.cardsReceived - 1]->baseX, player.hand[player.cardsReceived - 1]->baseY, (180 + ((player.cardsReceived - 1) * 80)), 300, 10);
                    player.hand[player.cardsReceived - 1]->baseX = newPos[0];
                    player.hand[player.cardsReceived - 1]->baseY = newPos[1];
                    animationState++;
                } else if (animationState == 31)
                {
                    if (player.score > 21) {
                        bust = true;
                    } else if (player.score == 21)
                    {
                        gameState = 4;
                    }
                    animationState = 0;
                }
            }
            else if (bust)
            {
                dealer.hand[0]->faceUp = true;
                DrawText("You Busted!", (int)(125 * scale), (int)(225 * scale), (int)(50 * scale), RED);
                DrawText("Press Enter to Continue", (int)(125 * scale), (int)(275 * scale), (int)(20 * scale), WHITE);
                if (IsKeyPressed(KEY_ENTER))
                {
                    player = Player();
                    dealer = Player();
                    deck = Deck();
                    deck.shuffle();
                    cardsInPlay = 0;
                    bust = false;
                    blackjack = false;
                    allowDrawCards = false;
                    gameState = 0;
                }
            }
            else if (blackjack)
            {
                dealer.hand[0]->faceUp = true;
                DrawText("Blackjack!", (int)(175 * scale), (int)(225 * scale), (int)(50 * scale), GOLD);
                DrawText("Press Enter to Continue", (int)(100 * scale), (int)(275 * scale), (int)(20 * scale), WHITE);
                if (IsKeyPressed(KEY_ENTER))
                {
                    // Reset everything
                    balance += bet * 2.5;
                    player = Player();
                    dealer = Player();
                    deck = Deck();
                    deck.shuffle();
                    cardsInPlay = 0;
                    bust = false;
                    blackjack = false;
                    allowDrawCards = false;
                    gameState = 0;
                }
            }
            
        } else if (gameState == 4)
        {
            if (animationState >= 0 && animationState < 16)
            {
                dealer.hand[0]->faceUp = true;
                animationState++;
            }
            if (animationState == 16)
            {
                if (dealer.score < 17)
                {
                    cardsInPlay++;
                    dealer.addCard(deck.cards[cardsInPlay - 1]);
                    cardsToDraw[cardsInPlay - 1] = deck.cards[cardsInPlay - 1];
                    cardsToDraw[cardsInPlay - 1]->baseX = 250;
                    cardsToDraw[cardsInPlay - 1]->baseY = 0;
                    animationState++;
                }
                else {
                    if (dealer.score > 21)
                    {
                        while (!IsKeyPressed(KEY_ENTER))
                        {
                            DrawText("You Win", 150, 250, 25, GREEN);
                            DrawText("Enter - Play Again", 150, 275, 25, WHITE);
                        }
                        

                        balance += bet * 2;
                        player = Player();
                        dealer = Player();
                        deck = Deck();
                        deck.shuffle();
                        cardsInPlay = 0;
                        bust = false;
                        blackjack = false;
                        allowDrawCards = false;
                        gameState = 0;
                    }
                    else if (dealer.score == player.score)
                    {
                        while (!IsKeyPressed(KEY_ENTER))
                        {
                            DrawText("Push", 150, 250, 25, WHITE);
                            DrawText("Enter - Play Again", 150, 275, 25, WHITE);
                        }
                        balance += bet;
                        player = Player();
                        dealer = Player();
                        deck = Deck();
                        deck.shuffle();
                        cardsInPlay = 0;
                        bust = false;
                        blackjack = false;
                        allowDrawCards = false;
                        gameState = 0;
                    }
                    else if (dealer.score > player.score)
                    {
                        while (!IsKeyPressed(KEY_ENTER))
                        {
                            DrawText("You lost", 150, 250, 25, RED);
                            DrawText("Enter - Play Again", 150, 275, 25, WHITE);
                        }
                        player = Player();
                        dealer = Player();
                        deck = Deck();
                        deck.shuffle();
                        cardsInPlay = 0;
                        bust = false;
                        blackjack = false;
                        allowDrawCards = false;
                        gameState = 0;
                    }
                    else if (player.score > dealer.score)
                    {
                        while (!IsKeyPressed(KEY_ENTER))
                        {
                            DrawText("You Win", 150, 250, 25, GREEN);
                            DrawText("Enter - Play Again", 150, 275, 25, WHITE);
                        }
                        
                        balance += bet * 2;
                        player = Player();
                        dealer = Player();
                        deck = Deck();
                        deck.shuffle();
                        cardsInPlay = 0;
                        bust = false;
                        blackjack = false;
                        allowDrawCards = false;
                        gameState = 0;
                    }
                }
            }
            if (animationState > 16 && animationState < 31){
                std::vector<int> newPos = interpolatePosition(dealer.hand[dealer.cardsReceived - 1]->baseX, dealer.hand[dealer.cardsReceived - 1]->baseY, (180 + ((dealer.cardsReceived - 1) * 80)), 50, 10);
                dealer.hand[dealer.cardsReceived - 1]->baseX = newPos[0];
                dealer.hand[dealer.cardsReceived - 1]->baseY = newPos[1];
                animationState++;
            }
            if (animationState == 31) { animationState = 16; }
            
        }

        if (allowDrawCards)
        {
            for (int i = 0; i < cardsInPlay; i++){
                cardsToDraw[i]->draw(scale);
            }
            DrawText(TextFormat("Your Score: %i", player.score), (int)(170 * scale), (int)(400 * scale), (int)(20 * scale), WHITE);
            if (gameState ==3)
            {
                DrawText("Enter - Hit", (int)(170 * scale), (int)(420 * scale), (int)(20 * scale), WHITE);
                DrawText("Space - Stand", (int)(170 * scale), (int)(440 * scale), (int)(20 * scale), WHITE);
            }
            

        }
        

        DrawText(TextFormat("Balance: %i", balance), 0, 0, (int)(20 * scale), WHITE);
        DrawText(TextFormat("Bet: %i", bet), 0, (int)(20 * scale), (int)(20 * scale), WHITE);
        DrawText("Q/A to resize", 0, (int)(40 * scale), (int)(20 * scale), WHITE);
    
        
        EndDrawing();
    }

    CloseWindow();
}