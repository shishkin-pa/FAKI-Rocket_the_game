#ifndef MARKER_H
#define MARKER_H

#include <SFML/Graphics.hpp>
#include "Booster.h"
#include "Platform.h"

// Класс для отображения маркера платформы, когда она вне экрана
class Marker {
public:
    Marker(); // Конструктор
    
    // Обновление состояния маркера
    void update(const Booster& booster, const Platform& platform, const sf::RenderWindow& window);
    
    // Отрисовка маркера
    void draw(sf::RenderWindow& window) const;

private:
    sf::ConvexShape markerShape; // Графическое представление маркера (треугольник)
    bool isVisible; // Видима ли платформа на экране
    float distanceToPlatform; // Расстояние до платформы
    sf::Vector2f platformPosition; // Позиция платформы
    sf::Vector2f lastValidPosition; // Последняя корректная позиция маркера
};

#endif