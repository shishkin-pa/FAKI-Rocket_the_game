#include "MenuScreen.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

// Конструктор меню
MenuScreen::MenuScreen(sf::RenderWindow& window, sf::Font& font)
    : window(window), font(font), menuActive(true), menuStep(0),
      engineTiltEnabled(true), windEnabled(false), windForce(0.f, 0.f),
      gravity(200.0f), isDraggingSlider(false), showSplash(true) {
    
    // Загрузка текстуры заставки
    if (!splashTexture.loadFromFile("textures/splash.png")) {
        std::cerr << "Failed to load splash.png" << std::endl;
        // Создание черного экрана при ошибке загрузки
        sf::Image img;
        img.create(1, 1, sf::Color::Black);
        splashTexture.loadFromImage(img);
    }
    
    // Настройка спрайта заставки
    splashScreen.setSize(sf::Vector2f(window.getSize()));
    splashScreen.setTexture(&splashTexture);
    
    // Загрузка текстур кнопок
    if (!tiltYesTexture.loadFromFile("textures/tilt_yes.png")) {
        std::cerr << "Failed to load tilt_yes.png" << std::endl;
    }
    if (!tiltNoTexture.loadFromFile("textures/tilt_no.png")) {
        std::cerr << "Failed to load tilt_no.png" << std::endl;
    }
    if (!windNoneTexture.loadFromFile("textures/wind_none.png")) {
        std::cerr << "Failed to load wind_none.png" << std::endl;
    }
    if (!windWeakTexture.loadFromFile("textures/wind_weak.png")) {
        std::cerr << "Failed to load wind_weak.png" << std::endl;
    }
    if (!windModerateTexture.loadFromFile("textures/wind_moderate.png")) {
        std::cerr << "Failed to load wind_moderate.png" << std::endl;
    }
    if (!windStrongTexture.loadFromFile("textures/wind_strong.png")) {
        std::cerr << "Failed to load wind_strong.png" << std::endl;
    }
    if (!startGameTexture.loadFromFile("textures/start_game.png")) {
        std::cerr << "Failed to load start_game.png" << std::endl;
    }
    if (!gravityMinTexture.loadFromFile("textures/gravity_min.png")) {
        std::cerr << "Failed to load gravity_min.png" << std::endl;
    }

    // Создание элементов меню
    createMenu();
    updateSliderColor();
}

// Создание элементов меню
void MenuScreen::createMenu() {
    // Получение размеров окна
    const sf::Vector2u windowSize = window.getSize();
    const float centerX = windowSize.x / 2.0f;
    const float centerY = windowSize.y / 2.0f;

    // Настройка кнопок наклона двигателя
    tiltYesButton.setSize(sf::Vector2f(312, 312));
    tiltYesButton.setPosition(centerX - 350, centerY - 156);
    tiltYesButton.setTexture(&tiltYesTexture);

    tiltNoButton.setSize(sf::Vector2f(312, 312));
    tiltNoButton.setPosition(centerX + 38, centerY - 156);
    tiltNoButton.setTexture(&tiltNoTexture);

    // Настройка кнопок ветра
    windButtons.clear();
    const std::vector<sf::Texture*> windTextures = {
        &windNoneTexture, &windWeakTexture, &windModerateTexture, &windStrongTexture
    };
    
    // Расчет позиций кнопок ветра
    const float totalWidth = 4 * 312 + 3 * 40;
    const float startX = centerX - totalWidth / 2;
    
    for (int i = 0; i < 4; ++i) {
        sf::RectangleShape button(sf::Vector2f(312, 312));
        button.setPosition(startX + i * (312 + 40), centerY - 156);
        button.setTexture(windTextures[i]);
        windButtons.push_back(button);
    }

    // Настройка кнопки минимальной гравитации
    gravityMinButton.setSize(sf::Vector2f(312, 312));
    gravityMinButton.setPosition(centerX - 156, centerY - 364);
    gravityMinButton.setTexture(&gravityMinTexture);

    // Настройка трека слайдера гравитации
    gravitySliderTrack.setSize(sf::Vector2f(624, 20));
    gravitySliderTrack.setPosition(centerX - 312, centerY);
    gravitySliderTrack.setFillColor(sf::Color(100, 100, 100));
    gravitySliderTrack.setOutlineThickness(5);
    gravitySliderTrack.setOutlineColor(sf::Color::Black);

    // Настройка заполненной части слайдера
    gravitySlider.setSize(sf::Vector2f(624, 20));
    gravitySlider.setPosition(centerX - 312, centerY);
    gravitySlider.setFillColor(sf::Color::White);
    gravitySlider.setOutlineThickness(5);
    gravitySlider.setOutlineColor(sf::Color::Black);

    // Настройка ползунка слайдера
    gravitySliderHandle.setSize(sf::Vector2f(20, 40));
    gravitySliderHandle.setPosition(centerX - 312 + (gravity / 400.0f) * gravitySlider.getSize().x, centerY - 10);
    gravitySliderHandle.setFillColor(sf::Color::Green);
    gravitySliderHandle.setOutlineThickness(3);
    gravitySliderHandle.setOutlineColor(sf::Color::Black);

    // Настройка кнопки старта
    startButton.setSize(sf::Vector2f(312, 312));
    startButton.setPosition(centerX - 156, centerY + 64);
    startButton.setTexture(&startGameTexture);

    // Обновление состояния кнопок
    updateButtonHover();
}

