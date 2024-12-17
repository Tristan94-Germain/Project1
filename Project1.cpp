#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>


sf::Texture EnnemyBullets;
sf::Texture SniperBullets;
enum Difficulty {
    Easy,
    Medium,
    Hard
};


Difficulty difficulty;


// Classe représentant un projectile du joueur
class Bullet {
public:
    sf::Sprite sprite;
    float speed;

    Bullet(float x, float y, float speed, const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setScale(sf::Vector2f(0.01, 0.01));
        sprite.setPosition(x, y);
        this->speed = speed;
    }

    void move(float deltaTime) {
        sprite.move(0, -speed * deltaTime);
    }
};
class LaserBullet {
public:
    sf::RectangleShape shape;
    int damage;

    LaserBullet(float x, float y) {
        shape.setSize(sf::Vector2f(10.f, 500.f));
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(x, y);
        damage = 3;

    }

    void move(float deltaTime) {
        shape.move(0.f, -500.f * deltaTime);
    }
};





// Classe représentant un projectile de l'ennemi
class EnemyBullet {
public:
    sf::Sprite sprite;
    float speed;

    EnemyBullet(float x, float y, float speed, const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setScale(sf::Vector2f(0.01, 0.01));
        sprite.setPosition(x, y);

        (difficulty == 1) ? this->speed = speed * 2  : this->speed = speed;
        (difficulty == 2) ? this->speed = speed * 5 : this->speed = speed;

    }

    void move(float deltaTime) {
        sprite.move(0, speed * deltaTime);
    }
};

class SniperBullet {
public:
    sf::Sprite sprite;
    sf::Vector2f direction;
    float speed;

    SniperBullet(float x, float y, sf::Vector2f playerPosition, float speed, const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setScale(sf::Vector2f(0.01, 0.01));
        sprite.setPosition(x, y);
        (difficulty == 1) ? this->speed = speed * 2: this->speed = speed;
        (difficulty == 2) ? this->speed = speed * 5 : this->speed = speed;

        // Calculer la direction vers le joueur
        sf::Vector2f delta = playerPosition - sf::Vector2f(x, y);
        float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
        direction = delta / length;
    }

    void move(float deltaTime) {
        sprite.move(direction * speed * deltaTime);
    }
};



// Classe représentant un ennemi de base
class Enemy {
public:
    sf::Sprite sprite;

    float speed;
    float shootTimer;
    float shootInterval;

    Enemy(float x, float y, float speed, const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setScale(0.1f, 0.1f);
        sprite.setPosition({ x, y });
        (difficulty == 1) ? this->speed = 100 : this->speed = speed;
        (difficulty == 2) ? this->speed = speed * 5 : this->speed = speed;
        shootTimer = 0.0f;


        (difficulty == 1) ? shootInterval = 0.5f : shootInterval = 2.0f;
        (difficulty == 2) ? shootInterval = 0.5f : shootInterval = 2.0f;
    }


    void move(float deltaTime) {
        sprite.move(0, speed * deltaTime);
    }

    void updateShoot(float deltaTime, std::vector<EnemyBullet>& enemyBullets) {
        shootTimer += deltaTime;
        if (shootTimer >= shootInterval) {
            shootTimer = 0.0f;
            enemyBullets.push_back(EnemyBullet(sprite.getPosition().x + sprite.getScale().x / 2, sprite.getPosition().y + sprite.getScale().y, 150.0f, EnnemyBullets));
        }
    }
};

// Classe représentant un ennemi robuste
class StrongEnemy {
public:
    sf::Sprite sprite;
    float speed;
    int resistance;
    float shootTimer;
    float shootInterval;

    StrongEnemy(float x, float y, float speed, const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setScale(0.1f, 0.1f);
        sprite.setPosition({ x,y });
        (difficulty == 1) ? this->speed = speed : this->speed = speed;
        (difficulty == 2) ? this->speed = speed * 5 : this->speed = speed;
        this->resistance = 3;
        shootTimer = 0.0f;
        (difficulty == 1) ? shootInterval = 1.5f : shootInterval = 2.0f;
        (difficulty == 2) ? shootInterval = 0.5f : shootInterval = 2.0f;
    }

    void move(float deltaTime) {
        sprite.move(0, speed * deltaTime);
    }

    void updateShoot(float deltaTime, std::vector<EnemyBullet>& enemyBullets) {
        shootTimer += deltaTime;
        if (shootTimer >= shootInterval) {
            shootTimer = 0.0f;
            enemyBullets.push_back(EnemyBullet(sprite.getPosition().x + sprite.getScale().x / 2, sprite.getPosition().y + sprite.getScale().y, 150.0f, EnnemyBullets));
        }
    }
};

class SniperEnemy {
public:
    sf::Sprite sprite;
    float speed;
    int resistance;
    float shootTimer;
    float shootInterval;

