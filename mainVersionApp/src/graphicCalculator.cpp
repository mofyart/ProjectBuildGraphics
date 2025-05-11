#include "graphicCalculator.hpp"
#include "components/controlMovementLimit/index.cpp"
#include "components/coordianteAxesOxOy/index.cpp"
#include "components/coordianteGrid/index.cpp"
#include "lib/exprtk.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <limits>

namespace {
const int lengthWindow = 1400;
const int widthWindow = 800;

const int wholeLengthWindow = 6000;
const int wholeWidthWindow = 5500;
} // namespace

namespace RealizationGraphicCalculator {

// Функция для вычисления значения функции в заданной точке x
double getFunc() {
    // Объявление переменной для хранения математического выражения
    std::string expression;
    // Объявление переменной для значения x
    double x;

    // Запрос ввода математического выражения у пользователя
    std::cout << "Введите математическое выражение (например, sin(x^2) + cos(x)): ";
    std::getline(std::cin, expression);

    // Запрос ввода значения x у пользователя
    std::cout << "Введите значение x: ";
    std::cin >> x;
    // Очистка буфера ввода после чтения числа
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Создание символической таблицы для exprtk
    exprtk::symbol_table<double> symbol_table;
    // Добавление переменной 'x' в символическую таблицу
    symbol_table.add_variable("x", x);

    // Создание объекта выражения
    exprtk::expression<double> expr;
    // Регистрация символической таблицы в выражении
    expr.register_symbol_table(symbol_table);

    // Создание парсера для компиляции выражения
    exprtk::parser<double> parser;
    // Компиляция математического выражения
    if (!parser.compile(expression, expr)) {
        // Вывод ошибки компиляции, если она произошла
        std::cerr << "Ошибка компиляции: " << parser.error() << std::endl;
        // Возврат значения по умолчанию или индикатора ошибки
        return 1.; // Возвращаем 1. как индикатор ошибки компиляции
    }

    // Вычисление значения выражения при заданном x
    double result = expr.value();
    // Вывод результата
    std::cout << "Результат: y = " << result << std::endl;

    // Возвращаем вычисленное значение
    return result;
}

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

    // Определение масштабов по осям X и Y
    const double scaleX = 50; // Масштаб по оси X
    const double scaleY = 50; // Масштаб по оси Y
    // Желаемая толщина линии графика в пикселях
    const float desiredGraphThickness = 2.0f;

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
    std::vector<sf::Vector2f> graph_points;
    // Определение диапазона x-координат в мировом пространстве (для всей "холста")
    const double sfml_x_start = 700.0 - wholeLengthWindow / 2.0;
    const double sfml_x_end = 700.0 + wholeLengthWindow / 2.0;
    // Шаг по x для вычисления точек графика
    const double sfml_x_step = 0.25;

    // Вычисление предполагаемого количества точек
    size_t num_points = 0;
    if (sfml_x_step > 0.00001) { // Проверка шага, чтобы избежать деления на ноль
        num_points = static_cast<size_t>((sfml_x_end - sfml_x_start) / sfml_x_step) + 1;
    }

    // Вычисление точек графика
    if (num_points > 0) {
        graph_points.reserve(num_points); // Резервирование памяти
        for (size_t i = 0; i < num_points; ++i) {
            // Текущая x-координата в мировом пространстве SFML
            double current_sfml_x = sfml_x_start + i * sfml_x_step;

            // Преобразование SFML x-координаты в математическую x
            // Центр системы координат (0,0 в мат. пр-ве) находится в (700, 400) в SFML
            if (scaleX == 0) {
                 continue; // Пропускаем, если масштаб по X нулевой
            }
            x_var = (current_sfml_x - 700.0) / scaleX;

            // Вычисление математической y-координаты
            double math_y = expr.value();

            // Преобразование математической y-координаты в SFML y-координату
             if (scaleY == 0) {
                 continue; // Пропускаем, если масштаб по Y нулевой
            }
            double sfml_y = 400.0 - math_y * scaleY;

            // Добавление точки в вектор
            graph_points.push_back(sf::Vector2f(static_cast<float>(current_sfml_x), static_cast<float>(sfml_y)));
        }
    }

    // Создание sf::VertexArray для построения толстой линии графика из треугольников
    sf::VertexArray graph_vertex_array(sf::Triangles);

