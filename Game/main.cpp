#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>


int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    unsigned int screenWidth = sf::VideoMode::getDesktopMode().width;
    unsigned int screenHeight = sf::VideoMode::getDesktopMode().height;

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Game", sf::Style::Fullscreen);
    
    sf::Texture bucketTexture;
    if (!bucketTexture.loadFromFile("assets/bucket.png")) {
        return -1;
    }
    sf::Sprite bucket(bucketTexture);
    unsigned int bucketWidth = bucket.getGlobalBounds().width;
    unsigned int bucketHeight = bucket.getGlobalBounds().height;


    bucket.setScale(5.f, 5.f);
    bucket.setPosition(screenWidth / 2, screenHeight - bucketHeight);




    sf::Texture appleTexture;
    if (!appleTexture.loadFromFile("assets/apple.png")) {
        return -1;
    }
    sf::Sprite apple(appleTexture);
    apple.setScale(3.f, 3.f);
    apple.setPosition(static_cast<float>(std::rand() % screenWidth), 0.7f);






    sf::Font font; 
    if (!font.loadFromFile("fonts/KOMIKAP_.ttf")) {
        return -1;
    }

    int score = 0;

    sf::Text scoreText; 
    scoreText.setFont(font);
    scoreText.setCharacterSize(48);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(10.f, 10.f);

    float bucketSpeed = 1.0f;
    float appleSpeed = .2f;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            bucket.move(-bucketSpeed, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            bucket.move(bucketSpeed, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }


        if (bucket.getPosition().x < 0) {
            bucket.setPosition(0, bucket.getPosition().y);
        }

        if (bucket.getPosition().x + bucket.getGlobalBounds().width > 2000) {
            bucket.setPosition(2000 - bucket.getGlobalBounds().width, bucket.getPosition().y);
        }

        apple.move(0.f, appleSpeed);

        if (apple.getGlobalBounds().intersects(bucket.getGlobalBounds())){
            score++;
            apple.setPosition(static_cast<float>(std::rand() % screenWidth), 0.7f);
        }

        if (apple.getPosition().y > screenHeight) {
            apple.setPosition(static_cast<float>(std::rand() % screenWidth), 0.7f);
        }

        scoreText.setString("Score " + std::to_string(score));

        window.clear(sf::Color::White);
        window.draw(bucket);
        window.draw(apple);
        window.draw(scoreText);
        window.display();
    }
}