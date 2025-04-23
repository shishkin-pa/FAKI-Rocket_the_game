#include "Engine.h"
#include <cmath>

// Конструктор двигателя
Engine::Engine() : flameVisible(false), currentFlameFrame(0), flameAnimationTime(0), 
                  flameFrameDuration(0.05f), isStarting(true) {
    // Настройка размеров и центра вращения двигателя и пламени
    shape.setSize(sf::Vector2f(30, 64));
    shape.setOrigin(shape.getSize().x / 2, shape.getSize().y); // Центр внизу
    flame.setSize(sf::Vector2f(30, 45));
    flame.setOrigin(flame.getSize().x / 2, flame.getSize().y - 15); // Центр чуть выше основания
}

// Установка текстуры двигателя
void Engine::setTexture(const sf::Texture& texture) {
    shape.setTexture(&texture);
    shape.setFillColor(sf::Color::White);
}

// Установка текстур для анимации пламени
void Engine::setFlameTextures(const std::vector<const sf::Texture*>& textures) {
    flameTextures = textures;
    if (!flameTextures.empty()) {
        flame.setTexture(flameTextures[0]); // Установка первой текстуры по умолчанию
    }
}

// Установка позиции двигателя с учетом пламени
void Engine::setPosition(const sf::Vector2f& position) {
    shape.setPosition(position);
    
    // Расчет позиции пламени с учетом угла поворота двигателя
    float radians = shape.getRotation() * 3.14159265f / 180.0f;
    float offsetX = 0;
    float offsetY = shape.getSize().y / 2;
    float flameX = position.x + offsetX * cos(radians) - offsetY * sin(radians);
    float flameY = position.y + offsetX * sin(radians) + offsetY * cos(radians);
    flame.setPosition(flameX, flameY);
}

// Установка угла поворота двигателя и пламени
void Engine::setRotation(float angle) {
    shape.setRotation(angle);
    flame.setRotation(angle);
    setPosition(shape.getPosition()); // Обновление позиции с новым углом
}

// Обновление анимации пламени
void Engine::update(float deltaTime) {
    if (!flameVisible || flameTextures.empty()) return;

    flameAnimationTime += deltaTime;
    
    // Проверка необходимости перехода к следующему кадру
    if (flameAnimationTime >= flameFrameDuration) {
        flameAnimationTime = 0;
        
        // Логика анимации: первые 3 кадра для плавного появления
        if (isStarting) {
            currentFlameFrame++;
            if (currentFlameFrame >= 3) {
                isStarting = false;
                currentFlameFrame = 3;
            }
        } else {
            // Циклическое переключение между кадрами 3-5
            currentFlameFrame = 3 + (currentFlameFrame + 1 - 3) % 3;
        }
        
        // Установка текущей текстуры пламени
        if (currentFlameFrame < flameTextures.size()) {
            flame.setTexture(flameTextures[currentFlameFrame], true);
        }
    }
}

// Отрисовка двигателя
void Engine::draw(sf::RenderWindow& window, bool isExploded) {
    if (!isExploded) {
        window.draw(shape); // Отрисовка двигателя если нет взрыва
        if (flameVisible && !flameTextures.empty()) {
            window.draw(flame); // Отрисовка пламени если оно видимо
        }
    }
}

// Управление видимостью пламени
void Engine::setFlameVisible(bool visible) {
    if (flameVisible != visible) {
        flameVisible = visible;
        if (visible) {
            // Сброс анимации при включении пламени
            isStarting = true;
            currentFlameFrame = 0;
            flameAnimationTime = 0;
            if (!flameTextures.empty()) {
                flame.setTexture(flameTextures[0]);
            }
        }
    }
}