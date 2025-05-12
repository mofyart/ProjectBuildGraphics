#include <SFML/Graphics.hpp>
#include "../controlMovementLimit/index.cpp"

// Главный цикл приложения
void DrawApp(sf::RenderWindow& window, const double scaleX, const double scaleY, sf::VertexArray& gorizontalLines, sf::VertexArray& vertLines,  std::vector<sf::RectangleShape>& OsX,  std::vector<sf::RectangleShape>& OsY, sf::VertexArray& graphVertexArray) {
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
        if (graphVertexArray.getVertexCount() > 0) {
            window.draw(graphVertexArray);
        }


        // Отображение всего нарисованного на окне
        window.display();
    }
}
