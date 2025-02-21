#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "exprtk.hpp"
auto getFunc() -> double {

    // Объявление переменной для хранения математического выражения
    std::string expression;

    // Запрос ввода математического выражения у пользователя
    std::cout << "Введите математическое выражение (например, sin(x^2) + cos(x)): ";
    std::getline(std::cin, expression);

    // Определение переменной для x
    double x;
    //std::cout << "Введите значение x: ";
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
    return expr.value();
    std::cout << "Результат: y = " << result << std::endl;
}

void CreateComponentsGraphics(sf::VertexArray& nameComponent, sf::Color color, int x1, int y1, int x2, int y2) {
    nameComponent[0].position = sf::Vector2f(x1, y1);
    nameComponent[1].position = sf::Vector2f(x2, y2);
    nameComponent[0].color = color;
    nameComponent[1].color = color;
}

int main() {

    std::string expression;

    std::cout << "Введите математическое выражение (например, sin(x^2) + cos(x)): ";
    std::getline(std::cin, expression);

    sf::RenderWindow window(sf::VideoMode(800, 600), "График функции");

    const double scaleX = 100; // Масштаб по оси X
    const double scaleY = 100; // Масштаб по оси Y

    sf::VertexArray OsX(sf::Lines, 2);
    sf::VertexArray OsY(sf::Lines, 2);

    CreateComponentsGraphics(OsX, sf::Color::Black, 0, 300, 800, 300);
    CreateComponentsGraphics(OsY, sf::Color::Black, 400, 0, 400, 600);

    sf::VertexArray leftPartPointerOsY(sf::Lines, 2);
    sf::VertexArray rightPartPointerOsY(sf::Lines, 2);

    CreateComponentsGraphics(leftPartPointerOsY, sf::Color::Black, 400, 0, 390, 20);
    CreateComponentsGraphics(rightPartPointerOsY, sf::Color::Black, 400, 0, 410, 20);

    sf::VertexArray bottomPartPointerOsX(sf::Lines, 2);
    sf::VertexArray topPartPointerOsX(sf::Lines, 2);

    CreateComponentsGraphics(bottomPartPointerOsX, sf::Color::Black, 800, 300, 780, 290);
    CreateComponentsGraphics(topPartPointerOsX, sf::Color::Black, 800, 300, 780, 310);

    sf::VertexArray graph(sf::LineStrip, 800);
    sf::VertexArray gorizontalLines(sf::Lines,( 600 / scaleY) * 2);
    sf::VertexArray vertLines(sf::Lines,(800 / scaleX) * 2);

    for (int i = 0, yPixel = 0; i < (600 / scaleY) * 2; i += 2, yPixel += scaleY){
        gorizontalLines[i].position = sf::Vector2f(0, yPixel);
        gorizontalLines[i].color =  sf::Color{55, 55, 55, 100};
        gorizontalLines[i + 1].position = sf::Vector2f(800, yPixel);
        gorizontalLines[i + 1].color =  sf::Color{55, 55, 55, 100};
    }

    for (int i = 0, xPixel = 0; i < (800 / scaleX) * 2; i += 2, xPixel += scaleX){
        vertLines[i].position = sf::Vector2f(xPixel, 0);
        vertLines[i].color =  sf::Color{55, 55, 55, 100};
        vertLines[i + 1].position = sf::Vector2f(xPixel, 600);
        vertLines[i + 1].color =  sf::Color{55, 55, 55, 100};
    }

    for (int x = 0; x < 800; ++x) {
        double realX = (x - 400) / scaleX;
        exprtk::symbol_table<double> symbol_table;
        symbol_table.add_variable("x", realX);

        exprtk::expression<double> expr;
        expr.register_symbol_table(symbol_table);

        exprtk::parser<double> parser;
        if (!parser.compile(expression, expr)) {
            std::cerr << "Ошибка компиляции: " << parser.error() << std::endl;
            return 1.;
        }
        float realY = expr.value();
        graph[x].position = sf::Vector2f(x, 300 - realY * scaleY);
        graph[x].color = sf::Color::Red;
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        window.draw(graph);

        window.draw(vertLines);
        window.draw(gorizontalLines);

        window.draw(OsX);
        window.draw(OsY);

        window.draw(leftPartPointerOsY);
        window.draw(rightPartPointerOsY);

        window.draw(bottomPartPointerOsX);
        window.draw(topPartPointerOsX);

        window.display();
    }

    return 0;
}