    // Генерация треугольников для толстой линии графика
    if (graph_points.size() >= 2) {
        // Для N точек будет N-1 отрезков. Каждый отрезок - это прямоугольник (2 треугольника = 6 вершин).
        graph_vertex_array.resize((graph_points.size() - 1) * 6);
        size_t current_vertex = 0;

        // Проходим по всем парам соседних точек
        for (size_t i = 0; i < graph_points.size() - 1; ++i) {
            sf::Vector2f p1 = graph_points[i];
            sf::Vector2f p2 = graph_points[i + 1];

            // Вектор направления отрезка
            sf::Vector2f direction = p2 - p1;
            // Длина отрезка
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (length == 0) {
                continue; // Пропускаем отрезки нулевой длины
            }

            // Единичный вектор направления
            sf::Vector2f unit_direction = direction / length;
            // Нормаль (перпендикуляр к направлению)
            sf::Vector2f normal(unit_direction.y, -unit_direction.x);

            // Смещение от центральной линии для создания толщины
            sf::Vector2f offset = normal * (desiredGraphThickness / 2.0f);

            // Вершины прямоугольника для текущего отрезка (p1, p2)
            sf::Vector2f v1 = p1 + offset; // Первая вершина первого треугольника
            sf::Vector2f v2 = p1 - offset; // Вторая вершина первого треугольника
            sf::Vector2f v3 = p2 + offset; // Третья вершина первого треугольника (и первая вершина второго)
            sf::Vector2f v4 = p2 - offset; // Третья вершина второго треугольника

            // Добавляем вершины первого треугольника (v1, v2, v3)
            graph_vertex_array[current_vertex++] = sf::Vertex(v1, sf::Color::Red); // Цвет графика - зеленый, как в вашем коде
            graph_vertex_array[current_vertex++] = sf::Vertex(v2, sf::Color::Red);
            graph_vertex_array[current_vertex++] = sf::Vertex(v3, sf::Color::Red);

            // Добавляем вершины второго треугольника (v3, v2, v4) - для создания прямоугольника
            graph_vertex_array[current_vertex++] = sf::Vertex(v3, sf::Color::Red);
            graph_vertex_array[current_vertex++] = sf::Vertex(v2, sf::Color::Red);
            graph_vertex_array[current_vertex++] = sf::Vertex(v4, sf::Color::Red);
        }
        // Обрезаем VertexArray, если были пропущенные сегменты
        graph_vertex_array.resize(current_vertex);
    }


    // Создаём горизонтальные линии координатной сетки
    size_t horizontal_lines_count = 0;
    if (scaleY != 0) horizontal_lines_count = (static_cast<size_t>(wholeWidthWindow / std::abs(scaleY))) * 2;
    sf::VertexArray gorizontalLines(sf::Lines, horizontal_lines_count);
    if (scaleY != 0) {
       CreateCoordinateGrid(gorizontalLines, sf::Color{55, 55, 55, 100}, scaleX, wholeWidthWindow, wholeLengthWindow, 'y');
    }


    // Создаём вертикальные линии координатной сетки
    size_t vertical_lines_count = 0;
    if (scaleX != 0) vertical_lines_count = (static_cast<size_t>(wholeLengthWindow / std::abs(scaleX))) * 2;
    sf::VertexArray vertLines(sf::Lines, vertical_lines_count);
    if (scaleX != 0) {
        CreateCoordinateGrid(vertLines, sf::Color{55, 55, 55, 100}, scaleY, wholeLengthWindow, wholeWidthWindow, 'x');
    }

    // Создаём ось Ox с определёнными параметрами
    std::vector<sf::RectangleShape> OsX;
    CreateCoordiantesAxes(OsX, 800, scaleX, wholeLengthWindow, sf::Color::Black, 2.0f, 'y');

    // Создаём ось Oy с определёнными параметрами
    std::vector<sf::RectangleShape> OsY;
    CreateCoordiantesAxes(OsY, 1400, scaleY, wholeLengthWindow, sf::Color::Black, 2.0f, 'x');


    sf::Event event; // Объект события SFML

    // Создание и настройка вида (камеры)
    sf::View view = window.getDefaultView();

    float zoomLevel = 1.0f;         // Начальный уровень зума (1.0 - без зума)
    const float zoomFactor = 0.5f;  // Минимальный уровень зума
    const float maxZoomLevel = 5.0f; // Максимальный уровень зума

    bool isMouseDragging = false; // Флаг для отслеживания перетаскивания
    sf::Vector2i initialMousePos; // Начальная позиция мыши для перетаскивания

    sf::Vector2f movementLimit(1000, 1000);          // Максимальное перемещение центра вида от начальной позиции
    sf::Vector2f initialViewCenter = view.getCenter(); // Начальный центр вида

