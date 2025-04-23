#include "Marker.h"
#include <cmath>

// Конструктор маркера
Marker::Marker() : isVisible(false), distanceToPlatform(0) {
    // Настройка формы маркера (треугольник)
    markerShape.setPointCount(3);
    markerShape.setPoint(0, sf::Vector2f(0, -15));  // Вершина
    markerShape.setPoint(1, sf::Vector2f(-10, 10)); // Левый нижний угол
    markerShape.setPoint(2, sf::Vector2f(10, 10));  // Правый нижний угол
    
    // Настройка внешнего вида
    markerShape.setFillColor(sf::Color(200, 100, 255)); // Фиолетовый цвет
    markerShape.setOutlineThickness(2); // Толщина контура
    markerShape.setOutlineColor(sf::Color(150, 50, 200)); // Темно-фиолетовый контур
    markerShape.setOrigin(0, 0); // Центр в вершине треугольника
}

// Обновление состояния маркера
void Marker::update(const Booster& booster, const Platform& platform, const sf::RenderWindow& window) {
    // Получение границ платформы и области видимости
    sf::FloatRect platformBounds = platform.getBounds();
    sf::View view = window.getView();
    sf::FloatRect viewArea(
        view.getCenter() - view.getSize()/2.f, 
        view.getSize()
    );

    // Проверка видимости платформы
    isVisible = viewArea.intersects(platformBounds);
    if (isVisible) return; // Если платформа видна, маркер не нужен

    // Настройка отступов маркера от краев экрана
    const float sideMargin = 50.f;   // Отступ от боковых краев
    const float bottomMargin = 40.f; // Отступ от нижнего края

    // Расчет допустимой области для маркера
    float markerMinX = viewArea.left + sideMargin;
    float markerMaxX = viewArea.left + viewArea.width - sideMargin;
    float markerY = viewArea.top + viewArea.height - bottomMargin;

    // Позиция центра платформы по X
    float platformCenterX = platformBounds.left + platformBounds.width/2;

    // Расчет позиции маркера с ограничениями
    float markerX;
    if (platformCenterX < markerMinX) {
        markerX = markerMinX; // Прижимаем к левому краю
    } 
    else if (platformCenterX > markerMaxX) {
        markerX = markerMaxX; // Прижимаем к правому краю
    } 
    else {
        markerX = platformCenterX; // Позиционируем по центру платформы
    }

    // Установка позиции маркера
    sf::Vector2f markerPos(markerX, markerY);

    // Расчет направления к платформе
    sf::Vector2f platformCenter(platformCenterX, platformBounds.top + platformBounds.height/2);
    sf::Vector2f direction = platformCenter - markerPos;
    float angle = atan2(direction.y, direction.x) * 180.f / 3.14159265f;
    angle += 90.f; // Коррекция угла для треугольника

    // Масштабирование в зависимости от расстояния
    distanceToPlatform = std::abs(booster.getPosition().y - platformBounds.top);
    float scale = std::min(1.5f, 1000.f/distanceToPlatform);

    // Установка параметров маркера
    markerShape.setPosition(markerPos);
    markerShape.setRotation(angle);
    markerShape.setScale(scale, scale);
}

// Отрисовка маркера
void Marker::draw(sf::RenderWindow& window) const {
    if (!isVisible) { // Отрисовываем только если платформа не видна
        window.draw(markerShape);
    }
}