#ifndef PLATFORM_H
#define PLATFORM_H

#include <SFML/Graphics.hpp>

// Класс для представления платформы/земли в игре
class Platform {
public:
    // Конструктор с позицией
    Platform(float x, float y);
    
    // Основные методы
    void draw(sf::RenderWindow& window); // Отрисовка платформы
    sf::FloatRect getBounds() const; // Получение границ платформы
    
    // Методы управления позицией
    void setPosition(float x, float y); // Установка позиции
    sf::Vector2f getPosition() const; // Получение позиции
    
    // Методы управления текстурой и внешним видом
    void setTexture(const sf::Texture& texture); // Установка текстуры
    void setSize(const sf::Vector2f& size); // Установка размера
    sf::Vector2f getSize() const; // Получение размера
    void setColor(const sf::Color& color); // Установка цвета

private:
    sf::RectangleShape shape; // Графическое представление платформы
};

#endif