#include "graphicCalculator.hpp"
#include "components/controlMovementLimit/index.cpp"
#include "components/coordianteAxesOxOy/index.cpp"
#include "components/coordianteGrid/index.cpp"
#include "lib/exprtk.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

namespace {
const int lengthWindow = 1400;
const int widthWindow = 800;
}  // namespace

namespace RealizationGraphicCalculator {
double getFunc() {
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

void StartApp() {
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
    sf::VertexArray gorizontalLines(sf::Lines, (5600 / scaleY) * 2);
    CreateCoordinateGrid(gorizontalLines, sf::Color{55, 55, 55, 100}, scaleX, 5600, 6800, 'y');

    // Создаём вертикальные линии координатной сетки
    sf::VertexArray vertLines(sf::Lines, (6800 / scaleX) * 2);
    CreateCoordinateGrid(vertLines, sf::Color{55, 55, 55, 100}, scaleY, 6800, 5600, 'x');

    // Создаём ось Ox с определёнными праметрами
    std::vector<sf::RectangleShape> OsX;
    CreateCoordiantesAxes(OsX, 800, scaleX, sf::Color::Black, 2.0f, 'y');

    // Создаём ось Oy с определёнными праметрами
    std::vector<sf::RectangleShape> OsY;
    CreateCoordiantesAxes(OsY, 1400, scaleY, sf::Color::Black, 2.0f, 'x');

    for (double x = -5000; x < 5000; x += 0.1) {
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
            return;
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

    sf::Event event;

    sf::View view = window.getDefaultView();

    float zoomLevel = 1.0f;           // Начальный уровень зума (1.0 - без зума)
    const float zoomFactor = 0.5f;    // Минимальный уровень зума
    const float maxZoomLevel = 5.0f;  // Максимальный уровень зума

    bool isMouseDragging = false;  // Флаг для отслеживания перетаскивания
    sf::Vector2i initialMousePos;  // Начальная позиция мыши для перетаскивания

    sf::Vector2f movementLimit(1000, 1000);             // Максимальное перемещение
    sf::Vector2f initialViewCenter = view.getCenter();  // Начальный центр вида

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            // Обработка закрытия окна
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Обработка зума колесиком мыши
            if (event.type == sf::Event::MouseWheelScrolled) {
                const float zoomCoefficient = 1.1f;  // Коэффициент изменения зума
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));  // Мировые координаты курсора

                if (event.mouseWheelScroll.delta > 0) {  // Колесико вверх (увеличение зума)
                    zoomLevel *= zoomCoefficient;
                    // Ограничение максимального уровня зума
                    if (zoomLevel > maxZoomLevel) {
                        zoomLevel = maxZoomLevel;
                    }
                } else if (event.mouseWheelScroll.delta < 0) {  // Колесико вниз (уменьшение зума)
                    zoomLevel /= zoomCoefficient;
                    // Ограничение минимального уровня зума
                    if (zoomLevel < zoomFactor) {
                        zoomLevel = zoomFactor;
                    }
                }

                // Сохранения старого центра вида
                sf::Vector2f oldCenter = view.getCenter();
                // Сохранения старого размера вида
                sf::Vector2f oldSize = view.getSize();
                // Устанавливает размер вида относительно зума
                view.setSize(window.getDefaultView().getSize() / zoomLevel);

                // Корректировка центра вида относительно курсора мыши
                sf::Vector2f newCenter = oldCenter + (mousePos - oldCenter) * (1.0f - oldSize.x / view.getSize().x);

                // Изменение положения центра
                sf::Vector2f delta = newCenter - initialViewCenter;

                ControlMovementLimit(delta, movementLimit, newCenter, initialViewCenter);
                // Если происходит выход за пределы границы, то происходит смещения центра в пределы границы по оси OX
                if (std::abs(delta.x) > movementLimit.x) {
                    newCenter.x = initialViewCenter.x + (delta.x > 0 ? movementLimit.x : -movementLimit.x);
                }

                // Если происходит выход за пределы границы, то происходит смещения центра в пределы границы по оси OY
                if (std::abs(delta.y) > movementLimit.y) {
                    newCenter.y = initialViewCenter.y + (delta.y > 0 ? movementLimit.y : -movementLimit.y);
                }

                // устанавливаем новый центр вида
                view.setCenter(newCenter);
                // установаливаем новый вид в окне
                window.setView(view);
            }

            // Обработка перетаскивания мышью
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                isMouseDragging = true;
                initialMousePos = sf::Mouse::getPosition(window);
            }

            // Обработка отпускания левой кнопки мыши
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                isMouseDragging = false;
            }

            if (isMouseDragging && event.type == sf::Event::MouseMoved) {
                // нынешнее позиция мышки
                sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
                sf::Vector2f delta = window.mapPixelToCoords(initialMousePos) - window.mapPixelToCoords(currentMousePos);

                // Устаннавливаем новый центр после перещения на некий ветор delta
                sf::Vector2f newCenter = view.getCenter() + delta;

                // Изменение положения центра относительно исходного
                sf::Vector2f totalDelta = newCenter - initialViewCenter;
                ControlMovementLimit(totalDelta, movementLimit, newCenter, initialViewCenter);

                if (std::abs(totalDelta.x) > movementLimit.x) {
                    newCenter.x = initialViewCenter.x + (totalDelta.x > 0 ? movementLimit.x : -movementLimit.x);
                }
                if (std::abs(totalDelta.y) > movementLimit.y) {
                    newCenter.y = initialViewCenter.y + (totalDelta.y > 0 ? movementLimit.y : -movementLimit.y);
                }

                view.setCenter(newCenter);
                window.setView(view);

                initialMousePos = currentMousePos;
            }
        }

        window.clear(sf::Color::White);

        // рисуем сам график
        for (const auto& point : graph) {
            window.draw(point);
        }

        // рисуем ось Ox
        for (const auto& point : OsX) {
            window.draw(point);
        }

        // рисуем ось Oy
        for (const auto& point : OsY) {
            window.draw(point);
        }

        // рисуем координатгую сетку
        window.draw(vertLines);
        window.draw(gorizontalLines);

        // Здесь можно добавить отрисовку объектов (например, спрайтов или фигур)
        window.display();
    }

    std::cout << "Введите математическое выражение (например, sin(x^2) + cos(x)): " << std::endl;
    std::cout << "Или введите no, если хотите выйти из программы" << std::endl;
    std::getline(std::cin, expression);
}
}  // namespace RealizationGraphicCalculator
