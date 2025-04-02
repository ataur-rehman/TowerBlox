#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>
#include<sstream>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

class TowerBlocks
{
public:
    TowerBlocks();
    static int score;
    static int score1;
    void run();
    void GameOver();
    void Congratulations();
    void showStartupMenu();
    void showOptionsMenu();



private:
    // RenderWindow window;
    RectangleShape hardButton; // Assuming you have set its bounds appropriately
    RectangleShape normalButton;

    int blocksSinceLastBackgroundMove;
    float backgroundYOffset;
    Texture backgroundImageTexture;
    Sprite backgroundImage;

    Font font; // Font for displaying score
    Text scoreText; // SFML Text for drawing score
    int lives;
    Text livesText;

    RenderWindow window;
    vector<RectangleShape> blocks;
    RectangleShape block;
    Vector2f pivot;
    float length;
    float angle;
    float maxAngle;
    float angularVelocity;
    float f = 0.0009;
    Vector2f lastFallenBlockPosition;
    bool isFalling;

    void handleEvents();
    void drawWire(const RectangleShape& shape);
    void drawBlocksWithOutline();
    void simulateFallingAnimation();
    void drawFallingBlock();
    void checkAndStackBlock();
    void updateSwingingBlock();
    void drawScore();
    void drawLives();
    bool isGameOver;
    void loadBackgroundImage(string str);
    void drawBackground();

};

int TowerBlocks::score = 0;
int TowerBlocks::score1 = 0;

TowerBlocks::TowerBlocks()
{
    window.create(VideoMode(950, 950), "Tower Blocks");

    block.setSize(Vector2f(70, 70));
    pivot = Vector2f(475, 0);
    length = 175.0;
    angle = 0.0;
    maxAngle = 3.14159 / 5.0;
    lives = 3;
    isFalling = false;

    block.setFillColor(Color::Magenta);

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);

    if (!font.loadFromFile("timesnewroman.ttf")) {
        // Handle error
    }

    loadBackgroundImage("bestpic.jpg");

  
    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setFillColor(Color::White);
    livesText.setString("Lives: 3");
    livesText.setPosition(10, 10);
}


void TowerBlocks::showStartupMenu()
{
    // Create "Start Game" button
    RectangleShape startButton(Vector2f(200, 50));
    startButton.setFillColor(Color::Transparent);
    startButton.setOutlineColor(Color::White);
    startButton.setOutlineThickness(2.0);
    startButton.setPosition((window.getSize().x - startButton.getSize().x) / 2, (window.getSize().y - startButton.getSize().y) / 2 - 70);

    // Create "End Game" button
    RectangleShape endButton(Vector2f(200, 50));
    endButton.setFillColor(Color::Transparent);
    endButton.setOutlineColor(Color::White);
    endButton.setOutlineThickness(2.0);
    endButton.setPosition((window.getSize().x - endButton.getSize().x) / 2, (window.getSize().y - endButton.getSize().y) / 2);

    Text startText;
    startText.setFont(font);
    startText.setCharacterSize(30);
    startText.setFillColor(Color::White);
    startText.setString("Start Game");
    FloatRect startTextBounds = startText.getLocalBounds();
    startText.setPosition(startButton.getPosition().x + (startButton.getSize().x - startTextBounds.width) / 2,
        startButton.getPosition().y + (startButton.getSize().y - startTextBounds.height) / 2 - 10);

    Text endText;
    endText.setFont(font);
    endText.setCharacterSize(30);
    endText.setFillColor(Color::White);
    endText.setString("End Game");
    FloatRect endTextBounds = endText.getLocalBounds();
    endText.setPosition(endButton.getPosition().x + (endButton.getSize().x - endTextBounds.width) / 2,
        endButton.getPosition().y + (endButton.getSize().y - endTextBounds.height) / 2 - 10);

    Text demoText;
    demoText.setFont(font);
    demoText.setCharacterSize(40);
    demoText.setFillColor(Color::White);
    demoText.setString("Tower Bloxx Demo");
    FloatRect demoTextBounds = demoText.getLocalBounds();
    demoText.setPosition((window.getSize().x - demoTextBounds.width) / 2, (window.getSize().y - demoTextBounds.height) / 2 - 175);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                if (startButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    return;  // Exit the function to start the game
                }
                else if (endButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    window.close();  // Exit the program
                }
            }
        }

        window.clear();
        window.draw(startButton);
        window.draw(startText);
        window.draw(endButton);
        window.draw(endText);
        window.draw(demoText);
        window.display();
    }
}