// Отрисовка меню
void MenuScreen::draw() {
    if (!menuActive) return;

    // Очистка экрана
    window.clear(sf::Color(50, 50, 50));

    // Отрисовка заставки или меню
    if (showSplash) {
        window.draw(splashScreen);
    }
    else {
        // Отрисовка текущего шага меню
        switch (menuStep) {
            case 0: // Шаг 1 - выбор наклона двигателей
                window.draw(tiltYesButton);
                window.draw(tiltNoButton);
                break;
            case 1: // Шаг 2 - выбор ветра
                for (auto& button : windButtons) {
                    window.draw(button);
                }
                break;
            case 2: // Шаг 3 - настройка гравитации и старт
                window.draw(gravityMinButton);
                window.draw(gravitySliderTrack);
                window.draw(gravitySlider);
                window.draw(gravitySliderHandle);
                window.draw(startButton);
                break;
        }
    }

    // Отображение на экране
    window.display();
}

// Обработка событий меню
void MenuScreen::handleEvent(sf::Event& event) {
    if (!menuActive) return;

    // Пропуск заставки по любому нажатию
    if (showSplash) {
        if (event.type == sf::Event::KeyPressed || 
            event.type == sf::Event::MouseButtonPressed) {
            showSplash = false;
        }
        return;
    }

    // Обновление состояния наведения
    updateButtonHover();

    // Обработка нажатий мыши
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // Обработка в зависимости от текущего шага меню
        switch (menuStep) {
            case 0: // Выбор наклона двигателей
                if (tiltYesButton.getGlobalBounds().contains(mousePos)) {
                    engineTiltEnabled = true;
                    menuStep = 1;
                }
                else if (tiltNoButton.getGlobalBounds().contains(mousePos)) {
                    engineTiltEnabled = false;
                    menuStep = 1;
                }
                break;
            case 1: // Выбор ветра
                for (size_t i = 0; i < windButtons.size(); ++i) {
                    if (windButtons[i].getGlobalBounds().contains(mousePos)) {
                        switch (i) {
                            case 0: // Нет ветра
                                windEnabled = false; 
                                windForce = sf::Vector2f(0.f, 0.f); 
                                break;
                            case 1: // Слабый ветер
                                windEnabled = true; 
                                windForce = sf::Vector2f(
                                    static_cast<float>((std::rand()%10+1)*2), 
                                    static_cast<float>(std::rand()%10+1)
                                ); 
                                break;
                            case 2: // Умеренный ветер
                                windEnabled = true; 
                                windForce = sf::Vector2f(
                                    static_cast<float>((std::rand()%15+5)*2), 
                                    static_cast<float>(std::rand()%15+5)
                                ); 
                                break;
                            case 3: // Сильный ветер
                                windEnabled = true; 
                                windForce = sf::Vector2f(
                                    static_cast<float>((std::rand()%20+10)*2), 
                                    static_cast<float>(std::rand()%20+10)
                                ); 
                                break;
                        }
                        menuStep = 2;
                        break;
                    }
                }
                break;
            case 2: // Настройка гравитации и старт
                if (gravitySliderHandle.getGlobalBounds().contains(mousePos)) {
                    isDraggingSlider = true;
                }
                else if (gravitySliderTrack.getGlobalBounds().contains(mousePos)) {
                    // Клик по треку - мгновенное перемещение ползунка
                    float relativeX = mousePos.x - gravitySliderTrack.getPosition().x;
                    float percent = std::max(0.f, std::min(1.f, relativeX / gravitySliderTrack.getSize().x));
                    
                    gravity = percent * 400.0f;
                    gravitySliderHandle.setPosition(
                        gravitySliderTrack.getPosition().x + percent * gravitySliderTrack.getSize().x - 10,
                        gravitySliderHandle.getPosition().y
                    );
                    
                    updateSliderColor();
                    isDraggingSlider = true;
                }
                else if (startButton.getGlobalBounds().contains(mousePos)) {
                    menuActive = false; // Запуск игры
                }
                break;
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased) {
        isDraggingSlider = false; // Окончание перетаскивания
    }
    else if (event.type == sf::Event::MouseMoved && isDraggingSlider) {
        // Перетаскивание ползунка
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        float newX = std::max(gravitySliderTrack.getPosition().x,
                            std::min(mousePos.x, 
                                    gravitySliderTrack.getPosition().x + gravitySliderTrack.getSize().x));
        gravitySliderHandle.setPosition(newX - 10, gravitySliderHandle.getPosition().y);
        gravity = (newX - gravitySliderTrack.getPosition().x) / gravitySliderTrack.getSize().x * 400.0f;
        
        updateSliderColor();
    }
}

