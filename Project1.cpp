#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <SFML/Audio.hpp>


class Bullet {
public:
    sf::RectangleShape shape;
    float speed;


    Bullet(float x, float y, float speed) {
        shape.setSize(sf::Vector2f(10, 5));
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(x, y);
        this->speed = speed;
    }


    void move(float deltaTime) {
        shape.move(0, -speed * deltaTime);
    }
};

// Classe représentant une balle d'ennemi
class EnemyBullet {
public:
    sf::RectangleShape shape;
    float speed;

    // Constructeur pour initialiser la balle d'ennemi
    EnemyBullet(float x, float y, float speed) {
        shape.setSize(sf::Vector2f(10, 5));
        shape.setFillColor(sf::Color::Green);
        shape.setPosition(x, y);
        this->speed = speed;
    }


    void move(float deltaTime) {
        shape.move(0, speed * deltaTime);
    }
};


class Enemy {
public:
    sf::Sprite sprite;
    float speed;
    float shootTimer;
    float shootInterval;

    // Constructeur pour initialiser l'ennemi
    Enemy(float x, float y, float speed, const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setScale(0.15f, 0.15f);
        sprite.setPosition({ x, y });
        this->speed = speed;
        shootTimer = 0.0f;
        shootInterval = 3.0f;
    }


    void move(float deltaTime) {
        sprite.move(0, speed * deltaTime);
    }


    void updateShoot(float deltaTime, std::vector<EnemyBullet>& enemyBullets) {
        shootTimer += deltaTime;
        if (shootTimer >= shootInterval) {
            shootTimer = 0.0f;
            enemyBullets.push_back(EnemyBullet(sprite.getPosition().x, sprite.getPosition().y + sprite.getGlobalBounds().height, 150.0f));

        }
    }
};

int health = 3;
int score = 0;


void Updatetext(sf::RenderWindow& window) {
    sf::Font font;


    if (!font.loadFromFile("C:\\Users\\tgermain\\source\\repos\\Project1\\Project1\\x64\\Release\\Starjhol.ttf")) {
        std::cerr << "Erreur de chargement de la police !" << std::endl;
    }


    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 10.f);
    scoreText.setString("Score: " + std::to_string(score));


    sf::Text healthText;
    healthText.setFont(font);
    healthText.setCharacterSize(24);
    healthText.setFillColor(sf::Color::Green);
    healthText.setPosition(10.f, 40.f);
    healthText.setString("Health: " + std::to_string(health));


    window.draw(scoreText);
    window.draw(healthText);
}

