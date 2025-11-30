#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>

struct Platform {
    sf::Sprite sprite;
    bool hasApple;
    bool hasGoldenApple;

    Platform(sf::Sprite sprite) {
        this->sprite = sprite;
        this->hasApple = false;
        this->hasGoldenApple = false;
    }
};

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
    float bucketSpeed = 7.f;

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
    bool fallingApple = false;

    //Initialize mrBean 
    sf::Texture mrBeanTexture;
    if (!mrBeanTexture.loadFromFile("assets/mrkidneybean.png")) {
        return -1;
    }
    sf::Sprite mrBean(mrBeanTexture);
    mrBean.setScale(1.5f, 1.5f);
    mrBean.setPosition((screenWidth / 2) - (mrBean.getGlobalBounds().width / 2), 0.f - mrBean.getGlobalBounds().height);
    float mrBeanSpeed = 3.f;
    float mrBeanJumpSpeed = -14.5f;
    bool mrBeanActive = false;

    //Initialize golden mrBean
    sf::Texture goldenBeanTexture;
    if (!goldenBeanTexture.loadFromFile("assets/goldenmrkidneybean.png")) {
        return -1;
    }
    sf::Sprite goldenBean(goldenBeanTexture);
    goldenBean.setScale(1.5f, 1.5f);

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
    std::vector<Platform> platforms(10, sf::Sprite(platformTexture));
    platforms[0].sprite.setScale(1.f, 1.f);
    float platformWidth = platforms[0].sprite.getGlobalBounds().width;
    float platformHeight = platforms[0].sprite.getGlobalBounds().height;
    //create platform in the middle that mr bean will always land on
    platforms[0].sprite.setPosition((screenWidth / 2) - (platformWidth / 2), screenHeight - (2 * platformHeight));
    //create random platforms above the starter platform
    for (size_t i = 1; i < platforms.size(); i++) {
        platforms[i].sprite.setScale(1.f, 1.f);
        float platformX = rand() % static_cast<int>(screenWidth - platformWidth);
        float platformY = screenHeight - ((i + 1) * 75);
        platforms[i].sprite.setPosition(platformX, platformY);
    }

    //Initialize platform apples
    sf::Sprite platformApple(appleTexture);
    platformApple.setScale(1.4f, 1.4f);
    platformApple.setPosition(-100.f, -100.f);
    //track which platform has the apple
    size_t applePlatform;

    //Initialize golden platform apples
    sf::Texture goldenAppleTexture;
    if (!goldenAppleTexture.loadFromFile("assets/goldenapple.png")) {
        return -1;
    }
    sf::Sprite goldenApple(goldenAppleTexture);
    goldenApple.setScale(1.4f, 1.4f);
    goldenApple.setPosition(-100.f, -100.f);
    //track which platform has the golden apple
    size_t goldenApplePlatform;

    //initialize sounds
    sf::SoundBuffer thunkBuffer;
    if (!thunkBuffer.loadFromFile("sounds/thunk.wav")) {
        return -1;
    }
    sf::Sound thunkSound(thunkBuffer);

    sf::SoundBuffer failBuffer;
    if (!failBuffer.loadFromFile("sounds/fail.wav")) {
        return -1;
    }
    sf::Sound failSound(failBuffer);

    sf::SoundBuffer transitionBuffer;
    if (!transitionBuffer.loadFromFile("sounds/transition.wav")) {
        return -1;
    }
    sf::Sound transitionSound(transitionBuffer);

    sf::SoundBuffer jumpBuffer;
    if (!jumpBuffer.loadFromFile("sounds/jump.wav")) {
        return -1;
    }
    sf::Sound jumpSound(jumpBuffer);
    jumpSound.setVolume(70);

    sf::SoundBuffer crunchBuffer;
    if (!crunchBuffer.loadFromFile("sounds/crunch.wav")) {
        return -1;
    }
    sf::Sound crunchSound(crunchBuffer);

    sf::SoundBuffer powerupBuffer;
    if (!powerupBuffer.loadFromFile("sounds/powerup.wav")) {
        return -1;
    }
    sf::Sound powerupSound(powerupBuffer);

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
    scoreText.setCharacterSize(32);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(10.f, 10.f);
    
    //initalize score elements
    sf::Text loseText;
    loseText.setFont(loseFont);
    loseText.setCharacterSize(32);
    loseText.setFillColor(sf::Color::Black);
    loseText.setPosition((screenWidth / 4), screenHeight / 5);

    //initalize score elements
    sf::Text winText;
    winText.setFont(winFont);
    winText.setCharacterSize(48);
    winText.setFillColor(sf::Color::Black);
    winText.setPosition((screenWidth / 4), screenHeight / 2);

    //initalize health bar elements
    std::vector<sf::Sprite> healthBar(3, sf::Sprite(mrBeanTexture));
    std::vector<sf::Sprite> goldBar(3, sf::Sprite(goldenBeanTexture));
    for (size_t i = 0; i < 3; i++) {
        healthBar[i].setScale(.7f, .7f);
        goldBar[i].setScale(.7f, .7f);
        healthBar[i].setPosition(10.f + (1.3 * healthBar[i].getGlobalBounds().width * i), 80.f);
        goldBar[i].setPosition(10.f + (1.3 * goldBar[i].getGlobalBounds().width * i), 90.f + goldBar[i].getGlobalBounds().height);
    }
    
    //initialize game control variable
    int gameStage = 1;
    int time = 180;
    int lives = 3;
    int gold = 0;
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
                    thunkSound.play();
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
                    transitionSound.play();
                    apple.setPosition(-100, -100);
                    mrBean.setPosition(-100, -100);
                    window.clear(sf::Color::White);
                    window.draw(bucket);
                    window.display();
                    break;
                }

                //if mr bean not saved game over
                if (mrBean.getPosition().y > screenHeight) {
                    failSound.play();
                    gameStage = 49;
                    break;
                }

                //update score
                scoreText.setString("SCORE " + std::to_string(score));
                
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
                    mrBean.setPosition((screenWidth / 2) - (mrBean.getGlobalBounds().width / 2), 0.f - mrBean.getGlobalBounds().height);
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
                //once mr bean has landed move to next stage
                if (mrBean.getGlobalBounds().intersects(platforms[0].sprite.getGlobalBounds())){
                    gameStage = 4;
                }

                //clear screen and draw objects
                window.clear();
                window.draw(background);
                window.draw(mrBean);
                for (auto& platform : platforms) {
                    window.draw(platform.sprite);
                }
                window.display();
                break;
            //transition phase start screen for jumper segment
            case 4:
                //once space is pressed move to next stage
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    mrBeanSpeed = 10.f;
                    //reset score
                    score = 0;
                    gameStage = 5;
                }
                //clear screen and draw objects
                window.clear();
                window.draw(background);
                window.draw(mrBean);
                for (auto& platform : platforms) {
                    window.draw(platform.sprite);
                }
                window.display();
                break;
            //jumper game segment
            case 5:
                //handle movement input
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    mrBean.move(-mrBeanSpeed, 0.f);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    mrBean.move(mrBeanSpeed, 0.f);
                }
                //bounds check for left and right wall on player, roll character to opposite side for wrap around effect
                if (mrBean.getPosition().x <= -mrBean.getGlobalBounds().width) {
                    mrBean.setPosition(screenWidth, mrBean.getPosition().y);
                }
                if (mrBean.getPosition().x > screenWidth) {
                    mrBean.setPosition(0, mrBean.getPosition().y);
                }

                //handle collisions and gravity for player
                for (auto& platform : platforms) {
                    if (mrBean.getGlobalBounds().intersects(platform.sprite.getGlobalBounds()) && (mrBean.getPosition().y + (mrBean.getGlobalBounds().height / 5)) < platform.sprite.getPosition().y && mrBeanJumpSpeed > 0) {
                        mrBeanJumpSpeed = -14.5f;
                        jumpSound.play();
                        break;
                    }
                }
                if (mrBean.getGlobalBounds().intersects(platformApple.getGlobalBounds())) {
                    lives--;
                    crunchSound.play();
                    if (gold > 0) {
                        gold--;
                    }
                    platformApple.setPosition(-100.f, -100.f);
                    platforms[applePlatform].hasApple = false;
                }
                if (mrBean.getGlobalBounds().intersects(apple.getGlobalBounds())) {
                    lives--;
                    crunchSound.play();
                    if (gold > 0) {
                        gold--;
                    }
                    apple.setPosition(-100.f, -100.f);
                    fallingApple = false;
                }
                if (mrBean.getGlobalBounds().intersects(goldenApple.getGlobalBounds())) {
                    if (lives < 3) {
                        lives++;
                    }
                    if (gold < 3) {
                        gold++;
                    }
                    powerupSound.play();
                    goldenApple.setPosition(-100.f, -100.f);
                    platforms[goldenApplePlatform].hasGoldenApple = false;
                }

                mrBeanJumpSpeed += .4f;
                mrBean.move(0, mrBeanJumpSpeed);
         
                //handle movement and spawning for all platforms and their associated apples
                for (size_t i = 0; i < platforms.size(); i++) {
                    //when player is moving up and within the top half of the screen
                    if (mrBeanJumpSpeed < 0 && mrBean.getPosition().y < (screenHeight / 2) ) {
                        //move all platforms down based on players current speed
                        platforms[i].sprite.setPosition(platforms[i].sprite.getPosition().x, platforms[i].sprite.getPosition().y - mrBeanJumpSpeed);
                        
                        //move all associated apples down as well
                        if (platforms[i].hasApple) {
                            platformApple.setPosition(platforms[i].sprite.getPosition().x + (platformWidth / 2) - (platformApple.getGlobalBounds().width / 2), platforms[i].sprite.getPosition().y - platformApple.getGlobalBounds().height);
                        }
                        if (platforms[i].hasGoldenApple) {
                            goldenApple.setPosition(platforms[i].sprite.getPosition().x + (platformWidth / 2) - (goldenApple.getGlobalBounds().width / 2), platforms[i].sprite.getPosition().y - goldenApple.getGlobalBounds().height);
                        }
                    }
                    //once a platform goes below the screen 
                    if (platforms[i].sprite.getPosition().y > screenHeight) {
                        //respawn that platform on top and increase score
                        score++;
                        platforms[i].sprite.setPosition(rand() % static_cast<int>(screenWidth - platformWidth), 0);
                        
                        //remove its associated apple from the playfield
                        if (platforms[i].hasApple) {
                            platformApple.setPosition(-100.f, -100.f);
                            platforms[i].hasApple = false;
                        }
                        if (platforms[i].hasGoldenApple) {
                            goldenApple.setPosition(-100.f, -100.f);
                            platforms[i].hasGoldenApple = false;
                        }

                        //when the score reaches a certain value attach either an apple or a golden apple to the next spawning platform
                        if (score % 25 == 0 && score > 0) {
                            platforms[i].hasApple = true;
                            applePlatform = i;
                        }
                        if (score % 40 == 0 && score > 0) {
                            platforms[i].hasGoldenApple = true;
                            goldenApplePlatform = i;
                        }
                    }
                }
                
                if (score % 35 == 0 && score > 0 && fallingApple == false) {
                    fallingApple = true;
                    apple.setPosition(static_cast<float>(std::rand() % appleRandOffset), -100.f);
                }
                if (fallingApple) {
                    apple.move(0.f, appleSpeed);
                }
                //bounds check for apple and ground, reset apple on contact
                if (apple.getPosition().y > screenHeight) {
                    apple.setPosition(static_cast<float>(std::rand() % appleRandOffset), -100.f);
                    fallingApple = false;
                }

                //handle win/lose conditions
                if (score > 300) {
                    if (gold == 3) {
                        gameStage = 7;
                    }
                    else {
                        gameStage = 6;
                    }
                }
                if ((mrBean.getPosition().y > screenHeight) || lives == 0){
                    failSound.play();
                    gameStage = 49;
                }

                //update score
                scoreText.setString("SCORE \n  " + std::to_string(score));

                window.clear();
                window.draw(background);
                window.draw(mrBean);
                for (auto& platform : platforms) {
                    window.draw(platform.sprite);
                }
                window.draw(platformApple);
                window.draw(goldenApple);
                window.draw(apple);
                for (size_t i = 0; i < lives; i++) {
                    window.draw(healthBar[i]);
                }
                for (size_t i = 0; i < gold; i++) {
                    window.draw(goldBar[i]);
                }
                window.draw(scoreText);
                window.display();
                break;
            case 6:

                break;
            case 7:
                break;
            case 49:
                loseText.setString("\t\tGAME OVER \n\tMr. Kidney Bean \n\tdid not make it.");
                window.clear(sf::Color::White);
                window.draw(loseText);
                window.display();
                break;
            default:
                return -2;
        }
    }
}