void TowerBlocks::showOptionsMenu()
{
    // Create Hard button
    RectangleShape hardButton(Vector2f(200, 50));
    hardButton.setFillColor(Color::Transparent);
    hardButton.setOutlineColor(Color::White);
    hardButton.setOutlineThickness(2.0f);
    hardButton.setPosition((window.getSize().x - hardButton.getSize().x) / 2, (window.getSize().y - hardButton.getSize().y) / 2 - 70);

    // Create Normal button
    RectangleShape normalButton(Vector2f(200, 50));
    normalButton.setFillColor(Color::Transparent);
    normalButton.setOutlineColor(Color::White);
    normalButton.setOutlineThickness(2.0);
    normalButton.setPosition((window.getSize().x - normalButton.getSize().x) / 2, (window.getSize().y - normalButton.getSize().y) / 2);

    Text hardText;
    hardText.setFont(font);
    hardText.setCharacterSize(30);
    hardText.setFillColor(Color::White);
    hardText.setString("Hard");
    FloatRect hardTextBounds = hardText.getLocalBounds();
    hardText.setPosition(hardButton.getPosition().x + (hardButton.getSize().x - hardTextBounds.width) / 2,
        hardButton.getPosition().y + (hardButton.getSize().y - hardTextBounds.height) / 2 - 10);

    Text normalText;
    normalText.setFont(font);
    normalText.setCharacterSize(30);
    normalText.setFillColor(Color::White);
    normalText.setString("Normal");
    FloatRect normalTextBounds = normalText.getLocalBounds();
    normalText.setPosition(normalButton.getPosition().x + (normalButton.getSize().x - normalTextBounds.width) / 2,
        normalButton.getPosition().y + (normalButton.getSize().y - normalTextBounds.height) / 2 - 10);

    Text chooseDifficultyText;
    chooseDifficultyText.setFont(font);
    chooseDifficultyText.setCharacterSize(40);
    chooseDifficultyText.setFillColor(Color::White);
    chooseDifficultyText.setString("Choose difficulty wisely!");
    FloatRect chooseDifficultyBounds = chooseDifficultyText.getLocalBounds();
    chooseDifficultyText.setPosition((window.getSize().x - chooseDifficultyBounds.width) / 2, (window.getSize().y - chooseDifficultyBounds.height) / 2 - 175);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseButtonPressed:
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                // Check if we clicked the "Hard" button
                if (hardButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                 
                    angularVelocity = 0.0017; // Set angular velocity for hard difficulty
                    lives = 1;  // Set lives to 1 for hard difficulty
                    return; // Exit the function
                }

                // Check if we clicked the "Normal" button
                if (normalButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                   
                    angularVelocity = 0.001; // Set angular velocity for normal difficulty
                    lives = 3;  // Set lives to 3 for normal difficulty
                    return; // Exit the function
                }
                break;


            }
        }
        window.clear();
        window.draw(hardButton);
        window.draw(hardText);
        window.draw(normalButton);
        window.draw(normalText);
        window.draw(chooseDifficultyText);
        window.display();
    }
}


void TowerBlocks::loadBackgroundImage(string str)
{
    if (!backgroundImageTexture.loadFromFile(str)) {
        // Handle error: Unable to load the background image
    }

    // Set the background image position and scale to fill the window
    backgroundImage.setTexture(backgroundImageTexture);
    backgroundImage.setPosition(0, window.getSize().y - blocks.size() * block.getSize().y);
    backgroundImage.setScale(
        (float)window.getSize().x / (float)backgroundImageTexture.getSize().x,
        (float)window.getSize().y / (float)backgroundImageTexture.getSize().y
    );
}

void TowerBlocks::handleEvents()
{
    Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed)
            window.close();

        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && !isFalling) {
            isFalling = true;
        }
    }
}

void TowerBlocks::drawWire(const RectangleShape& shape) {
    if (!isFalling)
    {
        VertexArray wireLine(Lines, 2);
        wireLine[0].position = pivot;
        wireLine[1].position = shape.getPosition() + Vector2f(shape.getSize().x / 2, shape.getSize().y / 2);
        wireLine[0].color = Color::White;
        wireLine[1].color = Color::White;
        window.draw(wireLine);
    }
}

void TowerBlocks::drawBackground()
{

    backgroundImage.setPosition(0, +backgroundYOffset);
    window.draw(backgroundImage);
}

