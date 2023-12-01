// main.cpp

#include <iostream>
#include "include/raylib.h"

// Window settings
const char *title = "Pong";
const int screenWidth = 1280;
const int screenHeight = 800;
const int fps = 60;

// Human settings
const int playerWidth = 25;
const int playerHeight = 120;
const int playerGapFromSide = 50;
const float playerVelocity = 4.5;
const Color playerColor = WHITE;

// Ball settings
const int ballRadius = 10;
const Vector2 ballPosition = {screenWidth / 2, screenHeight / 2};
const Vector2 ballVelocity = {5, 5};
const Color ballColor = WHITE;
const int collisionMultiplier = 5;

// Ball class
class Ball
{
public:
    float radius;
    Vector2 position;
    Vector2 velocity;
    Color color;

    Ball()
    {
        // Initialize properties with default values
        radius = 0;
        position = {0, 0};
        velocity = {0, 0};
        color = WHITE;
    };

    Ball(float radius, Vector2 position, Vector2 velocity, Color color)
    {
        this->radius = radius;
        this->position = position;
        this->velocity = velocity;
        this->color = color;
    };

    void Update()
    {
        // Update position based on velocity
        position.x += velocity.x;
        position.y += velocity.y;

        // Draw the ball
        DrawCircle(position.x, position.y, radius, color);

        // Check for window collision
        if (position.y + radius >= screenHeight || position.y - radius <= 0)
        {
            velocity.y *= -1;
        }
    };
};

// Create Ball object
Ball ball;

// Human class
class Human
{
public:
    int width;
    int height;
    float velocity;
    int score = 0;
    Vector2 position;
    Color color;

    Human()
    {
        // Initialize properties with default values
        width = 0;
        height = 0;
        position = {0, 0};
        velocity = 0;
        color = WHITE;
    };

    Human(int width, int height, Vector2 position, float velocity, Color color)
    {
        this->width = width;
        this->height = height;
        this->position = position;
        this->velocity = velocity;
        this->color = color;
    };

    void Update()
    {
        // Check for input and update position
        if (IsKeyDown(KEY_W) && position.y > 0)
        {
            position.y -= playerVelocity;
        }
        else if (IsKeyDown(KEY_S) && position.y + height < screenHeight)
        {
            position.y += playerVelocity;
        }

        // Draw the player
        DrawRectangle(position.x, position.y, width, height, color);
    };
};

// BOT class
class BOT : public Human
{
public:
    // Default constructor
    BOT() : Human()
    {
    }

    // Parameterized constructor
    BOT(int width, int height, Vector2 position, float velocity, Color color) : Human(width, height, position, velocity, color)
    {
    }

    void Update()
    {
        // Algorithm for BOT
        if (ball.position.y > (position.y + this->height / 2) && position.y + height < screenHeight)
        {
            position.y += playerVelocity;
        }
        else if (ball.position.y < (position.y + this->height / 2) && position.y > 0)
        {
            position.y -= playerVelocity;
        }

        // Draw the player
        DrawRectangle(position.x, position.y, width, height, color);
    };
};

// Create Human object
Human P1;
BOT P2;

// Function prototypes
void initWindow(int screenWidth, int screenHeight, const char *title, int fps);
void initGameComponents();
void drawBackground(Color backgroundColor, Color lineColor);
void checkForBallPlayerCollision();
void restartGame();
void checkWinningCondition();
void displayText();

int main()
{
    // Init window
    initWindow(screenWidth, screenHeight, title, fps);

    // Initialize game components
    initGameComponents();

    // Game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();

        // Draw backgound
        drawBackground(BLACK, WHITE);

        // Update components
        P1.Update();
        P2.Update();
        ball.Update();

        // Check for ball collision with players
        checkForBallPlayerCollision();

        // Check for winning condition
        checkWinningCondition();

        // Display scores
        displayText();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void checkForBallPlayerCollision()
{
    // Check for collision with player 1
    if (CheckCollisionCircleRec(ball.position, ball.radius, {P1.position.x, P1.position.y, (float)P1.width, (float)P1.height}))
    {
        // Calculate the relative position of the collision point to the center of the paddle
        float relativeCollisionY = ball.position.y - (P1.position.y + P1.height / 2);

        // Calculate a factor based on the relative position
        float collisionFactor = relativeCollisionY / (P1.height / 2);

        // Apply the factor to the ball's velocity
        ball.velocity.x *= -1;
        ball.velocity.y += collisionFactor * collisionMultiplier;
    }

    // Check for collision with player 2
    if (CheckCollisionCircleRec(ball.position, ball.radius, {P2.position.x, P2.position.y, (float)P2.width, (float)P2.height}))
    {
        // Calculate the relative position of the collision point to the center of the paddle
        float relativeCollisionY = ball.position.y - (P2.position.y + P2.height / 2);

        // Calculate a factor based on the relative position
        float collisionFactor = relativeCollisionY / (P2.height / 2);

        // Apply the factor to the ball's velocity
        ball.velocity.x *= -1;
        ball.velocity.y += collisionFactor * collisionMultiplier;
    }
}

// Initialize all the Components of the game
void initGameComponents()
{
    // Init Human
    P1 = Human(playerWidth, playerHeight, {0 + playerGapFromSide, screenHeight / 2 - playerHeight / 2}, playerVelocity, playerColor);

    // Init Human
    P2 = BOT(playerWidth, playerHeight, {screenWidth - playerGapFromSide - playerWidth, screenHeight / 2 - playerHeight / 2}, playerVelocity, playerColor);

    // Init ball with random velocity
    float randomVelocityX = GetRandomValue(3, 6);
    float randomVelocityY = GetRandomValue(3, 6);
    ball = Ball(ballRadius, ballPosition, {randomVelocityX, randomVelocityY}, ballColor);
}

// Initialize the Window
void initWindow(int screenWidth, int screenHeight, const char *title, int fps)
{
    std::cout << "Starting up game ..." << std::endl;
    InitWindow(screenWidth, screenHeight, title);
    SetTargetFPS(fps);
}

// Draws the background of the game
void drawBackground(Color backgroundColor, Color lineColor)
{
    ClearBackground(backgroundColor);
    DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, lineColor);
}

void checkWinningCondition()
{
    // Human wins
    if (ball.position.x + ball.radius >= screenWidth)
    {
        P1.score++;
        restartGame();
    }

    // BOT wins
    if (ball.position.x - ball.radius <= 0)
    {
        P2.score++;
        restartGame();
    }
}

void displayText()
{
    // Display scores
    DrawText(TextFormat("%i", P1.score), screenWidth / 2 - 100, 50, 50, WHITE);
    DrawText(TextFormat("%i", P2.score), screenWidth / 2 + 100, 50, 50, WHITE);

    // Display ball velocity
    DrawText(TextFormat("x%0.3f", abs(ball.velocity.y)), ball.position.x - 3 * ball.radius, ball.position.y - 3 * ball.radius, 20, WHITE);
}

void restartGame()
{
    P1.position = {0 + playerGapFromSide, screenHeight / 2 - playerHeight / 2};
    P2.position = {screenWidth - playerGapFromSide - playerWidth, screenHeight / 2 - playerHeight / 2};
    ball.position = {screenWidth / 2, screenHeight / 2};
    float randomVelocityX = GetRandomValue(3, 6);
    float randomVelocityY = GetRandomValue(3, 6);
    ball.velocity = {randomVelocityX, randomVelocityY};
}