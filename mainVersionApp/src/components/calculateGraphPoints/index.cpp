#include <SFML/Graphics.hpp>
#include "../../lib/exprtk.hpp"
#include <vector> // Убедитесь, что vector включен

// Добавлен параметр const sf::View& view
void CalculateGraphPoints(std::vector<sf::Vector2f>& graphPoints, double& xVar, const double scaleX, const double scaleY, exprtk::expression<double>& expr, const sf::View& view) {
    // Определение диапазона x-координат в мировом пространстве на основе текущего вида
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();

    const double sfmlXStart = viewCenter.x - viewSize.x / 2.0;
    const double sfmlXEnd = viewCenter.x + viewSize.x / 2.0;

    // Шаг по x для вычисления точек графика.
    const double sfmlXStep = 0.1;

    // Вычисление предполагаемого количества точек
    size_t num_points = 0;
    if (sfmlXStep > 0.00001) { // Проверка шага, чтобы избежать деления на ноль
        num_points = static_cast<size_t>((sfmlXEnd - sfmlXStart) / sfmlXStep) + 1;
    }

    // Очистка предыдущих точек и резервирование памяти
    graphPoints.clear();
    if (num_points > 0) {
        graphPoints.reserve(num_points);

        for (size_t i = 0; i < num_points; ++i) {
            // Текущая x-координата в мировом пространстве SFML
            double currentSfmlX = sfmlXStart + i * sfmlXStep;

            // Преобразование SFML x-координаты в математическую x
            if (scaleX == 0) {
                 // Обработка случая scaleX=0 зависит от требуемого поведения.
                 // Если это недопустимо, можно пропустить точку или вывести ошибку.
                 continue;
            }
            xVar = (currentSfmlX - 700.0) / scaleX;

            // Вычисление математической y-координаты
            double mathY = expr.value();

            // Преобразование математической y-координаты в SFML y-координату
             if (scaleY == 0) {
                 continue;
            }
            double sfmlY = 400.0 - mathY * scaleY;

            // Добавление точки в вектор, отвкчающий за построение грфаика
            graphPoints.push_back(sf::Vector2f(static_cast<float>(currentSfmlX), static_cast<float>(sfmlY)));
        }
    }
}
