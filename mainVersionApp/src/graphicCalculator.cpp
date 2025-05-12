#include "graphicCalculator.hpp"
#include "components/coordianteAxesOxOy/index.cpp"
#include "components/coordianteGrid/index.cpp"
#include "components/generationTriangles/index.cpp"
#include "components/calculateGraphPoints/index.cpp"
#include "components/drawGraphic/index.cpp"

#include "lib/exprtk.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
namespace {
const int lengthWindow = 1400;
const int widthWindow = 800;

const int wholeLengthWindow = 6000;
const int wholeWidthWindow = 5500;

// Определение масштабов по осям X и Y
const double scaleX = 50; // Масштаб по оси X
const double scaleY = 50; // Масштаб по оси Y


} // namespace

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
    if (!parser.compile(expression, expr)) {
        // Вывод ошибки компиляции и закрытие окна в случае неудачи
        std::cerr << "Ошибка компиляции выражения: " << parser.error() << std::endl;
        window.close();
        return; // Выход из функции
    }

    // Вектор для хранения исходных точек графика (в координатах SFML)
    std::vector<sf::Vector2f> graphPoints;
    CalculateGraphPoints(graphPoints, x_var, scaleX, scaleY, expr, wholeLengthWindow);

    // Создание sf::VertexArray для построения толстой линии графика из треугольников
    sf::VertexArray graphVertexArray(sf::Triangles);
    CraeteThicknessGraphics(graphVertexArray, graphPoints);

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

    DrawApp(window, scaleX, scaleY, gorizontalLines, vertLines, OsX, OsY, graphVertexArray);

    std::cout << "Окно закрыто." << std::endl;
}

} // namespace RealizationGraphicCalculator
