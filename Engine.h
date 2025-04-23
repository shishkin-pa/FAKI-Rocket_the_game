#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>
#include <vector>

class Engine {
public:
    Engine(); // Конструктор
    
    // Методы управления позицией и вращением
    void setPosition(const sf::Vector2f& position);
    void setRotation(float angle);
    
    // Методы отрисовки и управления видимостью пламени
    void draw(sf::RenderWindow& window, bool isExploded);
    void setFlameVisible(bool visible);
    
    // Методы для установки текстур
    void setTexture(const sf::Texture& texture);
    void setFlameTextures(const std::vector<const sf::Texture*>& textures);
    
    // Обновление состояния (анимация пламени)
    void update(float deltaTime);

private:
    sf::RectangleShape shape; // Графическое представление двигателя
    sf::RectangleShape flame; // Графическое представление пламени
    bool flameVisible; // Видимость пламени
    std::vector<const sf::Texture*> flameTextures; // Текстуры для анимации пламени
    int currentFlameFrame; // Текущий кадр анимации
    float flameAnimationTime; // Время текущего кадра
    float flameFrameDuration; // Длительность одного кадра
    bool isStarting; // Флаг начала анимации (для плавного старта)
};

#endif