#include "graphicCalculator.hpp"
#include "components/coordianteAxesOxOy/index.cpp"
#include "components/coordianteGrid/index.cpp"

#include "components/drawGraphic/index.cpp"

#include "lib/exprtk.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>


namespace RealizationGraphicCalculator {

// Основная функция запуска приложения графического калькулятора
void StartApp() {
    // Объявление переменной для хранения математического выражения
    std::string expression;

    // Запрос ввода математического выражения у пользователя
    std::cout << "Введите математическое выражение (например, sin(x^2) + cos(x)): ";
    std::getline(std::cin, expression);

    // Создание окна SFML
    sf::RenderWindow window(sf::VideoMode(lengthWindow, widthWindow), "График функции");
    // Установка ограничения частоты кадров
    window.setFramerateLimit(120);

    // Создание символической таблицы для exprtk
    exprtk::symbol_table<double> symbol_table;
    // Объявление переменной, которая будет связана с символом 'x' в exprtk
    double x_var;
    symbol_table.add_variable("x", x_var); // Добавление переменной 'x'

    // Создание объекта выражения для графика
    exprtk::expression<double> expr;
    expr.register_symbol_table(symbol_table); // Регистрация символической таблицы

    // Создание парсера и компиляция выражения для графика
   exprtk::parser<double> parser;
    if (!expression.empty()) {
        if (!parser.compile(expression, expr)) {
            std::cerr << "Ошибка компиляции начального выражения: " << parser.error() << std::endl;
            // Можно закрыть окно или продолжить с "пустым" графиком
        }
    } else {
        // Если выражение пустое, компилируем "0" для избежания ошибок
        if (!parser.compile("0", expr)) {
             std::cerr << "Ошибка компиляции выражения по умолчанию '0': " << parser.error() << std::endl;
        }
    }

    // Вектор для хранения исходных точек графика (в координатах SFML)
    sf::View view = window.getDefaultView();
    std::vector<sf::Vector2f> graphPoints;
    CalculateGraphPoints(graphPoints, x_var, scaleX, scaleY, expr, view);

    // Создание sf::VertexArray для построения толстой линии графика из треугольников
    sf::VertexArray graphVertexArray(sf::Triangles);
    CreateThicknessGraphics(graphVertexArray, graphPoints);

    // Создаём горизонтальные линии координатной сетки
    size_t horizontaLinesCount = 0;
    if (scaleY != 0) horizontaLinesCount = (static_cast<size_t>(wholeWidthWindow / std::abs(scaleY))) * 2;
    sf::VertexArray gorizontalLines(sf::Lines, horizontaLinesCount);
    if (scaleY != 0) {
       CreateCoordinateGrid(gorizontalLines, sf::Color{55, 55, 55, 100}, scaleX, wholeWidthWindow, wholeLengthWindow, 'y');
    }

    // Создаём вертикальные линии координатной сетки
    size_t verticalLinesCount = 0;
    if (scaleX != 0) verticalLinesCount = (static_cast<size_t>(wholeLengthWindow / std::abs(scaleX))) * 2;
    sf::VertexArray vertLines(sf::Lines, verticalLinesCount);
    if (scaleX != 0) {
        CreateCoordinateGrid(vertLines, sf::Color{55, 55, 55, 100}, scaleY, wholeLengthWindow, wholeWidthWindow, 'x');
    }

    // Создаём ось Ox с определёнными параметрами
    std::vector<sf::RectangleShape> OsX;
    CreateCoordiantesAxes(OsX, 800.0, scaleX, wholeLengthWindow, sf::Color::Black, 2.0f, 'y');

    // Создаём ось Oy с определёнными параметрами
    std::vector<sf::RectangleShape> OsY;
    CreateCoordiantesAxes(OsY, 1400.0, scaleY, wholeLengthWindow, sf::Color::Black, 2.0f, 'x');

    // Определяем шрифт строк, вводимой в окно
    sf::Font font;
    if (!font.loadFromFile("lib/fonts/ARIAL.TTF")) { // ВАЖНО: Убедитесь, что файл arial.ttf доступен
        std::cerr << "Критическая ошибка: Не удалось загрузить шрифт arial.ttf!" << std::endl;
        window.close();
        return;
    }


    // Создаём окно дополнительное для ввода функции
    sf::RectangleShape inputPanelBackground;
    float panelHeight = 40.0f;
    float panelWidth = static_cast<float>(lengthWindow) * 0.4f; // 40% ширины окна
    float panelPadding = 10.0f;

    //  Устанавливает размер прямоугольника 'inputPanelBackground'.
    inputPanelBackground.setSize(sf::Vector2f(panelWidth, panelHeight));
    inputPanelBackground.setFillColor(sf::Color(220, 220, 220, 200)); // Светло-серый, полупрозрачный
    inputPanelBackground.setOutlineColor(sf::Color::Black); // / Устанавливаем цвет контура (обводки)
    inputPanelBackground.setOutlineThickness(1.f); // толщина котура
    inputPanelBackground.setPosition(panelPadding, static_cast<float>(widthWindow) - panelHeight - panelPadding);

    sf::Text inputTextDisplay;
    inputTextDisplay.setFont(font);
    inputTextDisplay.setString(expression);
    inputTextDisplay.setCharacterSize(20); // Размер символов
    inputTextDisplay.setFillColor(sf::Color::Black);
    // Позиционирование текста внутри панели с небольшим отступом
    inputTextDisplay.setPosition(
        inputPanelBackground.getPosition().x + 5.f,
        inputPanelBackground.getPosition().y + (panelHeight - inputTextDisplay.getCharacterSize()) / 2.f - 2.f // Небольшая корректировка для вертикали
    );



    DrawApp(window, gorizontalLines, vertLines, OsX, OsY, graphVertexArray,
            expr, symbol_table, parser, x_var, expression, graphPoints,
            inputPanelBackground, inputTextDisplay, font,
            scaleX, scaleY, lengthWindow, widthWindow);

    std::cout << "Окно закрыто." << std::endl;
}

} // namespace RealizationGraphicCalculator
