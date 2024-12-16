#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <SFML/Audio.hpp>

sf::Texture EnnemyBullets;
sf::Texture SniperBullets;

// Classe représentant un projectile du joueur
class Bullet {
public:
    sf::Sprite sprite;
    float speed;

    Bullet(float x, float y, float speed, const sf :: Texture & texture ) {
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

    EnemyBullet(float x, float y, float speed, const sf:: Texture& texture) {
        sprite.setTexture(texture);
        sprite.setScale(sf::Vector2f(0.01, 0.01));
        sprite.setPosition(x, y);
        this->speed = speed;
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

    SniperBullet(float x, float y, sf::Vector2f playerPosition, float speed, const sf :: Texture& texture) {
        sprite.setTexture(texture);
        sprite.setScale(sf::Vector2f(0.01, 0.01));
        sprite.setPosition(x, y);
        this->speed = speed * 2;

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
        this->speed = speed;
        shootTimer = 0.0f;
        shootInterval = 2.0f;
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

    StrongEnemy(float x, float y, float speed,const sf:: Texture & texture) {
        sprite.setTexture(texture);
        sprite.setScale(0.1f, 0.1f);
        sprite.setPosition({ x,y });
        this->speed = speed * 0.5;
        this->resistance = 3;
        shootTimer = 0.0f;
        shootInterval = 5.0f;
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
        this->speed = speed;
        this->resistance = 2;
        shootTimer = 0.0f;
        shootInterval = 4.0f;
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

    Boss(float x, float y, float speed,const sf :: Texture & texture) {
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

    MultiShotBonus(float x, float y,const sf :: Texture & texture ) {
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

    ShieldBonus(float x, float y,const sf :: Texture & texture) {
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
    

    LaserBonus(float x, float y,const sf :: Texture & texture) {
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

// Fonction pour afficher le menu principal et attendre que le joueur commence
void mainMenu(sf::RenderWindow& window, bool& gameStarted1, bool& shopOpened, bool& settingOpened, bool& settingSonOpened, bool& difficulty, bool& gameOpened,
    bool& difficultyNormalOpened, bool& difficultyHardOpened, bool& gameStarted2, bool& gameStarted3) {
    sf::Font font;
    if (!font.loadFromFile("Asset\\Starjhol.ttf")) {
        std::cerr << "Erreur de chargement de la police !" << std::endl;
        return;
    }

    // Titre du jeu
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setCharacterSize(80);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setString("@");
    titleText.setPosition(window.getSize().x / 1.96 - titleText.getLocalBounds().width / 3, window.getSize().y / 5);

    // Texte pour commencer le jeu
    sf::Text startText;
    startText.setFont(font);
    startText.setCharacterSize(30);
    startText.setFillColor(sf::Color::White);
    startText.setString("Play");
    startText.setPosition(window.getSize().x / 1.96 - startText.getLocalBounds().width / 2, window.getSize().y / 2.5);

    // Texte pour afficher la boutique des skins
    sf::Text shopText;
    shopText.setFont(font);
    shopText.setCharacterSize(30);
    shopText.setFillColor(sf::Color::White);
    shopText.setString("Skin");
    shopText.setPosition(window.getSize().x / 1.96 - shopText.getLocalBounds().width / 2, window.getSize().y / 2);

    // Texte pour afficher les options
    sf::Text settingText;
    settingText.setFont(font);
    settingText.setCharacterSize(30);
    settingText.setFillColor(sf::Color::White);
    settingText.setString("Setting");
    settingText.setPosition(window.getSize().x / 1.95
   - settingText.getLocalBounds().width / 2, window.getSize().y / 1.70);

    // Texte pour afficher l'option Son
    sf::Text settingOptionText;  // Déclaration ici
    settingOptionText.setFont(font);
    settingOptionText.setCharacterSize(40);
    settingOptionText.setFillColor(sf::Color::White);
    settingOptionText.setString("Sound");
    settingOptionText.setPosition(window.getSize().x / 4 - settingOptionText.getLocalBounds().width / 2, window.getSize().y / 4);

    sf::Text difficultyNormalText;
    difficultyNormalText.setFont(font);
    difficultyNormalText.setCharacterSize(30);
    difficultyNormalText.setFillColor(sf::Color::White);
    difficultyNormalText.setString("Normal");
    difficultyNormalText.setPosition(window.getSize().x / 2.75 - difficultyNormalText.getLocalBounds().width / 2, window.getSize().y / 3);

    sf::Text difficultyHardText;
    difficultyHardText.setFont(font);
    difficultyHardText.setCharacterSize(30);
    difficultyHardText.setFillColor(sf::Color::White);
    difficultyHardText.setString("Hard");
    difficultyHardText.setPosition(window.getSize().x / 1.60 - difficultyHardText.getLocalBounds().width / 2, window.getSize().y / 3);

    sf::Text editeurText;
    editeurText.setFont(font);
    editeurText.setCharacterSize(30);
    editeurText.setFillColor(sf::Color::White);
    editeurText.setString("Editeur level");
    editeurText.setPosition(window.getSize().x / 2.05 - editeurText.getLocalBounds().width / 2, window.getSize().y / 2.15);


  

    // Gestion des événements
    sf::Event event;

    // Redessiner la fenêtre selon l'état actuel
    if (gameOpened) {
        sf::Text difficultyTitleText;
        difficultyTitleText.setFont(font);
        difficultyTitleText.setCharacterSize(50);
        difficultyTitleText.setFillColor(sf::Color::Yellow);
        difficultyTitleText.setString("Difficulty");
        difficultyTitleText.setPosition(window.getSize().x / 2 - difficultyTitleText.getLocalBounds().width / 2, window.getSize().y / 7);

        sf::Text difficultyText;
        difficultyText.setFont(font);
        difficultyText.setCharacterSize(30);
        difficultyText.setFillColor(sf::Color::Green);
        difficultyText.setString("Choose your difficulty...");
        difficultyText.setPosition(window.getSize().x / 2 - difficultyText.getLocalBounds().width / 2, window.getSize().y / 1.50);

        window.clear();  // Vider la fenêtre avant de dessiner
        window.draw(difficultyTitleText);
        window.draw(difficultyNormalText);
        window.draw(difficultyHardText);
        window.draw(editeurText);
        window.draw(difficultyText);
        window.display();  // Afficher le contenu de la boutique
    }

    else if (shopOpened) {
        // Si la boutique est ouverte, redessiner la fenêtre avec la boutique
        sf::Text shopTitleText;
        shopTitleText.setFont(font);
        shopTitleText.setCharacterSize(50);
        shopTitleText.setFillColor(sf::Color::Yellow);
        shopTitleText.setString("Skin");
        shopTitleText.setPosition(window.getSize().x / 2 - shopTitleText.getLocalBounds().width / 2, window.getSize().y / 8);

        sf::Text skinText;
        skinText.setFont(font);
        skinText.setCharacterSize(30);
        skinText.setFillColor(sf::Color::Green);
        skinText.setString("Choose your skin...");
        skinText.setPosition(window.getSize().x / 2 - skinText.getLocalBounds().width / 2, window.getSize().y / 1.5);

        // Vider la fenêtre et afficher la boutique
        window.clear();  // Vider la fenêtre avant de dessiner
        window.draw(shopTitleText);
        window.draw(skinText);
        window.display();  // Afficher le contenu de la boutique
    }
    else if (settingOpened) {
        // Si les options sont ouvertes, redessiner la fenêtre avec les paramètres
        sf::Text settingTitleText;
        settingTitleText.setFont(font);
        settingTitleText.setCharacterSize(50);
        settingTitleText.setFillColor(sf::Color::Yellow);
        settingTitleText.setString("Setting");
        settingTitleText.setPosition(window.getSize().x / 2 - settingTitleText.getLocalBounds().width / 2, window.getSize().y / 8);

        // Vider la fenêtre et afficher les options
        window.clear();  // Vider la fenêtre avant de dessiner
        window.draw(settingTitleText);
        window.draw(settingOptionText);  // Dessiner le texte "Son"
        window.display();  // Afficher les options

        if (settingSonOpened) {
            sf::Text upSon;
            upSon.setFont(font);
            upSon.setCharacterSize(30);
            upSon.setFillColor(sf::Color::White);
            upSon.setString("more");
            upSon.setPosition(window.getSize().x / 1.75 - upSon.getLocalBounds().width / 2, window.getSize().y / 3.75);

            sf::Text downSon;
            downSon.setFont(font);
            downSon.setCharacterSize(30);
            downSon.setFillColor(sf::Color::White);
            downSon.setString("less");
            downSon.setPosition(window.getSize().x / 2.25 - downSon.getLocalBounds().width / 2, window.getSize().y / 3.75);

            // Vider la fenêtre et afficher les options
            window.clear();  // Vider la fenêtre avant de dessiner
            window.draw(settingTitleText);
            window.draw(settingOptionText);
            window.draw(upSon);
            window.draw(downSon);
            window.display();  // Afficher les options
        }
    }
    else {
        // Si la boutique et les options ne sont pas ouvertes, afficher le menu principal
        window.clear();  // Vider la fenêtre avant de dessiner
        window.draw(titleText);
        window.draw(shopText);
        window.draw(startText);
        window.draw(settingText);
        window.display();  // Afficher le menu principal
    }

    // Boucle d'événements
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Gestion des clics sur les boutons "Play", "Skin", "Option"
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            if (startText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                gameOpened = true;
            }
            // Vérifier si l'utilisateur clique sur "Normal"
            if (difficultyNormalText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                gameStarted1 = true;
            }

            // Vérifier si l'utilisateur clique sur "Hard"
            if (difficultyHardText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                gameStarted2 = true;
            }

            // Vérifier si l'utiliseur clique sur "Editeur level"
            if (editeurText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                gameStarted3 = true;
            }

            // Vérifier si l'utilisateur clique sur "Skin"
            if (shopText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                shopOpened = true;
            }

            // Vérifier si l'utilisateur clique sur "Option"
            if (settingText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                settingOpened = true;
            }

            // Vérifier si l'utilisateur clique sur "Son"
            if (settingOptionText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                settingSonOpened = true;
            }
        }

        // Gestion des touches "Esc" pour fermer les menus
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            shopOpened = false;
            settingOpened = false;
            gameOpened = false;
        }
    }
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
    backgroundTexture3.loadFromFile("Asset\\fond_du_jeu_etoile_2.png");
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
    bool gameStarted1 = false;
    bool shopOpened = false;
    bool settingOpened = false;
    bool settingSonOpened = false;
    bool difficulty = false;
    bool gameOpened = false;
    bool difficultyNormalOpened = false;
    bool difficultyHardOpened = false;
    bool gameStarted2 = false;
    bool gameStarted3 = false;
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

    while (window.isOpen() && !gameStarted1) {
        mainMenu(window, gameStarted1, shopOpened, settingOpened, settingSonOpened, difficulty, gameOpened, difficultyNormalOpened, difficultyHardOpened, gameStarted2, gameStarted3);
    }

































































    // Loop principal du jeu
    while (window.isOpen() && gameStarted1) {
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
                    if (rand() % 100 < 10) {
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

                        if (rand() % 100) {
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
  




