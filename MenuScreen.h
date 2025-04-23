#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include <SFML/Graphics.hpp>
#include <vector>

// Класс для отображения и управления экраном меню
class MenuScreen {
public:
    // Конструктор, принимающий окно и шрифт
    MenuScreen(sf::RenderWindow& window, sf::Font& font);
    
    // Основные методы
    void draw(); // Отрисовка меню
    void handleEvent(sf::Event& event); // Обработка событий
    bool isMenuActive() const; // Проверка активности меню
    void setMenuActive(bool active); // Установка активности меню
    void resetMenu(); // Сброс состояния меню

    // Методы получения настроек
    bool isEngineTiltEnabled() const; // Включен ли наклон двигателей
    bool isWindEnabled() const; // Включен ли ветер
    sf::Vector2f getWindForce() const; // Получение силы ветра
    float getGravity() const; // Получение значения гравитации

private:
    // Вспомогательные методы
    void createMenu(); // Создание элементов меню
    void updateButtonHover(); // Обновление состояния наведения на кнопки
    void updateSliderColor(); // Обновление цвета слайдера гравитации

    // Ссылки на внешние объекты
    sf::RenderWindow& window; // Окно для отрисовки
    sf::Font& font; // Шрифт для текста

    // Состояние меню
    bool menuActive; // Активно ли меню
    int menuStep; // Текущий шаг меню (0-2)

    // Текстуры кнопок
    sf::Texture tiltYesTexture; // Текстура кнопки "Да" для наклона
    sf::Texture tiltNoTexture; // Текстура кнопки "Нет" для наклона
    sf::Texture windNoneTexture; // Текстура кнопки "Нет ветра"
    sf::Texture windWeakTexture; // Текстура кнопки "Слабый ветер"
    sf::Texture windModerateTexture; // Текстура кнопки "Умеренный ветер"
    sf::Texture windStrongTexture; // Текстура кнопки "Сильный ветер"
    sf::Texture startGameTexture; // Текстура кнопки "Начать игру"
    sf::Texture gravityMinTexture; // Текстура кнопки минимальной гравитации

    // Графические элементы меню
    sf::RectangleShape tiltYesButton; // Кнопка "Да" для наклона
    sf::RectangleShape tiltNoButton; // Кнопка "Нет" для наклона
    std::vector<sf::RectangleShape> windButtons; // Кнопки выбора ветра
    sf::RectangleShape startButton; // Кнопка старта игры
    sf::RectangleShape gravityMinButton; // Кнопка минимальной гравитации

    // Элементы слайдера гравитации
    sf::RectangleShape gravitySlider; // Заполненная часть слайдера
    sf::RectangleShape gravitySliderTrack; // Фон слайдера
    sf::RectangleShape gravitySliderHandle; // Ползунок слайдера

    // Настройки игры
    bool engineTiltEnabled; // Флаг наклона двигателей
    bool windEnabled; // Флаг наличия ветра
    sf::Vector2f windForce; // Вектор силы ветра
    float gravity; // Значение гравитации
    bool isDraggingSlider; // Флаг перетаскивания ползунка

    // Элементы заставки
    sf::Texture splashTexture; // Текстура заставки
    sf::RectangleShape splashScreen; // Спрайт заставки
    bool showSplash; // Флаг показа заставки
};

#endif