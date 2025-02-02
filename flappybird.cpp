#include <iostream>
#include<algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

const float gravity = 350.0;

//gameIsOver 
bool gameIsOver = false;
int score2;

bool isCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2)
{
    return sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds());
}

int main() {
    enum GameState { Playing, GameOver };

    sf::Font font;

    if (!font.loadFromFile("arial.ttf"))
    {
        std::cout << "Error loading font !";
    }
    sf::Font font2;

    if (!font2.loadFromFile("C:\\Users\\ayba8\\Downloads\\arial-font\\arial.ttf"))
    {
        std::cout << "Error loading font !";
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setPosition(350, 350); // adjust position
    scoreText.setFillColor(sf::Color::White);

    sf::Text scoreText2;
    scoreText2.setFont(font2);
    scoreText2.setCharacterSize(50);
    scoreText2.setPosition(1, -1); // adjust position
    scoreText2.setFillColor(sf::Color::White);

    float deltatime = 0;
    int i;// random number
    bool x = true;
    int speed = 150;
    float moveSpeed = 100;
    float jumpSpeed = 150;
    float verticalVelocity = 0;
    int score = 0;

    scoreText.setString("Score: " + std::to_string(score));

    // Bird character 
    Texture text;
    text.loadFromFile("haha.png ");

    Sprite Player;
    Player.setTexture(text);

    Clock gameclock;
    Clock inctime; // inc music and speed of pipes

    RenderWindow window(VideoMode(900, 800), "SFML Window");

    // Load the background texture
    Texture backgroundTexture;
    backgroundTexture.loadFromFile("day.png ");

    // Splash screen
    Texture splashTexture;
    splashTexture.loadFromFile("start.png ");
    Sprite splashSprite(splashTexture);
    // Set the splash screen scale to cover the entire window
    splashSprite.setScale(static_cast<float>(window.getSize().x) / splashSprite.getLocalBounds().width,
        static_cast<float>(window.getSize().y) / splashSprite.getLocalBounds().height);


    window.clear();
    window.draw(splashSprite);
    window.display();
    sleep(seconds(2));




    // Create a sprite for the background
    Sprite background;
    background.setTexture(backgroundTexture);
    background.setScale(static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y);

    // Making  pipe number 0 on the ground
    Texture pipetexturedown;
    pipetexturedown.loadFromFile("pipe.png");
    Sprite pipespritedown(pipetexturedown);
    pipespritedown.setPosition(600, 600);
    pipespritedown.scale(0.7f, 0.7f);

    // Making pipe number 0 upward 
    Texture pipetextureup;
    pipetextureup.loadFromFile("pipe_up.png");
    Sprite pipespriteup(pipetextureup);
    pipespriteup.setPosition(600, 0);
    pipespriteup.scale(0.7f, 0.9f);

    // Load game-over texture
    Texture gameOverTexture;
    gameOverTexture.loadFromFile("gameover.png");
    Sprite gameOverSprite(gameOverTexture);
    gameOverSprite.setScale(static_cast<float>(window.getSize().x) / gameOverTexture.getSize().x,
        static_cast<float>(window.getSize().y) / gameOverTexture.getSize().y);


    // random pipe up height
    int upmax = 10, upmin = -80;

    //random pipe down height
    int downmax = 500, downmin = 250;
    GameState gamestate = GameState::Playing;

    // Inserting the sound
    SoundBuffer voiceBuffer, jumpBuffer, collisionBuffer;

    if (!voiceBuffer.loadFromFile("general audio.mpeg") ||
        !jumpBuffer.loadFromFile("jump.mpeg") ||
        !collisionBuffer.loadFromFile("collision.mpeg"))
    {
        return EXIT_FAILURE;
    }


    Sound jumpSound, voiceSound, collisionSound;
    voiceSound.setBuffer(voiceBuffer);
    voiceSound.setVolume(100);
    voiceSound.setLoop(true);
    voiceSound.play();

    jumpSound.setBuffer(jumpBuffer);
    collisionSound.setBuffer(collisionBuffer);
    



    while (window.isOpen()) {
        deltatime = gameclock.getElapsedTime().asSeconds();
        gameclock.restart();

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        pipespritedown.move(-speed * deltatime, 0);
        pipespriteup.move(-speed * deltatime, 0);




        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            if (gamestate == GameState::Playing) {
                verticalVelocity = -jumpSpeed;
                 
            }
        }

        if (gamestate == GameState::Playing) {
            Player.move(0, verticalVelocity * deltatime);


            // Apply gravity
            verticalVelocity += gravity * deltatime;

        }

        //calculate the score

        if (pipespriteup.getPosition().x + 0.7 < Player.getPosition().x) {
            score++;
            score2++;
            jumpSound.play();
            scoreText.setString("Score: " + std::to_string(score));
            scoreText2.setString("Score: " + std::to_string(score2));
            // Reset bird and pipes
            pipespritedown.setPosition(900, pipespriteup.getPosition().y + 600);
            srand(time(NULL));
            i = rand() % (upmax - upmin + 1) + upmin;
            pipespriteup.setPosition(900, i);


            if (pipespriteup.getPosition().x < -600) {
                srand(time(NULL));
                i = rand() % (upmax - upmin + 1) + upmin;
                pipespriteup.setPosition(900, i);
                pipespriteup.move(-speed * deltatime, 0);
            }

            if (pipespritedown.getPosition().x < -600) {
                int y = pipespriteup.getPosition().y + 600;
                pipespritedown.setPosition(900, y);
            }
        }


        if (isCollision(Player, pipespritedown) || isCollision(Player, pipespriteup) || Player.getPosition().y > window.getSize().y || Player.getPosition().y < 0) {
            gamestate = GameState::GameOver;

            verticalVelocity = 0.0f;
            // Reset bird and pipes
            Player.setPosition(100, 300);
            pipespritedown.setPosition(600, 650);
            pipespriteup.setPosition(600, 0);
             collisionSound.play();
             voiceSound.stop();
        }

        else if (gamestate == GameState::GameOver) {


            if (event.key.code == sf::Keyboard::Space) {
                if (gameIsOver) {
                    // Reset game state and relevant variables
                    gamestate = GameState::Playing;
                    verticalVelocity = 0.0f;
                    score = 0;
                    // Reset bird and pipes
                    Player.setPosition(0, 300);
                    pipespritedown.setPosition(600, 650);
                    pipespriteup.setPosition(600, 0);
                    voiceSound.play();
                    gameIsOver = false;
                }

            }
            else {
                gameIsOver = true;
            }
        }


        if (gameIsOver == false) {
            // Draw your game objects here
            window.clear();
            // Draw the background
            window.draw(background);

            // Draw the player
            window.draw(Player);

            // Draw pip down
            window.draw(pipespritedown);

            // Draw pip up
            window.draw(pipespriteup);

            // Draw game objects
            window.display();

        }


        else {
            window.clear();
            // Draw game over window
            window.draw(gameOverSprite);

            window.draw(scoreText);

            // Draw game over text, buttons, or any other elements
            // indicating the game is over
            window.display();


        }
    }
    if (gamestate == GameState::GameOver)
        gamestate = GameState::Playing;

    return 0;
}