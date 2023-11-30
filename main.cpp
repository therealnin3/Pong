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
const float playerVelocity = 5;
const Color playerColor = WHITE;

// Ball settings
const int ballRadius = 10;
const Vector2 ballPosition = {screenWidth / 2, screenHeight / 2};
const Vector2 ballVelocity = {5, 0};
const Color ballColor = WHITE;

// Human class
class Human
{
public:
    int width;
    int height;
    float velocity;
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

// Ai class
class Ai : public Human
{
    // No need to define a separate constructor for Ai if it's the same as Human
};

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

        if (position.x + radius >= screenWidth || position.x - radius <= 0)
        {
            velocity.x *= -1;
        }
    };
};

// Game components
Human P1;
Ai P2;
Ball ball;

// Function prototypes
void initWindow(int screenWidth, int screenHeight, const char *title, int fps);
void initGameComponents();
void drawBackground(Color backgroundColor, Color lineColor);

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
        // P2.Update();
        ball.Update();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// Initialize all the Components of the game
void initGameComponents()
{
    // Init Human
    P1 = Human(playerWidth, playerHeight, {0 + playerGapFromSide, screenHeight / 2 - playerHeight / 2}, playerVelocity, playerColor);

    // Init Ai
    P2 = Human(playerWidth, playerHeight, {0 + playerGapFromSide, screenHeight / 2 - playerHeight / 2}, playerVelocity, playerColor);

    // Init ball
    ball = Ball(ballRadius, ballPosition, ballVelocity, ballColor);
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