// Обновление состояния наведения на кнопки
void MenuScreen::updateButtonHover() {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    const sf::Color hoverColor(50, 100, 150); // Цвет контура при наведении

    // Обновление состояния кнопок в зависимости от текущего шага
    switch (menuStep) {
        case 0: // Шаг 1 - наклон двигателей
            tiltYesButton.setOutlineThickness(tiltYesButton.getGlobalBounds().contains(mousePos) ? 5 : 0);
            tiltYesButton.setOutlineColor(hoverColor);
            tiltNoButton.setOutlineThickness(tiltNoButton.getGlobalBounds().contains(mousePos) ? 5 : 0);
            tiltNoButton.setOutlineColor(hoverColor);
            break;
        case 1: // Шаг 2 - ветер
            for (auto& button : windButtons) {
                button.setOutlineThickness(button.getGlobalBounds().contains(mousePos) ? 5 : 0);
                button.setOutlineColor(hoverColor);
            }
            break;
        case 2: // Шаг 3 - гравитация и старт
            startButton.setOutlineThickness(startButton.getGlobalBounds().contains(mousePos) ? 5 : 0);
            startButton.setOutlineColor(hoverColor);
            gravityMinButton.setOutlineThickness(gravityMinButton.getGlobalBounds().contains(mousePos) ? 5 : 0);
            gravityMinButton.setOutlineColor(hoverColor);
            break;
    }
}

// Проверка активности меню
bool MenuScreen::isMenuActive() const {
    return menuActive;
}

// Установка активности меню
void MenuScreen::setMenuActive(bool active) {
    menuActive = active;
    if (active) {
        menuStep = 0; // Сброс к первому шагу
    }
}

// Проверка включения наклона двигателей
bool MenuScreen::isEngineTiltEnabled() const {
    return engineTiltEnabled;
}

// Проверка включения ветра
bool MenuScreen::isWindEnabled() const {
    return windEnabled;
}

// Получение силы ветра
sf::Vector2f MenuScreen::getWindForce() const {
    return windForce;
}

// Получение значения гравитации
float MenuScreen::getGravity() const {
    return gravity;
}

// Сброс меню
void MenuScreen::resetMenu() {
    // Сброс состояния
    menuActive = true;
    menuStep = 0;
    showSplash = true;
    isDraggingSlider = false;
    
    // Пересоздание элементов
    createMenu();
    
    // Сброс настроек (кроме гравитации)
    float currentGravity = gravity;
    engineTiltEnabled = true;
    windEnabled = false;
    windForce = sf::Vector2f(0.f, 0.f);
    gravity = currentGravity;
    
    updateSliderColor();
}

// Обновление цвета слайдера гравитации
void MenuScreen::updateSliderColor() {
    gravitySliderHandle.setFillColor(
        (gravity >= 190 && gravity <= 210) ? sf::Color::Green : sf::Color::Red
    );
}