    SniperEnemy(float x, float y, float speed, const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setScale(0.08f, 0.08f);
        sprite.setPosition({ x,y });
        (difficulty == 1) ? this->speed = speed : this->speed = speed;
        (difficulty == 2) ? this->speed = speed * 5 : this->speed = speed;
        this->resistance = 2;
        shootTimer = 0.0f;
        (difficulty == 1) ? shootInterval = 1.5f : shootInterval = 2.0f;
        (difficulty == 2) ? shootInterval = 0.5f : shootInterval = 2.0f;
    }

    void move(float deltaTime) {
        sprite.move(0, speed * deltaTime);
    }

    void updateShoot(float deltaTime, std::vector<SniperBullet>& sniperBullets, sf::Vector2f playerPosition) {
        shootTimer += deltaTime;
        if (shootTimer >= shootInterval) {
            shootTimer = 0.0f;
            // Créer un nouveau SniperBullet qui suit le joueur
            sniperBullets.push_back(SniperBullet(sprite.getPosition().x + sprite.getScale().x / 2, sprite.getPosition().y + sprite.getScale().y, playerPosition, 150.0f, SniperBullets));
        }
    }
};

class Boss {
public:
    sf::Sprite sprite;
    float speed;
    int resistance;
    float shootTimer;
    float shootInterval;
    int direction;

    Boss(float x, float y, float speed, const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setScale(0.2f, 0.2f);  // Couleur du boss
        sprite.setPosition({ x,50 });
        sprite.setOrigin({ sprite.getTexture()->getSize().x / 2.f,sprite.getTexture()->getSize().y / 2.f });
        this->speed = speed * 3;
        this->resistance = 50;
        shootTimer = 0.0f;
        shootInterval = 0.2f;  // Intervalle entre les tirs du boss
        direction = 1;
    }

    void move(float deltaTime) {
        // Déplacer horizontalement en fonction de la direction
        sprite.move(direction * speed * deltaTime, 0);

        // Vérifier si le boss atteint le bord de l'écran
        if (sprite.getPosition().x <= 100 || sprite.getPosition().x + sprite.getScale().x >= 700) {
            direction *= -1;  // Inverser la direction
        }
    }



    void updateShoot(float deltaTime, std::vector<EnemyBullet>& enemyBullets) {
        shootTimer += deltaTime;
        if (shootTimer >= shootInterval) {
            shootTimer = 0.0f;
            // Le boss tire plusieurs projectiles à la fois
            enemyBullets.push_back(EnemyBullet(sprite.getPosition().x + sprite.getScale().x / 2, sprite.getPosition().y + sprite.getScale().y, 150.0f, EnnemyBullets));
            enemyBullets.push_back(EnemyBullet(sprite.getPosition().x + sprite.getScale().x / 2 - 20, sprite.getPosition().y + sprite.getScale().y, 150.0f, EnnemyBullets));
            enemyBullets.push_back(EnemyBullet(sprite.getPosition().x + sprite.getScale().x / 2 + 20, sprite.getPosition().y + sprite.getScale().y, 150.0f, EnnemyBullets));
        }
    }
};

// Classe représentant un bonus multi-tir
class MultiShotBonus {
public:
    sf::Sprite sprite;
    float speed;

    MultiShotBonus(float x, float y, const sf::Texture& texture) {
        sprite.setScale(sf::Vector2f(0.1, 0.1));
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        speed = 100.0f;
    }

    void move(float deltaTime) {
        sprite.move(0, speed * deltaTime);
    }
};

class ShieldBonus {
public:
    sf::Sprite sprite;
    float speed;

    ShieldBonus(float x, float y, const sf::Texture& texture) {
        sprite.setScale(sf::Vector2f(0.1, 0.1));
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        speed = 100.0f;
    }


    void move(float deltaTime) {
        sprite.move(0, speed * deltaTime);
    }
};


class LaserBonus {
public:
    sf::Sprite sprite;
    float speed;
    bool isActive;


    LaserBonus(float x, float y, const sf::Texture& texture) {
        sprite.setScale(sf::Vector2f(0.1, 0.1));
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        speed = 100.0f;
        isActive = true;

    }


    void move(float deltaTime) {
        sprite.move(0.f, 100.f * deltaTime);
    }
};




















































// Variables globales pour la santé et le score
int health = 100;
int score = 0;

// Fonction pour mettre à jour l'affichage du score et de la santé
void Updatetext(sf::RenderWindow& window) {
    sf::Font font;

    if (!font.loadFromFile("Asset\\Starjhol.ttf")) {
        std::cerr << "Erreur de chargement de la police !" << std::endl;
    }

    // Affichage du score
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 10.f);
    scoreText.setString("Score: " + std::to_string(score));

    // Affichage de la santé
    sf::Text healthText;
    healthText.setFont(font);
    healthText.setCharacterSize(24);
    healthText.setFillColor(sf::Color::Green);
    healthText.setPosition(10.f, 40.f);
    healthText.setString("Health: " + std::to_string(health));

    window.draw(scoreText);
    window.draw(healthText);
}






