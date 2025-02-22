#include "components/coordianteAxesOxOy/index.cpp"
#include "components/coordianteGrid/index.cpp"
#include "exprtk.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

namespace {
const int lengthWindow = 1400;
const int widthWindow = 800;
}  // namespace

auto getFunc() -> double {
    // Объявление переменной для хранения математического выражения
    std::string expression;

    // Запрос ввода математического выражения у пользователя
    std::cout << "Введите математическое выражение (например, sin(x^2) + cos(x)): ";
    std::getline(std::cin, expression);

    // Определение переменной для x
    double x;
    // std::cout << "Введите значение x: ";
    std::cin >> x;

    // Создание символической таблицы
    exprtk::symbol_table<double> symbol_table;
    symbol_table.add_variable("x", x);

    // Создание выражения
    exprtk::expression<double> expr;
    expr.register_symbol_table(symbol_table);

    // Компиляция выражения
    exprtk::parser<double> parser;
    if (!parser.compile(expression, expr)) {
        std::cerr << "Ошибка компиляции: " << parser.error() << std::endl;
        return 1.;
    }

    // Вычисление результата
    double result = expr.value();
    std::cout << "Результат: y = " << result << std::endl;
    return expr.value();
}

int main() {
    // Объявление переменной для хранения математического выражения
    std::string expression;

    // Запрос ввода математического выражения у пользователя
    std::cout << "Введите математическое выражение (например, sin(x^2) + cos(x)): ";
    std::getline(std::cin, expression);

    sf::RenderWindow window(sf::VideoMode(lengthWindow, widthWindow), "График функции");

    const double scaleX = 50;  // Масштаб по оси X
    const double scaleY = 50;  // Масштаб по оси Y

    std::vector<sf::RectangleShape> graph;

    // Создаём горизонтальные линии координатной сетки
    sf::VertexArray gorizontalLines(sf::Lines, (widthWindow / scaleY) * 2);
    CreateCoordinateGrid(gorizontalLines, sf::Color{55, 55, 55, 100}, scaleX, widthWindow, lengthWindow, 'y');

    // Создаём вертикальные линии координатной сетки
    sf::VertexArray vertLines(sf::Lines, (lengthWindow / scaleX) * 2);
    CreateCoordinateGrid(vertLines, sf::Color{55, 55, 55, 100}, scaleY, lengthWindow, widthWindow, 'x');

    // Создаём ось Ox с определёнными праметрами
    std::vector<sf::RectangleShape> OsX;
    CreateCoordiantesAxes(OsX, widthWindow, scaleX, sf::Color::Black, 2.0f, 'y');

    // Создаём ось Oy с определёнными праметрами
    std::vector<sf::RectangleShape> OsY;
    CreateCoordiantesAxes(OsY, lengthWindow, scaleY, sf::Color::Black, 2.0f, 'x');

    for (double x = 0; x < lengthWindow; x += 0.1) {
        double realX = (x - 700) / scaleX;

        // Создание символической таблицы
        exprtk::symbol_table<double> symbol_table;
        symbol_table.add_variable("x", realX);

        // Создание выражения
        exprtk::expression<double> expr;
        expr.register_symbol_table(symbol_table);

        // Компиляция выражения
        exprtk::parser<double> parser;
        if (!parser.compile(expression, expr)) {
            std::cerr << "Ошибка компиляции: " << parser.error() << std::endl;
            return 1;
        }

        // создание точки
        sf::RectangleShape point(sf::Vector2f(3.0f, 3.0f));

        // поиск знчения y
        float realY = expr.value();

        // определяем позицию точки
        point.setPosition(x - 1.5, 400 - realY * scaleY - 1.5);
        point.setFillColor(sf::Color::Green);

        // добавляем точку в вектор
        graph.push_back(point);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        // рисуем сам график
        for (const auto& point : graph) {
            window.draw(point);
        }

        // рисуем сам ось Ox
        for (const auto& point : OsX) {
            window.draw(point);
        }

        // рисуем сам ось Oy
        for (const auto& point : OsY) {
            window.draw(point);
        }

        // рисуем сам координатгую сетку
        window.draw(vertLines);
        window.draw(gorizontalLines);

        window.display();
    }

    return 0;
}