void mainMenu(sf::RenderWindow& window, bool& gameStarted) {
    sf::Font font;
    if (!font.loadFromFile("C:\\Users\\tgermain\\source\\repos\\Project1\\Project1\\x64\\Release\\Starjhol.ttf")) {
        std::cerr << "Erreur de chargement de la police !" << std::endl;
        return;
    }


    sf::Text titleText;
    titleText.setFont(font);
    titleText.setCharacterSize(100);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setString("@");
    titleText.setPosition(window.getSize().x / 2 - titleText.getLocalBounds().width / 3, window.getSize().y / 5);


    sf::Text startText;
    startText.setFont(font);
    startText.setCharacterSize(20);
    startText.setFillColor(sf::Color::Yellow);
    startText.setString("Press Enter to Start");
    startText.setPosition(window.getSize().x / 2 - startText.getLocalBounds().width / 2, window.getSize().y / 2);


    window.clear();
    window.draw(titleText);
    window.draw(startText);
    window.display();


    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            gameStarted = true;
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Fenêtre SFML");


    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("C:\\Users\\tgermain\\source\\repos\\Project1\\Project1\\x64\\Release\\background.jpg")) {
        std::cerr << "Erreur de chargement de la texture du fond !" << std::endl;
        return -1;
    }

    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(sf::Vector2f{ 3.5f, 2.0f });

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("C:\\Users\\tgermain\\source\\repos\\Project1\\Project1\\x64\\Release\\R.png")) {
        return -1;
    }

    sf::Sprite playerSprite;
    playerSprite.setOrigin((sf::Vector2f)playerTexture.getSize() / 2.0f);
    playerSprite.setTexture(playerTexture);
    playerSprite.setScale(sf::Vector2f{ 0.1f, 0.1f });
    playerSprite.setPosition(sf::Vector2f{ 400, 400 });


    sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile("C:\\Users\\tgermain\\source\\repos\\Project1\\Project1\\x64\\Release\\TIEV2.png")) {
        std::cerr << "Erreur de chargement de la texture de l'ennemi !" << std::endl;
        return -1;
    }


    sf::SoundBuffer menuMusicBuffer;
    if (!menuMusicBuffer.loadFromFile("C:\\Users\\tgermain\\source\\repos\\Project1\\Project1\\x64\\Release\\Star-Wars-_Main-Theme_.ogg")) {
        std::cout << "Erreur : Impossible de charger la musique du menu." << std::endl;
        return -1;
    }

    sf::Sound menuMusic;
    menuMusic.setBuffer(menuMusicBuffer);
    menuMusic.setLoop(true);
    menuMusic.play();


    sf::SoundBuffer gameMusicBuffer;
    if (!gameMusicBuffer.loadFromFile("C:\\Users\\tgermain\\source\\repos\\Project1\\Project1\\x64\\Release\\Anakin-vs.-Obi-Wan.ogg")) {
        std::cout << "Erreur : Impossible de charger la musique du jeu." << std::endl;
        return -1;
    }

    sf::Sound gameMusic;
    gameMusic.setBuffer(gameMusicBuffer);
    gameMusic.setLoop(true);
    gameMusic.play();

    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    std::vector<EnemyBullet> enemyBullets;

    std::srand(static_cast<unsigned int>(std::time(0)));
    float enemySpeed = 50.0f;

    sf::Clock clock;  // Horloge pour mesurer le temps écoulé

    bool isExploded = false;
    bool gameOver = false;
    bool gameStarted = false;

    float enemySpawnTimer = 0.0f;
    float enemySpawnInterval = 1.0f;

    // Affichage du menu principal jusqu'à ce que le jeu commence
    while (window.isOpen() && !gameStarted) {
        mainMenu(window, gameStarted);
    }

    menuMusic.stop();
    gameMusic.play();

    // Boucle de jeu principale
    while (window.isOpen() && gameStarted) {
        float deltaTime = clock.restart().asSeconds();  // Calculer le temps écoulé depuis la dernière image

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (!gameOver) {
            // Contrôles du joueur (déplacement)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                playerSprite.move(0, -200.0f * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                playerSprite.move(0, 200.0f * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                playerSprite.move(-200.0f * deltaTime, 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                playerSprite.move(200.0f * deltaTime, 0);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !gameOver) {
            bullets.push_back(Bullet(playerSprite.getPosition().x, playerSprite.getPosition().y, 300.0f));
        }

        // Déplacer les projectiles du joueur
        for (size_t i = 0; i < bullets.size(); ++i) {
            bullets[i].move(deltaTime);
        }

        // Déplacer les ennemis et gérer les tirs
        for (size_t i = 0; i < enemies.size(); ++i) {
            enemies[i].move(deltaTime);
            enemies[i].updateShoot(deltaTime, enemyBullets);
            if (enemies[i].sprite.getPosition().y >= window.getSize().y) {
                gameOver = true;
            }
        }

        //les projectiles des ennemis et  les collisions
        for (size_t i = 0; i < enemyBullets.size(); ++i) {
            enemyBullets[i].move(deltaTime);
            if (playerSprite.getGlobalBounds().intersects(enemyBullets[i].shape.getGlobalBounds())) {
                health--;
                enemyBullets.erase(enemyBullets.begin() + i);
                if (health <= 0) {
                    gameOver = true;
                }
                break;
            }
        }

        //  les collisions avec les ennemis
        for (size_t i = 0; i < enemies.size(); ++i) {
            if (playerSprite.getGlobalBounds().intersects(enemies[i].sprite.getGlobalBounds())) {
                health--;
                enemies.erase(enemies.begin() + i);

                if (health <= 0) {
                    gameOver = true;
                }
                break;
            }
        }

        // collisions avec les projectiles du joueur
        for (size_t i = 0; i < bullets.size(); ++i) {
            for (size_t j = 0; j < enemies.size(); ++j) {
                if (bullets[i].shape.getGlobalBounds().intersects(enemies[j].sprite.getGlobalBounds())) {
                    bullets.erase(bullets.begin() + i);
                    enemies.erase(enemies.begin() + j);
                    score += 1;
                    break;
                }
            }
        }

        // Apparition des ennemis
        enemySpawnTimer += deltaTime;
        if (enemySpawnTimer >= enemySpawnInterval) {
            enemySpawnTimer = 0.0f;
            enemies.push_back(Enemy(rand() % 700 + 50, 0, enemySpeed, enemyTexture));
        }

        window.clear();

        window.draw(backgroundSprite);

        // Dessiner tous les éléments du jeu
        if (!isExploded && !gameOver) {
            window.draw(playerSprite);
        }

        for (const auto& enemy : enemies) {
            window.draw(enemy.sprite);
        }

        for (const auto& enemyBullet : enemyBullets) {
            window.draw(enemyBullet.shape);
        }

        for (const auto& bullet : bullets) {
            window.draw(bullet.shape);
        }

        // Afficher les informations de score et de santé
        Updatetext(window);

        window.display();

        if (gameOver) {
            sf::sleep(sf::seconds(2));
            window.close();
        }
    }

    return 0;
}