Difficulty showDifficultyMenu(sf::RenderWindow& window, bool& settingOpened, bool& skinOpened, bool& menuActif) {
    sf::Font font;
    if (!font.loadFromFile("Asset\\Starjhol.ttf")) {
        std::cerr << "Erreur de chargement de la police!" << std::endl;
        return Difficulty::Easy;
    }

    // Déclaration des textes
    sf::Text title("Orbitum Wars", font, 30);
    title.setPosition(window.getSize().x / 2.f - title.getGlobalBounds().width / 2.f, 50);

    sf::Text easyText("Easy", font, 20);
    easyText.setPosition(window.getSize().x / 2.f - easyText.getGlobalBounds().width / 2.f, 150);

    sf::Text mediumText("Medium", font, 20);
    mediumText.setPosition(window.getSize().x / 2.f - mediumText.getGlobalBounds().width / 2.f, 200);

    sf::Text hardText("Hard", font, 20);
    hardText.setPosition(window.getSize().x / 2.f - hardText.getGlobalBounds().width / 2.f, 250);

    sf::Text playText("Play", font, 20);
    playText.setPosition(window.getSize().x / 2.f - playText.getGlobalBounds().width / 2.f, 200);

    sf::Text skinText("Skin", font, 20);
    skinText.setPosition(window.getSize().x / 2.f - skinText.getGlobalBounds().width / 2.f, 250);

    sf::Text settingText("Setting", font, 20);
    settingText.setPosition(window.getSize().x / 2.f - settingText.getGlobalBounds().width / 2.f, 300);

    sf::Text selectedText(">", font, 20);
    selectedText.setFillColor(sf::Color::Red);

    int selectedIndex = 0;
    bool showDifficultyOptions = false;

    while (window.isOpen()) {
        sf::Event event;
        window.clear();

        // Affichage des menus selon l'état
        if (skinOpened) {
            // Affichage menu "Skin"
            sf::Text skinTitleText("Skin", font, 30);
            skinTitleText.setPosition(window.getSize().x / 2.f - skinTitleText.getGlobalBounds().width / 2.f, 50);
            sf::Text skinMenuText("Choose your skin... ", font, 20);
            skinMenuText.setPosition(window.getSize().x / 2.f - skinMenuText.getGlobalBounds().width / 2.f, 450);

            window.draw(skinMenuText);
            window.draw(skinTitleText);
            window.display();
            continue; // Sort de cette boucle si skinOpened est activé
        }

        if (settingOpened) {
            // Affichage menu "Settings"
            sf::Text settingTitleText("Settings", font, 30);
            settingTitleText.setPosition(window.getSize().x / 2.f - settingTitleText.getGlobalBounds().width / 2.f, 50);
            sf::Text soundText("Sound", font, 20);
            soundText.setPosition(window.getSize().x / 3.f - soundText.getGlobalBounds().width / 2.f, 150);

            window.draw(soundText);
            window.draw(settingTitleText);
            window.display();
            continue; // Sort de cette boucle si settingOpened est activé
        }

        // Affichage du menu principal si pas d'autres menus ouverts
        if (!showDifficultyOptions) {
            window.draw(title);
            window.draw(settingText);
            window.draw(skinText);
            window.draw(playText);
        }
        else {
            window.draw(title);
            window.draw(easyText);
            window.draw(mediumText);
            window.draw(hardText);
            selectedText.setPosition(window.getSize().x / 2.f - selectedText.getGlobalBounds().width / 2.f, 150 + selectedIndex * 50);
            window.draw(selectedText);
        }

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    if (showDifficultyOptions) {
                        selectedIndex = (selectedIndex > 0) ? selectedIndex - 1 : 2;
                    }
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    if (showDifficultyOptions) {
                        selectedIndex = (selectedIndex < 2) ? selectedIndex + 1 : 0;
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    if (!showDifficultyOptions) {
                        showDifficultyOptions = true;
                    }
                    else {
                        return static_cast<Difficulty>(selectedIndex);
                    }
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    skinOpened = false;
                    settingOpened = false;
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (playText.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        showDifficultyOptions = true;
                    }
                    else if (showDifficultyOptions) {
                        if (easyText.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                            return Difficulty::Easy;
                        }
                        else if (mediumText.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                            return Difficulty::Medium;
                        }
                        else if (hardText.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                            return Difficulty::Hard;
                        }
                    }
                    if (settingText.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        settingOpened = true;
                    }
                    if (skinText.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        skinOpened = true;
                    }
                }
            }
        }

        window.display();
    }

    return Difficulty::Easy;
}






































































































