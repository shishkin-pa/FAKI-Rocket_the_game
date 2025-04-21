#ifndef PLATFORM_H
#define PLATFORM_H

#include <SFML/Graphics.hpp>

// Класс Platform представляет платформу для посадки бустера
class Platform {
public:
    // Конструктор, принимающий координаты платформы
    Platform(float x, float y);
    
    // Метод для отрисовки платформы в окне
    void draw(sf::RenderWindow& window);
    
    // Возвращает границы платформы (для проверки коллизий)
    sf::FloatRect getBounds() const;
    
    // Устанавливает позицию платформы
    void setPosition(float x, float y);
    
    // Возвращает текущую позицию платформы
    sf::Vector2f getPosition() const;
    
    // Устанавливает текстуру платформы
    void setTexture(const sf::Texture& texture);
    
    // Устанавливает размер платформы
    void setSize(const sf::Vector2f& size);
    
    // Возвращает размер платформы
    sf::Vector2f getSize() const;
    
    // Устанавливает цвет платформы
    void setColor(const sf::Color& color);

private:
    // Прямоугольная форма платформы (основной графический объект)
    sf::RectangleShape shape;
};

#endif