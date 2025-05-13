#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "../controlMovementLimit/index.cpp"
#include "../calculateGraphPoints/index.cpp"
#include "../generationTriangles/index.cpp"
#include "../../lib/exprtk.hpp"


void DrawApp(sf::RenderWindow& window,
    sf::VertexArray& gorizontalLines,
    sf::VertexArray& vertLines,
    std::vector<sf::RectangleShape>& OsX,
    std::vector<sf::RectangleShape>& OsY,
    sf::VertexArray& graphVertexArray,
    exprtk::expression<double>& expr,
    exprtk::symbol_table<double>& symbolTable,
    exprtk::parser<double>& parserRef,
    double& xVarRef,
    std::string& currentExpressionRef,
    std::vector<sf::Vector2f>& graphPointsRef,
    sf::RectangleShape& inputPanel,
    sf::Text& inputText,
    sf::Font& font,
    const double scaleX,
    const double scaleY,
    const int windowTotalLength,
    const int windowTotalWidth
    ) {

    sf::Event event; // Объект события SFML. Хранит информацаию о событии

    // Создание и настройка вида камеры
    sf::View view = window.getDefaultView();

    float zoomLevel = 1.0f;  // Начальный уровень зума (1.0 - без зума)
    const float maxZoomLevel = 5.0f; // Максимальный уровень зума
    const float minZoomFactor = 0.5f; // Минимальный уровень зума (возможно, стоит уменьшить)

    bool isMouseDragging = false; // Флаг для отслеживания перетаскивания
    sf::Vector2i initialMousePos; // Начальная позиция мыши для перетаскивания

    sf::Vector2f movementLimit(1150, 1150); // Максимальное перемещение центра вида от начальной позиции
    sf::Vector2f initialViewCenter = view.getCenter(); // Начальный центр вида

    // Флаг для отслеживания необходимости пересчета графика после ввода или изменения вида
    bool recalculateGraph = true;


    while (window.isOpen()) {
        // Обработка событий
        while (window.pollEvent(event)) {
            // Обработка закрытия окна
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Обработка ввода текста для панели
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') {
                    if (!currentExpressionRef.empty()) {
                        currentExpressionRef.pop_back(); // При нажатии на "backspace" удаляем символ из рамки
                        inputText.setString(currentExpressionRef); // Обновить отображаемый текст
                    }
                } else if (event.text.unicode == '\r' || event.text.unicode == '\n') {

                } else if (event.text.unicode >= 32 && event.text.unicode < 127) {
                    currentExpressionRef += static_cast<char>(event.text.unicode);
                    inputText.setString(currentExpressionRef);
                }
            }

            if (event.type == sf::Event::KeyPressed) { // нажатие кнопки
                if (event.key.code == sf::Keyboard::Enter) { // обработка события нажатия Enter
                    if (!currentExpressionRef.empty()) {
                        if (parserRef.compile(currentExpressionRef, expr)) {
                            inputText.setFillColor(sf::Color::Black); // Сброс цвета при успехе
                            recalculateGraph = true; // Установить флаг для пересчета графика
                        } else {
                            std::cerr << "Ошибка компиляции выражения: " << parserRef.error() << std::endl;
                            inputText.setFillColor(sf::Color::Red); // Индикация ошибки
                            graphVertexArray.clear(); // Показать пустой график при ошибке
                            graphPointsRef.clear(); // Очистить точки, чтобы избежать отрисовки старого графика
                        }
                    } else { // Если строка ввода пуста, можно скомпилировать "0" или очистить график
                        if (parserRef.compile("0", expr)) {
                             inputText.setFillColor(sf::Color::Black);
                             recalculateGraph = true; // Установить флаг для пересчета графика
                        } else {
                             // Это маловероятно для "0", но на всякий случай
                             inputText.setFillColor(sf::Color::Red);
                             graphVertexArray.clear();
                             graphPointsRef.clear();
                        }
                    }
                }
            }

            // Обработка зума колесиком мыши
            if (event.type == sf::Event::MouseWheelScrolled) {
                const float zoomCoefficient = 1.1f; // Коэффициент изменения зума (более плавный)
                sf::Vector2f mousePosView = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);

                if (event.mouseWheelScroll.delta > 0) {
                    zoomLevel *= zoomCoefficient;
                    if (zoomLevel > maxZoomLevel) {
                        zoomLevel = maxZoomLevel;
                    }
                } else if (event.mouseWheelScroll.delta < 0) {
                    zoomLevel /= zoomCoefficient;
                    if (zoomLevel < minZoomFactor) {
                        zoomLevel = minZoomFactor;
                    }
                }

                sf::Vector2f oldCenter = view.getCenter();
                sf::Vector2f oldSize = view.getSize();
                view.setSize(window.getDefaultView().getSize() / zoomLevel);
                 // Расчет нового центра вида так, чтобы точка под курсором осталась на месте
                sf::Vector2f newCenter = mousePosView - (mousePosView - oldCenter) * (view.getSize().x / oldSize.x);

                // Проверяем ограничения на перемещение центра вида
                sf::Vector2f totalDelta = newCenter - initialViewCenter;
                ControlMovementLimit(totalDelta, movementLimit, newCenter, initialViewCenter);
                // Проверка и коррекция ограничений вручную
                sf::Vector2f deltaFromInitial = newCenter - initialViewCenter;
                if (std::abs(deltaFromInitial.x) > movementLimit.x) {
                    newCenter.x = initialViewCenter.x + (deltaFromInitial.x > 0 ? movementLimit.x : -movementLimit.x);
                }
                if (std::abs(deltaFromInitial.y) > movementLimit.y) {
                    newCenter.y = initialViewCenter.y + (deltaFromInitial.y > 0 ? movementLimit.y : -movementLimit.y);
                }

                view.setCenter(newCenter);
                recalculateGraph = true; // Установить флаг для пересчета графика после зума
            }

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

                recalculateGraph = true; // Установить флаг для пересчета графика после перетаскивания
            }

            // Обработка нажатия левой кнопки мыши для начала перетаскивания
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    isMouseDragging = true;
                    initialMousePos = sf::Mouse::getPosition(window);
            }

            // Обработка отпускания левой кнопки мыши для окончания перетаскивания
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                isMouseDragging = false;
            }
        } // Конец цикла обработки событий

        // Пересчет графика, если установлен флаг (после ввода выражения или изменения вида)
        if (recalculateGraph) {
             // Проверяем, что выражение скомпилировано успешно перед пересчетом точек
             if (parserRef.compile(currentExpressionRef.empty() ? "0" : currentExpressionRef, expr)) {
                graphPointsRef.clear();

                CalculateGraphPoints(graphPointsRef, xVarRef, scaleX, scaleY, expr, view);
                graphVertexArray.clear(); // Очищаем старые вершины

                CreateThicknessGraphics(graphVertexArray, graphPointsRef);
                inputText.setFillColor(sf::Color::Black); // Сброс цвета текста ввода при успешной компиляции
             } else {
                 // Если компиляция текущего выражения (или "0") неуспешна,
                 // то при изменении вида график все равно очищается или остается пустым.
                 graphVertexArray.clear();
                 graphPointsRef.clear();
             }
             recalculateGraph = false; // Сбросить флаг после пересчета
        }


        window.setView(view); // Применяем вид ко всему окну перед отрисовкой мира

        // Очистка окна белым цветом
        window.clear(sf::Color::White);

        // Рисуем координатную сетку
        if (scaleY != 0 && gorizontalLines.getVertexCount() > 0) window.draw(gorizontalLines);
        if (scaleX != 0 && vertLines.getVertexCount() > 0) window.draw(vertLines);

        // Рисуем оси Ox и Oy
        for (const auto& point : OsX) {
            window.draw(point);
        }
        for (const auto& point : OsY) {
            window.draw(point);
        }

        // Рисуем сам график
        if (graphVertexArray.getVertexCount() > 0) {
            window.draw(graphVertexArray);
        }

        // Рисуем элементы интерфейса в экранных координатах
        sf::View currentGraphView = window.getView(); // Сохраняем текущий вид графика
        window.setView(window.getDefaultView()); // Устанавливаем вид по умолчанию

        window.draw(inputPanel); // Рисуем фон панели
        window.draw(inputText); // Рисуем текст ввода

        window.setView(currentGraphView); // Восстанавливаем вид графика

        // Отображение всего нарисованного на окне
        window.display();
    }
}