int main() {

    sf::RenderWindow window(sf::VideoMode(800, 800), "Fenêtre SFML", sf::Style::None);


    sf::Clock clock;

    // Variables pour gérer les défilements des fonds
    float backgroundSpeed = 100.0f;
    float backgroundSpeed2 = 30.0f;// Vitesse de défilement des fonds
    sf::Sprite backgroundSprite1, backgroundSprite2, backgroundSprite3, backgroundSprite4, backgroundSprite5;  // Sprites des backgrounds
    sf::Texture backgroundTexture1, backgroundTexture2, backgroundTexture3, backgroundTexture4, backgroundTexture5;  // Textures des backgrounds

    // Chargement des textures de fond
    backgroundTexture1.loadFromFile("Asset\\fond_du_jeu_etoile.png");
    backgroundTexture2.loadFromFile("Asset\\fond_du_jeu_etoile.png");
    backgroundTexture3.loadFromFile("Asset\fond_du_jeu_etoile_2.png");
    backgroundTexture4.loadFromFile("Asset\\fond_du_jeu_planette.png");
    backgroundTexture5.loadFromFile("Asset\\fond_planet2.png");


    // Assurez-vous que les sprites utilisent ces textures
    backgroundSprite1.setTexture(backgroundTexture1);
    backgroundSprite2.setTexture(backgroundTexture2);
    backgroundSprite3.setTexture(backgroundTexture3);
    backgroundSprite4.setTexture(backgroundTexture4);
    backgroundSprite5.setTexture(backgroundTexture5);

    // Initialisation des positions des fonds
    backgroundSprite1.setPosition(0, 0);
    backgroundSprite2.setPosition(0, -backgroundSprite2.getGlobalBounds().height);
    backgroundSprite3.setPosition(0, -backgroundSprite3.getGlobalBounds().height);
    backgroundSprite4.setPosition(0, 0);
    backgroundSprite5.setPosition(0, -backgroundSprite5.getGlobalBounds().height);





    // Vaisseau du joueur en forme de rectangle
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("Asset\\Vaisseau_joueur_1.png")) {
        return -1;
    }

    sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile("Asset\\Ennemis_basic.png")) {
        std::cerr << "Erreur de chargement de la texture de l'ennemi !" << std::endl;
        return -1;
    }

    sf::Texture enemyStrongTexture;
    if (!enemyStrongTexture.loadFromFile("Asset\\ennemis_tank1.png")) {
        std::cerr << "Erreur de chargement de la texture de l'ennemi !" << std::endl;
        return -1;
    }

    sf::Texture enemySniperTexture;
    if (!enemySniperTexture.loadFromFile("Asset\\Ennemis_Sniper.png")) {
        std::cerr << "Erreur de chargement de la texture de l'ennemi !" << std::endl;
        return -1;
    }

    sf::Texture Bosse;
    if (!Bosse.loadFromFile("Asset\\Ennemis_Boss.png")) {
        std::cerr << "Erreur de chargement de la texture de l'ennemi !" << std::endl;
        return -1;
    }

    sf::Texture Bullets;
    if (!Bullets.loadFromFile("Asset\\tir_joueur.png")) {
        std::cerr << "Erreur de chargement de la texture de l'ennemi !" << std::endl;
        return -1;
    }




    if (!EnnemyBullets.loadFromFile("Asset\\tir_Ennemis_basic.png")) {
        std::cerr << "Erreur de chargement de la texture de l'ennemi !" << std::endl;
        return -1;
    }

    if (!SniperBullets.loadFromFile("Asset\\tir_Ennemis_Sniper.png")) {
        std::cerr << "Erreur de chargement de la texture de l'ennemi !" << std::endl;
        return -1;
    }

    sf::Texture MultiTirs;
    if (!MultiTirs.loadFromFile("Asset\\Bonus_multi-tir.png")) {
        std::cerr << "Erreur de chargement de la texture de l'ennemi !" << std::endl;
        return -1;
    }

    sf::Texture Shiledss;
    if (!Shiledss.loadFromFile("Asset\\Bonus_bouclier.png")) {
        std::cerr << "Erreur de chargement de la texture de l'ennemi !" << std::endl;
        return -1;
    }


    sf::Texture Laser;
    if (!Laser.loadFromFile("Asset\\Bonus_laser.png")) {
        std::cerr << "Erreur de chargement de la texture de l'ennemi !" << std::endl;
        return -1;
    }








    sf::Sprite playerSprite;
    playerSprite.setOrigin((sf::Vector2f)playerTexture.getSize() / 2.0f);
    playerSprite.setTexture(playerTexture);
    playerSprite.setScale(sf::Vector2f{ 0.1f, 0.1f });
    playerSprite.setPosition(sf::Vector2f{ 500, 500 });



    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    std::vector<StrongEnemy> strongEnemies;
    std::vector<EnemyBullet> enemyBullets;
    std::vector<SniperEnemy> sniperEnemies;
    std::vector<SniperBullet> sniperBullets;
    std::vector<Boss> bosses;
    std::vector<MultiShotBonus> multiShotBonuses;
    std::vector< ShieldBonus>shieldBonuses;
    std::vector<LaserBonus>laserBonuses;
    std::vector<LaserBullet>laserBullets;







    std::srand(static_cast<unsigned int>(std::time(0)));
    float enemySpeed = 50.0f;



    bool isExploded = false;
    bool gameOver = false;





    bool gameOpened = false;
    bool settingOpened = false;
    bool skinOpened = false;
    bool menuActif = false;


    bool bossAppeared = false;
    bool isMultiShotActive = false;
    bool shieldActive = false;
    bool isLaserActive = false;



    float enemySpawnTimer = 0.0f;
    float enemySpawnInterval = 1.0f;

    float shootCooldown = 0.0f;
    float shootCooldownMax = 0.2f;

    float multiShotTimer = 0.0f;
    float multiShotDuration = 5.0f;

    float shieldTimer = 0.0f;
    float shieldDuration = 5.0f;

    float laserShootTimer = 0.0f;
    float laserShootDuration = 5.0f;



    // Seuil de score pour l'apparition du boss
    int bossScoreThreshold = 250;
    // Ajout de la fonction pour le menu de difficulté
    difficulty = showDifficultyMenu(window, settingOpened, skinOpened, menuActif);



    // Ajuster les paramètres de difficulté
    switch (difficulty) {
    case Easy:
        backgroundSpeed = 50.0f;
        enemySpawnInterval = 3;
        break;
    case Medium:
        backgroundSpeed = 50.0f;
        enemySpawnInterval = 2;
        break;
    case Hard:
        backgroundSpeed = 50.0f;
        enemySpawnInterval = 1;
        break;
    }

    // Loop principal du jeu
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();


        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        // Déplacer les fonds verticalement pour créer un effet de parallaxe
        backgroundSprite1.move(0, backgroundSpeed * deltaTime);
        backgroundSprite2.move(0, backgroundSpeed * deltaTime);
        backgroundSprite3.move(0, backgroundSpeed * deltaTime);
        backgroundSprite5.move(0, backgroundSpeed2 * deltaTime);
        backgroundSprite4.move(0, backgroundSpeed2 * deltaTime);


        // Si un fond atteint la fin de l'écran, on le réinitialise en haut
        if (backgroundSprite1.getPosition().y >= window.getSize().y) {
            backgroundSprite1.setPosition(0, -backgroundSprite1.getGlobalBounds().height);  // Réinitialiser en haut
        }
        if (backgroundSprite2.getPosition().y >= window.getSize().y) {
            backgroundSprite2.setPosition(0, -backgroundSprite2.getGlobalBounds().height);  // Réinitialiser en haut
        }
        if (backgroundSprite3.getPosition().y >= window.getSize().y) {
            backgroundSprite3.setPosition(0, -backgroundSprite3.getGlobalBounds().height);  // Réinitialiser en haut
        }

        if (backgroundSprite4.getPosition().y >= window.getSize().y) {
            backgroundSprite4.setPosition(0, -backgroundSprite4.getGlobalBounds().height);  // Réinitialiser en haut
        }
        if (backgroundSprite5.getPosition().y >= window.getSize().y) {
            backgroundSprite5.setPosition(0, -backgroundSprite5.getGlobalBounds().height);  // Réinitialiser en haut
        }





        if (!gameOver) {
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

        // Tir du joueur
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootCooldown >= shootCooldownMax && !gameOver) {
            if (isMultiShotActive) {
                // Tirer plusieurs projectiles (exemple: trois projectiles)
                bullets.push_back(Bullet(playerSprite.getPosition().x - 24, playerSprite.getPosition().y, 300.0f, Bullets));
                bullets.push_back(Bullet(playerSprite.getPosition().x + playerSprite.getScale().x / 2, playerSprite.getPosition().y, 300.0f, Bullets));
                bullets.push_back(Bullet(playerSprite.getPosition().x + playerSprite.getScale().x + 24, playerSprite.getPosition().y, 300.0f, Bullets));
            }
            else {
                if (isLaserActive) {
                    // Tirer un laser 
                    laserBullets.push_back(LaserBullet(playerSprite.getPosition().x + playerSprite.getScale().x / 1 - 30, playerSprite.getPosition().y));
                }
                else {
                    bullets.push_back(Bullet(playerSprite.getPosition().x + playerSprite.getScale().x / 2, playerSprite.getPosition().y, 300.0f, Bullets));
                }
            }
            shootCooldown = 0.0f;
        }

        shootCooldown += deltaTime;


        // Déplacer les projectiles du joueur
        for (size_t i = 0; i < bullets.size(); ++i) {
            bullets[i].move(deltaTime);
        }
        for (size_t i = 0; i < laserBullets.size(); ++i) {
            laserBullets[i].move(deltaTime);
        }
        // Déplacer les ennemis et gérer leurs tirs
        for (size_t i = 0; i < enemies.size(); ++i) {
            enemies[i].move(deltaTime);
            enemies[i].updateShoot(deltaTime, enemyBullets);
            if (enemies[i].sprite.getPosition().y >= window.getSize().y) {
                gameOver = true;
            }
        }

        // Déplacer les ennemis robustes et gérer leurs tirs
        for (size_t i = 0; i < strongEnemies.size(); ++i) {
            strongEnemies[i].move(deltaTime);
            strongEnemies[i].updateShoot(deltaTime, enemyBullets);
            if (strongEnemies[i].sprite.getPosition().y >= window.getSize().y) {
                gameOver = true;
            }
        }
        // Déplacer les ennemis Sniper et gérer leurs tirs
        for (size_t i = 0; i < sniperEnemies.size(); ++i) {
            sniperEnemies[i].move(deltaTime);
            sniperEnemies[i].updateShoot(deltaTime, sniperBullets, playerSprite.getPosition());  // Passer la position du joueur
        }

        // Déplacer et gérer les tirs du boss
        for (size_t i = 0; i < bosses.size(); ++i) {
            bosses[i].move(deltaTime);
            bosses[i].updateShoot(deltaTime, enemyBullets);
        }

        // Déplacer les bonus multi-tir
        for (size_t i = 0; i < multiShotBonuses.size(); ++i) {
            multiShotBonuses[i].move(deltaTime);
        }
        //Déplacer le bonus Bouclier 
        for (size_t i = 0; i < shieldBonuses.size(); ++i) {
            shieldBonuses[i].move(deltaTime);
        }
        //Déplcer le bonuslaser
        for (size_t i = 0; i < laserBonuses.size(); ++i) {
            laserBonuses[i].move(deltaTime);
        }

        // Déplacer les projectiles des ennemis et gérer les collisions avec le joueur
        for (size_t i = 0; i < enemyBullets.size(); ++i) {
            enemyBullets[i].move(deltaTime);

            if (playerSprite.getGlobalBounds().intersects(enemyBullets[i].sprite.getGlobalBounds())) {
                if (!shieldActive) {
                    health--;
                }
                enemyBullets.erase(enemyBullets.begin() + i);
                if (health <= 0) {
                    gameOver = true;
                }
                break;
            }
        }


        // Déplacer les projectiles Sniper et gérer les collisions
        for (size_t i = 0; i < sniperBullets.size(); ++i) {
            sniperBullets[i].move(deltaTime);
            if (playerSprite.getGlobalBounds().intersects(sniperBullets[i].sprite.getGlobalBounds())) {
                if (!shieldActive) {
                    health--;
                }
                sniperBullets.erase(sniperBullets.begin() + i);
                if (health <= 0) {
                    gameOver = true;
                }
                break;
            }
        }




        // Vérifier les collisions entre le joueur et les ennemis
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

        // Vérifier les collisions entre le joueur et les ennemis robustes
        for (size_t i = 0; i < strongEnemies.size(); ++i) {
            if (playerSprite.getGlobalBounds().intersects(strongEnemies[i].sprite.getGlobalBounds())) {
                health--;
                strongEnemies.erase(strongEnemies.begin() + i);
                if (health <= 0) {
                    gameOver = true;
                }
                break;
            }
        }

        // Vérifier les collisions entre le joueur et les ennemis sniper
        for (size_t i = 0; i < sniperEnemies.size(); ++i) {
            if (playerSprite.getGlobalBounds().intersects(sniperEnemies[i].sprite.getGlobalBounds())) {
                health--;
                sniperEnemies.erase(sniperEnemies.begin() + i);
                if (health <= 0) {
                    gameOver = true;
                }
                break;
            }
        }

        // Vérifier les collisions entre les projectiles du joueur et le boss
        for (size_t i = 0; i < bullets.size(); ++i) {
            for (size_t j = 0; j < bosses.size(); ++j) {
                if (bullets[i].sprite.getGlobalBounds().intersects(bosses[j].sprite.getGlobalBounds())) {
                    bullets.erase(bullets.begin() + i);
                    bosses[j].resistance--;
                    if (bosses[j].resistance <= 0) {
                        bosses.erase(bosses.begin() + j);
                        score += 10;
                        gameOver = true;
                    }
                    break;
                }
            }
        }


        // Vérifier les collisions entre les projectiles du joueur et les ennemis
        for (size_t i = 0; i < bullets.size(); ++i) {
            for (size_t j = 0; j < enemies.size(); ++j) {
                if (bullets[i].sprite.getGlobalBounds().intersects(enemies[j].sprite.getGlobalBounds())) {
                    bullets.erase(bullets.begin() + i);
                    enemies.erase(enemies.begin() + j);
                    score += 1;
                    if (rand() % 100 < 25) {
                        multiShotBonuses.push_back(MultiShotBonus(enemies[j].sprite.getPosition().x, enemies[j].sprite.getPosition().y, MultiTirs));
                        std::cout << "Bonus de multitir genere !" << std::endl;
                    }
                    break;
                }
            }
        }


        for (size_t i = 0; i < laserBullets.size(); ++i) {
            for (size_t j = 0; j < enemies.size(); ++j) {
                if (laserBullets[i].shape.getGlobalBounds().intersects(enemies[j].sprite.getGlobalBounds())) {
                    laserBullets.erase(laserBullets.begin() + i);
                    enemies.erase(enemies.begin() + j);
                    score += 1;




                    if (rand() % 100 < 10) {
                        multiShotBonuses.push_back(MultiShotBonus(enemies[j].sprite.getPosition().x, enemies[j].sprite.getPosition().y, MultiTirs));
                        std::cout << "Bonus de multitir genere !" << std::endl;
                    }
                    break;
                }
            }
        }

        // Vérifier les collisions entre les projectiles du joueur et les ennemis robustes
        for (size_t i = 0; i < bullets.size(); ++i) {
            for (size_t j = 0; j < strongEnemies.size(); ++j) {
                if (bullets[i].sprite.getGlobalBounds().intersects(strongEnemies[j].sprite.getGlobalBounds())) {
                    bullets.erase(bullets.begin() + i);
                    strongEnemies[j].resistance--;
                    if (strongEnemies[j].resistance <= 0) {
                        strongEnemies.erase(strongEnemies.begin() + j);
                        score += 2;


                        if (rand() % 100 < 20) {
                            shieldBonuses.push_back(ShieldBonus(strongEnemies[j].sprite.getPosition().x, strongEnemies[j].sprite.getPosition().y, Shiledss));
                            std::cout << "Bonus de bouclier generer !" << std::endl;
                        }
                    }
                    break;
                }
            }
        }

        for (size_t i = 0; i < laserBullets.size(); ++i) {
            for (size_t j = 0; j < strongEnemies.size(); ++j) {
                if (laserBullets[i].shape.getGlobalBounds().intersects(strongEnemies[j].sprite.getGlobalBounds())) {
                    laserBullets.erase(laserBullets.begin() + i);
                    strongEnemies[j].resistance -= laserBullets[i].damage;

                    if (strongEnemies[j].resistance <= 0) {
                        strongEnemies.erase(strongEnemies.begin() + j);
                        score += 2;


                        if (rand() % 100 < 20) {
                            shieldBonuses.push_back(ShieldBonus(strongEnemies[j].sprite.getPosition().x, strongEnemies[j].sprite.getPosition().y, Shiledss));
                            std::cout << "Bonus de bouclier generer !" << std::endl;
                        }
                    }
                    break;
                }
            }
        }

        // Vérifier les collisions entre les projectiles du joueur et les ennemis sniper
        for (size_t i = 0; i < bullets.size(); ++i) {
            for (size_t j = 0; j < sniperEnemies.size(); ++j) {
                if (bullets[i].sprite.getGlobalBounds().intersects(sniperEnemies[j].sprite.getGlobalBounds())) {
                    bullets.erase(bullets.begin() + i);
                    sniperEnemies[j].resistance--;
                    if (sniperEnemies[j].resistance <= 0) {
                        sniperEnemies.erase(sniperEnemies.begin() + j);
                        score += 3;

                        if (rand() % 100 < 10) {
                            laserBonuses.push_back(LaserBonus(sniperEnemies[j].sprite.getPosition().x, sniperEnemies[j].sprite.getPosition().y, Laser));
                            std::cout << "Laser Bonus generer !" << std::endl;
                        }
                    }
                    break;
                }
            }
        }
        for (size_t i = 0; i < laserBullets.size(); ++i) {
            for (size_t j = 0; j < sniperEnemies.size(); ++j) {
                if (laserBullets[i].shape.getGlobalBounds().intersects(sniperEnemies[j].sprite.getGlobalBounds())) {
                    laserBullets.erase(laserBullets.begin() + i);
                    sniperEnemies[j].resistance -= laserBullets[i].damage;  // Applique les dégâts du laser

                    if (sniperEnemies[j].resistance <= 0) {
                        sniperEnemies.erase(sniperEnemies.begin() + j);
                        score += 2;



                        if (rand() % 100) {
                            laserBonuses.push_back(LaserBonus(sniperEnemies[j].sprite.getPosition().x, sniperEnemies[j].sprite.getPosition().y, Laser));
                            std::cout << "Laser Bonus generer !" << std::endl;
                        }
                    }
                    break;
                }
            }
        }





        // Vérifier la collision entre le joueur et les bonus
        for (size_t i = 0; i < multiShotBonuses.size(); ++i) {
            if (playerSprite.getGlobalBounds().intersects(multiShotBonuses[i].sprite.getGlobalBounds())) {
                isMultiShotActive = true;
                multiShotBonuses.erase(multiShotBonuses.begin() + i);
                break;
            }
        }


        if (isMultiShotActive) {
            multiShotTimer += deltaTime;
            if (multiShotTimer >= multiShotDuration) {
                isMultiShotActive = false;
                multiShotTimer = 0.0f;
            }
        }

        // Vérifier la collision entre le joueur et les bonus de bouclier
        for (size_t i = 0; i < shieldBonuses.size(); ++i) {
            if (playerSprite.getGlobalBounds().intersects(shieldBonuses[i].sprite.getGlobalBounds())) {
                shieldActive = true;
                shieldBonuses.erase(shieldBonuses.begin() + i);
                break;
            }
        }

        if (shieldActive) {
            shieldTimer += deltaTime;
            if (shieldTimer >= shieldDuration) {
                shieldActive = false;
                shieldTimer = 0.0f;
            }
        }

        // Vérifier la collision entre le joueur et les bonus de tir laser
        for (size_t i = 0; i < laserBonuses.size(); ++i) {
            if (playerSprite.getGlobalBounds().intersects(laserBonuses[i].sprite.getGlobalBounds())) {
                isLaserActive = true;
                laserBonuses.erase(laserBonuses.begin() + i);
                break;
            }
        }

        if (isLaserActive) {
            laserShootTimer += deltaTime;
            if (laserShootTimer >= laserShootDuration) {
                isLaserActive = false;
                laserShootTimer = 0.0f;

            }
        }










        // Apparition des ennemis avec ajustement basé sur le score
        if (!bossAppeared) {
            enemySpawnTimer += deltaTime;


            if (enemySpawnTimer >= enemySpawnInterval) {
                enemySpawnTimer = 0.0f;

                // Si le score est inférieur à 50, on génère seulement des ennemis de base
                if (score < 50) {
                    int enemyType = rand() % 1;

                    if (enemyType == 0) {
                        enemies.push_back(Enemy(rand() % 700 + 80, 0, enemySpeed, enemyTexture));
                    }

                }
                // Si le score est entre 50 et 150, on commence à ajouter des srongEnnemy
                else if (score >= 50 && score < 150) {
                    int enemyType = rand() % 2;

                    if (enemyType == 0) {
                        enemies.push_back(Enemy(rand() % 700 + 80, 0, enemySpeed, enemyTexture));
                    }
                    else if (enemyType == 1) {
                        strongEnemies.push_back(StrongEnemy(rand() % 700 + 20, 0, enemySpeed, enemyStrongTexture));
                    }

                }
                // Si le score est supèrieur a 150 
                else {
                    int enemyType = rand() % 4;

                    if (enemyType == 0) {
                        enemies.push_back(Enemy(rand() % 700 + 80, 0, enemySpeed, enemyTexture));
                    }
                    else if (enemyType == 1) {
                        strongEnemies.push_back(StrongEnemy(rand() % 700 + 20, 0, enemySpeed, enemyStrongTexture));
                    }
                    else if (enemyType == 2) {
                        sniperEnemies.push_back(SniperEnemy(rand() % 700 + 20, 0, enemySpeed, enemySniperTexture));
                    }
                }
            }
        }

        // Dans la boucle principale du jeu, vérifier si le score atteint le seuil
        if (score >= bossScoreThreshold && bosses.empty()) {
            enemies.clear();
            strongEnemies.clear();
            sniperEnemies.clear();
            bosses.push_back(Boss(350, 0, 30.0f, Bosse));
            bossScoreThreshold = -1;
            bossAppeared = true;
        }







        window.clear();

        window.draw(backgroundSprite3);
        window.draw(backgroundSprite2);
        window.draw(backgroundSprite1);
        window.draw(backgroundSprite5);
        window.draw(backgroundSprite4);






        // Dessiner tous les éléments du jeu (joueur, ennemis, projectiles)
        if (!isExploded && !gameOver) {
            window.draw(playerSprite);
        }

        for (const auto& enemy : enemies) {
            window.draw(enemy.sprite);
        }

        for (const auto& strongEnemy : strongEnemies) {
            window.draw(strongEnemy.sprite);
        }

        for (const auto& sniperEnemy : sniperEnemies) {
            window.draw(sniperEnemy.sprite);
        }

        for (const auto& enemyBullet : enemyBullets) {
            window.draw(enemyBullet.sprite);
        }

        for (const auto& sniperBullet : sniperBullets) {
            window.draw(sniperBullet.sprite);
        }

        for (const auto& bullet : bullets) {
            window.draw(bullet.sprite);
        }
        for (const auto& laserbullet : laserBullets) {
            window.draw(laserbullet.shape);
        }

        for (const auto& boss : bosses) {
            window.draw(boss.sprite);
        }
        for (const auto& multishotbonus : multiShotBonuses) {
            window.draw(multishotbonus.sprite);
        }

        for (const auto& shieldBonus : shieldBonuses) {
            window.draw(shieldBonus.sprite);
        }

        for (const auto& laserBonus : laserBonuses) {
            window.draw(laserBonus.sprite);
        }




        // Afficher le score et la santé
        Updatetext(window);

        window.display();

        if (gameOver) {
            sf::sleep(sf::seconds(2));
            window.close();
        }
    }

    return 0;
}

