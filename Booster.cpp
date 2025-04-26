#include "Booster.h"
#include "Game.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>

// Функция для нормализации угла в диапазон [-180, 180]
float normalizeAngle(float angle) {
    while (angle > 180.0f) angle -= 360.0f;
    while (angle < -180.0f) angle += 360.0f;
    return angle;
}

// Конструктор бустера
Booster::Booster(float x, float y, Game* game)
    : position(x, y), velocity(0, 0), angle(0), engineAngle(0), landed(false), exploded(false),
      thrustForce(0), angularVelocity(0), windForce(0, 0), engineTiltEnabled(true),
      gravity(0), landingMessageShown(false), groundLandingMessageShown(false), game(game) {
    
    // Настройка графического представления бустера
    shape.setTexture(&game->getBoosterTexture());
    shape.setSize(sf::Vector2f(160, 256)); // Размер бустера
    shape.setOrigin(shape.getSize().x / 2, shape.getSize().y / 2); // Центр вращения
    shape.setPosition(position); // Установка начальной позиции

    // Генерация случайных начальных параметров
    velocity.y = (std::rand() % 80 + 10); // Случайная вертикальная скорость
    velocity.x = 0; // Начальная горизонтальная скорость
    initialAngle = (std::rand() % 120 - 60); // Случайный начальный угол
    angle = initialAngle;
    shape.setRotation(angle); // Установка угла поворота

    // Загрузка текстур пламени для анимации
    std::vector<const sf::Texture*> flameTextures;
    for (int i = 0; i < 6; ++i) {
        flameTextures.push_back(&game->getFlameTexture(i));
    }

    // Настройка двигателей
    leftEngine.setTexture(game->getEngineTexture());
    rightEngine.setTexture(game->getEngineTexture());
    leftEngine.setFlameTextures(flameTextures);
    rightEngine.setFlameTextures(flameTextures);
    
    updateEngines(); // Первоначальное позиционирование двигателей
}

Booster::~Booster() {
    // Деструктор для очистки памяти
}

// Основной метод обновления состояния бустера
void Booster::update(float deltaTime) {
    if (!landed && !exploded) {
        // Применение гравитации
        velocity.y += gravity * deltaTime;

        // Влияние ветра
        velocity += windForce * deltaTime;

        // Сопротивление воздуха (замедление)
        velocity.x -= airResistance * velocity.x * deltaTime;
        velocity.y -= airResistance * velocity.y * deltaTime;

        // Обновление позиции на основе скорости
        position += velocity * deltaTime;

        // Ограничение высоты (чтобы бустер не улетел слишком высоко)
        const float maxHeight = 4000.0f;
        if (position.y < -maxHeight) {
            position.y = -maxHeight;
            if (velocity.y < 0) {
                velocity.y = 0; // Обнуление скорости при достижении верхней границы
            }
        }

        // Ограничение горизонтального перемещения (циклический мир)
        const float maxLength = 3000.0f;
        if (position.x < -maxLength) {
            position.x = maxLength;
        }
        if (position.x > maxLength) {
            position.x = -maxLength;
        }

        shape.setPosition(position); // Обновление позиции графического представления

        // Расчет момента силы от двигателей (вращение бустера)
        float torque = thrustForce * (shape.getSize().x / 2 - 15) * (-std::sin(engineAngle * 3.14159265f / 180.0f));
        angularVelocity += torque * deltaTime / 2; // Обновление угловой скорости
        angularVelocity -= angularDamping * angularVelocity * deltaTime; // Затухание вращения
        angle += angularVelocity * deltaTime; // Обновление угла

        angle = normalizeAngle(angle); // Нормализация угла
        shape.setRotation(angle); // Установка нового угла поворота

        updateEngines(); // Обновление позиции двигателей

        // Обновление анимации двигателей
        leftEngine.update(deltaTime);
        rightEngine.update(deltaTime);
    }
}

// Применение тяги двигателей
void Booster::applyThrust(float thrust) {
    if (!landed && !exploded) {
        thrustForce = thrust;
        
        // Расчет вектора тяги на основе угла бустера и угла двигателей
        float radians = (angle + engineAngle) * 3.14159265f / 180.0f;
        velocity.x += thrust * std::sin(radians); // Горизонтальная компонента
        velocity.y -= thrust * std::cos(radians); // Вертикальная компонента

        // Включение/выключение анимации пламени
        leftEngine.setFlameVisible(thrust > 0);
        rightEngine.setFlameVisible(thrust > 0);
    } else {
        // Выключение пламени при посадке или взрыве
        leftEngine.setFlameVisible(false);
        rightEngine.setFlameVisible(false);
    }
}

// Поворот двигателей
void Booster::rotateEngines(float deltaAngle) {
    if (!landed && !exploded && engineTiltEnabled) {
        engineAngle += deltaAngle;
        // Ограничение угла наклона двигателей
        if (engineAngle > maxEngineAngle) engineAngle = maxEngineAngle;
        if (engineAngle < -maxEngineAngle) engineAngle = -maxEngineAngle;
    }
}