void TowerBlocks::drawBlocksWithOutline()
{
    for (const auto& blockInst : blocks)
    {
        RectangleShape outlinedBlock(blockInst);
        outlinedBlock.setOutlineColor(Color::Black);
        outlinedBlock.setOutlineThickness(1.0f);

        window.draw(outlinedBlock);

        RectangleShape windowRect(Vector2f(20, 20));
        windowRect.setFillColor(Color::Black);

        float windowGap = 10.0f;
        float windowPosY = outlinedBlock.getPosition().y + windowGap;

        while (windowPosY + windowRect.getSize().y < outlinedBlock.getPosition().y + outlinedBlock.getSize().y) {
            float windowPosX = outlinedBlock.getPosition().x + windowGap;

            while (windowPosX + windowRect.getSize().x < outlinedBlock.getPosition().x + outlinedBlock.getSize().x) {
                windowRect.setPosition(windowPosX, windowPosY);
                window.draw(windowRect);

                windowPosX += windowRect.getSize().x + windowGap;
            }

            windowPosY += windowRect.getSize().y + windowGap;
        }
    }
}

void TowerBlocks::simulateFallingAnimation() {
    block.move(0, 10);

    RectangleShape outlinedBlock(block);
    outlinedBlock.setOutlineColor(Color::Black);
    outlinedBlock.setOutlineThickness(1.0);

    window.draw(outlinedBlock);

    RectangleShape windowRect(Vector2f(20, 20));
    windowRect.setFillColor(Color::Black);

    float windowGap = 10.0;
    float windowPosY = outlinedBlock.getPosition().y + windowGap;

    while (windowPosY + windowRect.getSize().y < outlinedBlock.getPosition().y + outlinedBlock.getSize().y) {
        float windowPosX = outlinedBlock.getPosition().x + windowGap;

        while (windowPosX + windowRect.getSize().x < outlinedBlock.getPosition().x + outlinedBlock.getSize().x) {
            windowRect.setPosition(windowPosX, windowPosY);
            window.draw(windowRect);

            windowPosX += windowRect.getSize().x + windowGap;
        }

        windowPosY += windowRect.getSize().y + windowGap;
    }
}

void TowerBlocks::drawFallingBlock()
{
    // window.display();
    this_thread::sleep_for(chrono::milliseconds(20));
}

void TowerBlocks::checkAndStackBlock()
{
    if (isFalling && block.getPosition().y >= window.getSize().y - block.getSize().y * (blocks.size() + 1))
    {
        const float currentBlockPositionX = block.getPosition().x;
        const float currentBlockWidth = block.getSize().x;
        const float currentBlockRight = currentBlockPositionX + currentBlockWidth;

        bool isAligned = true;
        if (!blocks.empty()) {
            const float lastBlockPositionX = lastFallenBlockPosition.x;
            const float lastBlockWidth = blocks.back().getSize().x; // Assuming this is consistent with block.getSize().x
            const float lastBlockRight = lastBlockPositionX + lastBlockWidth;

            // Check if the current block is within the boundaries of the last block
            isAligned = currentBlockPositionX < lastBlockRight && currentBlockRight > lastBlockPositionX;
        }

        // If aligned, stack the block on top of the last one
        if (isAligned)
        {
            // angularVelocity = 0.001f;  // Set a fixed angular velocity temporarily
            score++;
            if (score1 == 0)
            {
                score1++;
                score--;
            }
            block.setPosition(currentBlockPositionX, window.getSize().y - block.getSize().y * (blocks.size() + 1));
            blocks.push_back(block);
            lastFallenBlockPosition = block.getPosition();
            isFalling = false;  // Stop the falling action after successful stacking

            // Increment background offset after each block, starting from the 5th block up to the 10th block
            if (blocks.size() >= 6 && blocks.size() <= 10)
            {
                backgroundYOffset += block.getSize().y / 4.0f; // Adjust the value to control the offset increment
            }
        }
        else
        {
            // Unsuccessful stacking, decrement lives
            lives--;
            if (lives < 0)
            {
                // Game over logic or reset the game
                lives = 3;  // Reset lives to 3 for simplicity
                score = 0;  // Reset score
            }
            else
            {
                // Reset block position for the next attempt
                block.setPosition(pivot.x - block.getSize().x / 2, pivot.y);
                isFalling = false;
            }
        }

        // If more than 5 blocks, remove the bottom block
        if (blocks.size() > 5)
        {
            blocks.erase(blocks.begin());

            for (size_t i = 0; i < blocks.size(); ++i)
            {
                float newY = window.getSize().y - block.getSize().y * (i + 1);
                blocks[i].setPosition(blocks[i].getPosition().x, newY);
            }
        }
    }
}

