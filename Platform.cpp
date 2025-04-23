#include "Platform.h"

// Конструктор платформы
Platform::Platform(float x, float y) {
    // Настройка размера и позиции
    shape.setSize(sf::Vector2f(600, 40));
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::White); // Белый цвет по умолчанию
}

// Отрисовка платформы
void Platform::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

// Установка цвета платформы
void Platform::setColor(const sf::Color& color) {
    shape.setFillColor(color);
}

// Получение границ платформы
sf::FloatRect Platform::getBounds() const {
    return shape.getGlobalBounds();
}

// Установка позиции платформы
void Platform::setPosition(float x, float y) {
    shape.setPosition(x, y);
}

// Получение позиции платформы
sf::Vector2f Platform::getPosition() const {
    return shape.getPosition();
}

// Установка текстуры платформы
void Platform::setTexture(const sf::Texture& texture) {
    shape.setTexture(&texture);
}

// Установка размера платформы
void Platform::setSize(const sf::Vector2f& size) {
    shape.setSize(size);
}

// Получение размера платформы
sf::Vector2f Platform::getSize() const {
    return shape.getSize();
}