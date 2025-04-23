#ifndef BOOSTER_H
#define BOOSTER_H

#include <SFML/Graphics.hpp>
#include "Engine.h"

class Game; // Предварительное объявление класса Game для избежания циклических зависимостей

class Booster {
public:
    // Конструктор: создает бустер с заданными координатами и ссылкой на игру
    Booster(float x, float y, Game* game);
    ~Booster();

    // Основные методы управления бустером
    void update(float deltaTime); // Обновление состояния бустера
    void applyThrust(float thrust); // Применение тяги двигателей
    void rotateEngines(float deltaAngle); // Поворот двигателей
    void rotateGyroscope(float deltaAngle, float deltaTime); // Коррекция угла гироскопом
    void checkLanding(const sf::FloatRect& platformBounds, const sf::FloatRect& groundBounds); // Проверка посадки
    void explode(); // Взрыв бустера
    void draw(sf::RenderWindow& window); // Отрисовка бустера

    // Геттеры для получения состояния бустера
    bool isLanded() const;
    bool isExploded() const;
    sf::Vector2f getVelocity() const;
    float getAngle() const;
    float getInitialAngle() const;
    sf::Vector2f getWind() const;
    sf::Vector2f getPosition() const;

    // Сеттеры для изменения параметров бустера
    void setWind(const sf::Vector2f& windForce);
    void setPosition(float x, float y);
    void setVelocity(float x, float y);
    void setEngineTiltEnabled(bool enabled);
    void setGravity(float gravity);

private:
    void updateEngines(); // Обновление позиции и угла двигателей

    // Ссылка на основной игровой объект
    Game* game;

    // Физические параметры бустера
    sf::Vector2f position; // Позиция в мире
    sf::Vector2f velocity; // Вектор скорости
    float angle; // Текущий угол наклона
    float initialAngle; // Начальный угол (генерируется случайно)
    float engineAngle; // Угол наклона двигателей
    float angularVelocity; // Угловая скорость
    float thrustForce; // Сила тяги двигателей
    sf::Vector2f windForce; // Влияние ветра
    float gravity; // Гравитация

    // Графические компоненты
    sf::RectangleShape shape; // Основная форма бустера
    Engine leftEngine; // Левый двигатель
    Engine rightEngine; // Правый двигатель

    // Состояния бустера
    bool landed; // Флаг успешной посадки
    bool exploded; // Флаг взрыва
    bool engineTiltEnabled; // Разрешено ли управление наклоном двигателей
    bool landingMessageShown; // Показано ли сообщение о посадке
    bool groundLandingMessageShown; // Показано ли сообщение о посадке на землю

    // Физические константы
    const float maxLandingSpeed = 100.0f; // Максимальная скорость для успешной посадки
    const float maxHorizontalLandingSpeed = 50.0f; // Максимальная горизонтальная скорость посадки
    const float airResistance = 0.2f; // Коэффициент сопротивления воздуха
    const float angularDamping = 0.5f; // Затухание вращения
    const float maxEngineAngle = 30.0f; // Максимальный угол наклона двигателей
    const float gyroscopeTorque = 20.0f; // Момент силы гироскопа
};

#endif