    // Главный цикл приложения
    while (window.isOpen()) {
        // Обработка событий
        while (window.pollEvent(event)) {
            // Обработка закрытия окна
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Обработка зума колесиком мыши
            if (event.type == sf::Event::MouseWheelScrolled) {
                const float zoomCoefficient = 1.1f; // Коэффициент изменения зума
                // Получаем позицию мыши в мировых координатах текущего вида
                sf::Vector2f mousePosView = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);

                // Определяем направление прокрутки колесика
                if (event.mouseWheelScroll.delta > 0) { // Колесико вверх (увеличение зума)
                    zoomLevel *= zoomCoefficient;
                    // Ограничение максимального уровня зума
                    if (zoomLevel > maxZoomLevel) {
                        zoomLevel = maxZoomLevel;
                    }
                } else if (event.mouseWheelScroll.delta < 0) { // Колесико вниз (уменьшение зума)
                    zoomLevel /= zoomCoefficient;
                    // Ограничение минимального уровня зума
                    if (zoomLevel < zoomFactor) {
                        zoomLevel = zoomFactor;
                    }
                }

                // Сохраняем старый центр и размер вида для корректировки
                sf::Vector2f oldCenter = view.getCenter();
                sf::Vector2f oldSize = view.getSize();

                // Устанавливаем новый размер вида относительно уровня зума
                view.setSize(window.getDefaultView().getSize() / zoomLevel);

                // Вычисляем новый центр вида так, чтобы точка под курсором осталась на месте
                sf::Vector2f newCenter = mousePosView - (mousePosView - oldCenter) * (view.getSize().x / oldSize.x);

                // Проверяем ограничения на перемещение центра вида
                sf::Vector2f deltaLimit = newCenter - initialViewCenter;
                ControlMovementLimit(deltaLimit, movementLimit, newCenter, initialViewCenter);

                // Корректируем центр, если он вышел за пределы ограничений по X
                if (std::abs(deltaLimit.x) > movementLimit.x) {
                    newCenter.x = initialViewCenter.x + (deltaLimit.x > 0 ? movementLimit.x : -movementLimit.x);
                }
                // Корректируем центр, если он вышел за пределы ограничений по Y
                if (std::abs(deltaLimit.y) > movementLimit.y) {
                    newCenter.y = initialViewCenter.y + (deltaLimit.y > 0 ? movementLimit.y : -movementLimit.y);
                }

                // Устанавливаем новый центр вида и применяем вид к окну
                view.setCenter(newCenter);
                window.setView(view);
            }

            // Обработка нажатия левой кнопки мыши для начала перетаскивания
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                isMouseDragging = true; // Устанавливаем флаг перетаскивания
                initialMousePos = sf::Mouse::getPosition(window); // Сохраняем начальную позицию мыши в окне
            }

            // Обработка отпускания левой кнопки мыши для окончания перетаскивания
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                isMouseDragging = false; // Сбрасываем флаг перетаскивания
            }

            // Обработка движения мыши во время перетаскивания
            if (isMouseDragging && event.type == sf::Event::MouseMoved) {
                // Текущая позиция мыши в окне
                sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
                // Вычисляем смещение в мировых координатах на основе движения мыши в координатах окна
                sf::Vector2f worldDelta = window.mapPixelToCoords(initialMousePos, view) - window.mapPixelToCoords(currentMousePos, view);

                // Вычисляем новый предлагаемый центр вида
                sf::Vector2f newCenter = view.getCenter() + worldDelta;

                // Проверяем ограничения на перемещение центра вида
                sf::Vector2f totalDelta = newCenter - initialViewCenter;
                ControlMovementLimit(totalDelta, movementLimit, newCenter, initialViewCenter);

                // Корректируем центр, если он вышел за пределы ограничений по X
                 if (std::abs(totalDelta.x) > movementLimit.x) {
                     newCenter.x = initialViewCenter.x + (totalDelta.x > 0 ? movementLimit.x : -movementLimit.x);
                 }
                 // Корректируем центр, если он вышел за пределы ограничений по Y
                 if (std::abs(totalDelta.y) > movementLimit.y) {
                     newCenter.y = initialViewCenter.y + (totalDelta.y > 0 ? movementLimit.y : -movementLimit.y);
                 }

                // Устанавливаем новый центр вида и применяем вид к окну
                view.setCenter(newCenter);
                window.setView(view);

                // Обновляем начальную позицию мыши для следующего шага перетаскивания
                initialMousePos = currentMousePos;
            }
        }

        // Очистка окна белым цветом
        window.clear(sf::Color::White);

        // Рисуем координатную сетку (сначала горизонтальные, потом вертикальные)
        if (scaleY != 0 && gorizontalLines.getVertexCount() > 0) window.draw(gorizontalLines);
        if (scaleX != 0 && vertLines.getVertexCount() > 0) window.draw(vertLines);

        // Рисуем оси Ox и Oy
        for (const auto& point : OsX) {
            window.draw(point);
        }
        for (const auto& point : OsY) {
            window.draw(point);
        }

        // Рисуем сам график (толстой линией)
        if (graph_vertex_array.getVertexCount() > 0) {
            window.draw(graph_vertex_array);
        }


        // Отображение всего нарисованного на окне
        window.display();
    }

    std::cout << "Окно закрыто." << std::endl;
}

} // namespace RealizationGraphicCalculator