// Коррекция угла гироскопом
void Booster::rotateGyroscope(float deltaAngle, float deltaTime) {
    if (!landed && !exploded) {
        // Медленная коррекция угла с помощью гироскопа
        float torque = engineTiltEnabled ? -gyroscopeTorque : gyroscopeTorque * 10;
        angularVelocity += torque * deltaAngle * deltaTime;
    }
}

// Проверка условий посадки
void Booster::checkLanding(const sf::FloatRect& platformBounds, const sf::FloatRect& groundBounds) {
    if (!exploded && shape.getGlobalBounds().intersects(platformBounds)) {
        // Проверка условий для успешной посадки на платформу:
        // 1. Вертикальная скорость ниже максимальной
        // 2. Горизонтальная скорость ниже максимальной
        // 3. Угол наклона близок к вертикали
        if (std::abs(velocity.y) < maxLandingSpeed && 
            std::abs(velocity.x) < maxHorizontalLandingSpeed && 
            std::abs(angle) < 6) {
            landed = true;
            velocity = sf::Vector2f(0, 0); // Остановка бустера
            leftEngine.setFlameVisible(false);
            rightEngine.setFlameVisible(false);
            if (!landingMessageShown) {
                if (game) game->showExitButton("Congratulations! Successful landing!", sf::Color::Green);
                landingMessageShown = true;
            }
        } else {
            explode(); // Если условия не выполнены - взрыв
        }
    } 
    else if (!exploded && shape.getGlobalBounds().intersects(groundBounds)) {
        // Проверка условий для посадки на землю (менее строгие)
        if (std::abs(velocity.y) < maxLandingSpeed && 
            std::abs(velocity.x) < maxHorizontalLandingSpeed && 
            std::abs(angle) < 6) {
            landed = true;
            velocity = sf::Vector2f(0, 0);
            leftEngine.setFlameVisible(false);
            rightEngine.setFlameVisible(false);
            if (!groundLandingMessageShown) {
                if (game) game->showExitButton("You landed on ground! Not perfect but safe.", sf::Color::Yellow);
                groundLandingMessageShown = true;
            }
        } else {
            explode();
        }
    }
}

// Взрыв бустера
void Booster::explode() {
    if (!exploded) {
        exploded = true;
        shape.setFillColor(sf::Color::Transparent); // Скрытие бустера
        std::cout << "BOOM! The booster has exploded!" << std::endl;
        if (game) game->showExitButton("BOOM! The rocket has exploded!", sf::Color::Red);
    }
}

// Отрисовка бустера
void Booster::draw(sf::RenderWindow& window) {
    if (!exploded) {
        window.draw(shape); // Отрисовка только если бустер не взорвался
    }

    // Отрисовка двигателей с учетом состояния взрыва
    leftEngine.draw(window, exploded);
    rightEngine.draw(window, exploded);
}

// Геттеры и сеттеры
bool Booster::isLanded() const { return landed; }
bool Booster::isExploded() const { return exploded; }
sf::Vector2f Booster::getVelocity() const { return velocity; }
void Booster::setVelocity(float x, float y) {
    velocity.x = x;
    velocity.y = y;
}
float Booster::getAngle() const { return angle; }
float Booster::getInitialAngle() const { return initialAngle; }
void Booster::setWind(const sf::Vector2f& windForce) { this->windForce = windForce; }
sf::Vector2f Booster::getWind() const { return windForce; }
sf::Vector2f Booster::getPosition() const { return position; }
void Booster::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
    shape.setPosition(position);
    updateEngines();
}
void Booster::setEngineTiltEnabled(bool enabled) {
    engineTiltEnabled = enabled;
    if (!enabled) {
        engineAngle = 0; // Сброс угла двигателей при отключении управления
    }
}
void Booster::setGravity(float gravity) { this->gravity = gravity; }

// Обновление позиции и угла двигателей
void Booster::updateEngines() {
    // Преобразование угла в радианы для тригонометрических функций
    float radians = angle * 3.14159265f / 180.0f;

    // Расчет позиции левого двигателя
    float leftX = position.x - (shape.getSize().x / 2 - 15) * std::cos(radians) - (shape.getSize().y / 2) * std::sin(radians);
    float leftY = position.y - (shape.getSize().x / 2 - 15) * std::sin(radians) + (shape.getSize().y / 2) * std::cos(radians);
    leftEngine.setPosition(sf::Vector2f(leftX, leftY));
    leftEngine.setRotation(angle + engineAngle);

    // Расчет позиции правого двигателя
    float rightX = position.x + (shape.getSize().x / 2 - 15) * std::cos(radians) - (shape.getSize().y / 2) * std::sin(radians);
    float rightY = position.y + (shape.getSize().x / 2 - 15) * std::sin(radians) + (shape.getSize().y / 2) * std::cos(radians);
    rightEngine.setPosition(sf::Vector2f(rightX, rightY));
    rightEngine.setRotation(angle + engineAngle);
}