void TowerBlocks::drawLives()
{
    stringstream livesStream;
    livesStream << "Lives: " << lives;
    livesText.setString(livesStream.str());
    window.draw(livesText);
}

void TowerBlocks::drawScore() {


    stringstream scoreStream;
    scoreStream << "Score: " << score;
    scoreText.setString(scoreStream.str());
    // Set the position for the score text to top right
    scoreText.setPosition(window.getSize().x - scoreText.getLocalBounds().width - 30, 10);
    window.draw(scoreText);
}
void TowerBlocks::updateSwingingBlock()
{
    angle += angularVelocity;

    if (angle >= maxAngle || angle <= -maxAngle) {
        angularVelocity = -angularVelocity;
    }

    float x1 = pivot.x + length * sin(angle);
    float y1 = pivot.y + length * cos(angle);

    block.setPosition(x1, y1);

    RectangleShape outlinedBlock(block);
    outlinedBlock.setOutlineColor(Color::Black);
    outlinedBlock.setOutlineThickness(1.0f);

    window.draw(outlinedBlock);

    RectangleShape windowRect(Vector2f(20, 20));
    windowRect.setFillColor(Color::Black);

    float windowGap = 10.0f;
    float windowPosY = outlinedBlock.getPosition().y + windowGap;

    while (windowPosY + windowRect.getSize().y < outlinedBlock.getPosition().y + outlinedBlock.getSize().y)
    {
        float windowPosX = outlinedBlock.getPosition().x + windowGap;

        while (windowPosX + windowRect.getSize().x < outlinedBlock.getPosition().x + outlinedBlock.getSize().x)
        {
            windowRect.setPosition(windowPosX, windowPosY);
            window.draw(windowRect);

            windowPosX += windowRect.getSize().x + windowGap;
        }

        windowPosY += windowRect.getSize().y + windowGap;
    }

    //  window.display();
}

void TowerBlocks::Congratulations() {
    Text congratsText;
    congratsText.setFont(font);
    congratsText.setCharacterSize(30);
    congratsText.setFillColor(Color::White);
    congratsText.setString("Not a Surprise.You Won!!");

    // Center the text on the screen
    FloatRect textBounds = congratsText.getLocalBounds();
    congratsText.setPosition((window.getSize().x - textBounds.width) / 2, (window.getSize().y - textBounds.height) / 2 - 20);
  
    window.clear();
    window.draw(congratsText);
    window.display();

    // Add a delay to show the message for a moment
    this_thread::sleep_for(chrono::seconds(3));

    // Close the window and end the program
    window.close();
}

void TowerBlocks::GameOver()
{
    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(30);
    gameOverText.setFillColor(Color::White);
    gameOverText.setString("Noob!  You Lost.");

    // Center the text on the screen
    FloatRect textBounds = gameOverText.getLocalBounds();
    gameOverText.setPosition((window.getSize().x - textBounds.width) / 2, (window.getSize().y - textBounds.height) / 2 - 20);
 
    window.clear();
    window.draw(gameOverText);
    window.display();

    // Add a delay to show the "Sedd,You lost." message for a moment
    this_thread::sleep_for(chrono::seconds(2));

    // Close the window and end the program
    window.close();
}

void TowerBlocks::run() {
    isGameOver = false;

    // Set the target score for congratulations
    const int targetScoreForCongrats = 10;

    while (window.isOpen()) {
        handleEvents();
        window.clear();
        drawBackground();
        if (!isGameOver) {
            drawWire(block);
            drawBlocksWithOutline();


            if (isFalling) {
                simulateFallingAnimation();
                drawFallingBlock();
                checkAndStackBlock();
            }
            else {
                updateSwingingBlock();
            }

            drawScore();
            drawLives();

            // Check for congratulations condition
            if (score >= targetScoreForCongrats)
            {
                isGameOver = true;
                Congratulations();
            }
            // Check for game over condition
            else if (lives == 0)
            {
                isGameOver = true;
                GameOver();
            }

            else {

                window.display();
            }
        }
    }
}

int main() {
    TowerBlocks game;
    game.showStartupMenu();
    game.showOptionsMenu();
    game.run();

    return 0;
}