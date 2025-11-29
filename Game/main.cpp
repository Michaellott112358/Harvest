#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>

int main()
{
    //seed random
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    //initialize window size variables and establish render context
    unsigned int screenWidth = 900;
    unsigned int screenHeight = 900;
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Harvest");
    window.setFramerateLimit(60);

    //Initlaize bucket
    sf::Texture bucketTexture;
    if (!bucketTexture.loadFromFile("assets/bucket.png")) {
        return -1;
    }
    sf::Sprite bucket(bucketTexture);
    unsigned int bucketWidth = bucket.getGlobalBounds().width;
    unsigned int bucketHeight = bucket.getGlobalBounds().height;
    bucket.setScale(3.f, 3.f);
    bucket.setPosition(screenWidth / 2, screenHeight - (3.1f * bucketHeight));
    float bucketSpeed = 6.f;

    //Initialize apple
    sf::Texture appleTexture;
    if (!appleTexture.loadFromFile("assets/apple.png")) {
        return -1;
    }
    sf::Sprite apple(appleTexture);
    apple.setScale(2.f, 2.f);
    int appleRandOffset = screenWidth - apple.getGlobalBounds().width;
    apple.setPosition(static_cast<float>(std::rand() % appleRandOffset), 0.f - apple.getGlobalBounds().height);
    float appleSpeed = 7.f;

    //Initialize mrBean 
    sf::Texture mrBeanTexture;
    if (!mrBeanTexture.loadFromFile("assets/mrkidneybean.png")) {
        return -1;
    }
    sf::Sprite mrBean(mrBeanTexture);
    mrBean.setScale(1.5f, 1.5f);
    mrBean.setPosition(screenWidth / 2, 0.f - mrBean.getGlobalBounds().height);
    float mrBeanSpeed = 3.f;
    float mrBeanJumpSpeed = 0.f;

    //Initialize Background
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/background.png")) {
        return -1;
    }
    sf::Sprite background(backgroundTexture);
    background.setScale(7.f, 7.f);
    background.setPosition(0, 0);

    //Initialize Platforms
    sf::Texture platformTexture;
    if (!platformTexture.loadFromFile("assets/platform.png")) {
        return -1;
    }
    std::vector<sf::Sprite> platforms(10, sf::Sprite(platformTexture));
    platforms[0].setScale(1.f, 1.f);
    float platformWidth = platforms[0].getGlobalBounds().width;
    float platformHeight = platforms[0].getGlobalBounds().height;
    platforms[0].setPosition((screenWidth / 2) - (platformWidth / 2), screenHeight - (2 * platformHeight));
    for (size_t i = 1; i < platforms.size(); i++) {
        
        platforms[i].setScale(1.f, 1.f);
        float platformX = rand() % static_cast<int>(screenWidth - platformWidth);
        float platformY = screenHeight - ((i + 1) * 75);
        platforms[i].setPosition(platformX, platformY);
    }

    //Initialize fonts
    sf::Font scoreFont; 
    if (!scoreFont.loadFromFile("fonts/Segment16CBold.ttf")) {
        return -1;
    }
    sf::Font loseFont;
    if (!loseFont.loadFromFile("fonts/SquaresBold.otf")) {
        return -1;
    }
    sf::Font winFont;
    if (!winFont.loadFromFile("fonts/SquaresBold.otf")) {
        return -1;
    }
    sf::Font dialogueFont;
    if (!dialogueFont.loadFromFile("fonts/SquaresBold.otf")) {
        return -1;
    }

    //initalize score elements
    int score = 0;
    sf::Text scoreText; 
    scoreText.setFont(scoreFont);
    scoreText.setCharacterSize(48);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition((screenWidth / 2), 10.f);
    
    //initalize score elements
    sf::Text loseText;
    loseText.setFont(loseFont);
    loseText.setCharacterSize(48);
    loseText.setFillColor(sf::Color::Black);
    loseText.setPosition((screenWidth / 4), screenHeight / 5);

    //initalize score elements
    sf::Text winText;
    winText.setFont(winFont);
    winText.setCharacterSize(48);
    winText.setFillColor(sf::Color::Black);
    winText.setPosition((screenWidth / 4), screenHeight / 2);

    //initialize game control variable
    int gameStage = 3;
    bool mrBeanActive = false;
    int time = 180;
    float bucketScale = 5.f;

    //game loop 
    while (window.isOpen())
    {
        //initialize event & handle window closed event
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        //exit game if escape is pressed
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }
       
        switch (gameStage){
            //Starting stage, apple catch segment
            case 1:
                //handle movement input
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    bucket.move(-bucketSpeed, 0.f);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    bucket.move(bucketSpeed, 0.f);
                }
                //bounds check for left and right wall on bucket
                if (bucket.getPosition().x < 0) {
                    bucket.setPosition(0, bucket.getPosition().y);
                }
                if (bucket.getPosition().x + bucket.getGlobalBounds().width > screenWidth) {
                    bucket.setPosition(screenWidth - bucket.getGlobalBounds().width, bucket.getPosition().y);
                }

                //when active move mr bean
                if (!mrBeanActive) {
                    //move the apple
                    apple.move(0.f, appleSpeed);
                }
                else {
                    mrBean.move(0.f, mrBeanSpeed);
                }

                //bounds check for apple and bucket, reset apple on contact
                if ((apple.getGlobalBounds().intersects(bucket.getGlobalBounds())) && (apple.getPosition().y < bucket.getPosition().y)) {
                    score++;
                    //activate mrBean randomly once score goes above 30
                    if (score > 20) {
                        //if (std::rand() % (50 - score) + 1 == 1) {
                            mrBeanActive = true;
                            apple.setPosition(-500.f,-500.f);
                    }else {
                            apple.setPosition(static_cast<float>(std::rand() % appleRandOffset), 0.7f);
                    }
                }
                //bounds check for apple and ground, reset apple on contact
                if (apple.getPosition().y > screenHeight) {
                    apple.setPosition(static_cast<float>(std::rand() % appleRandOffset), 0.7f);
                }
                
                //determine if mr bean will spawn then begin scene transition
                if (mrBean.getGlobalBounds().intersects(bucket.getGlobalBounds())) {
                    gameStage = 2;
                    apple.setPosition(-100, -100);
                    mrBean.setPosition(-100, -100);
                    scoreText.setPosition(-100, -100);
                    window.clear(sf::Color::White);
                    window.draw(bucket);
                    window.display();
                    break;
                }

                //if mr bean not saved game over
                if (mrBean.getPosition().y > screenHeight) {
                    gameStage = 49;
                    break;
                }

                //update score
                scoreText.setString("SCORE \n  " + std::to_string(score));
                
                //clear window and draw elements
                window.clear(sf::Color::White);
                window.draw(bucket);
                window.draw(apple);
                window.draw(mrBean);
                window.draw(scoreText);
                window.display();

                break;
            //transition
            case 2:
                if (time <= 0) {
                    //once this phase is over remove the bucket and set the background, move to next phase
                    bucket.setScale(1.f,1.f);
                    bucket.setPosition(-500, -500);
                    mrBean.setScale(2.f, 2.f);
                    mrBean.setPosition(screenWidth / 2, 0.f - mrBean.getGlobalBounds().height);
                    window.clear();
                    window.draw(background);
                    window.draw(bucket);
                    window.display();
                    gameStage = 3;
                    break;
                }
                //grow bucket scale while moving it to the center
                bucket.setScale(bucketScale, bucketScale);
                bucketScale += 1.f;
                bucket.setPosition(bucket.getPosition().x - 8.f, bucket.getPosition().y - 9.f);
                time--;
                window.clear(sf::Color::White);
                window.draw(bucket);
                window.display();
                break;
            //transition mr bean falling
            case 3:
                mrBean.move(0.f, mrBeanSpeed);
                if (mrBean.getGlobalBounds().intersects(platforms[0].getGlobalBounds())){
                    gameStage = 4;
                }
                window.clear();
                window.draw(background);
                window.draw(mrBean);
                for (auto& platform : platforms) {
                    window.draw(platform);
                }
                window.display();
                break;
            //jumping game phase
            case 4:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    gameStage = 5;
                }
                window.clear();
                window.draw(background);
                window.draw(mrBean);
                for (auto& platform : platforms) {
                    window.draw(platform);
                }
                window.display();
                break;
            case 5:
                //handle movement input
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    mrBean.move(-mrBeanSpeed, 0.f);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    mrBean.move(mrBeanSpeed, 0.f);
                }

                //handle collision and gravity
                for (auto& platform : platforms) {
                    if (mrBean.getGlobalBounds().intersects(platform.getGlobalBounds()) && mrBean.getPosition().y < platform.getPosition().y && mrBeanJumpSpeed > 0) {
                        mrBeanJumpSpeed = -15.f;
                        break;
                    }
                }
                mrBeanJumpSpeed += .4f;
                mrBean.move(0, mrBeanJumpSpeed);
                std::cout << "mr bean speed: " << mrBeanJumpSpeed << "coordinates X: " << mrBean.getPosition().x << " Y: " << mrBean.getPosition().y << std::endl;
                if ((mrBean.getPosition().y > screenHeight)) {
                    gameStage = 49;
                }

                for (auto& platform : platforms) {
                    if (mrBeanJumpSpeed < 0 && mrBean.getPosition().y < (screenHeight / 3) ) {
                        platform.setPosition(platform.getPosition().x, platform.getPosition().y - mrBeanJumpSpeed);
                    }
                    if (platform.getPosition().y > screenHeight) {
                        std::cout << "test";
                        platform.setPosition(rand() % static_cast<int>(screenWidth - platformWidth), 0);
                    }
                }

                window.clear();
                window.draw(background);
                window.draw(mrBean);
                for (auto& platform : platforms) {
                    window.draw(platform);
                }
                window.display();
                break;
            case 6:

                break;
            case 49:
                loseText.setString("\t\t\t\t\t\tGAME OVER \n\n\n\n\n\n\n\n\n\n   Mr. Kidney Bean did not make it.");
                window.clear(sf::Color::White);
                window.draw(loseText);
                window.display();
                break;
            default:
                return -2;
        }
    }
}