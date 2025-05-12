#include <SFML/Graphics.hpp>

#include "../../lib/exprtk.hpp"

void CalculateGraphPoints(std::vector<sf::Vector2f>& graphPoints, double& x_var, const double scaleX, const double scaleY,  exprtk::expression<double>& expr, const int wholeLengthWindow ) {
     // Определение диапазона x-координат в мировом пространстве
    const double sfml_x_start = 700.0 - wholeLengthWindow / 2.0;
    const double sfml_x_end = 700.0 + wholeLengthWindow / 2.0;
    // Шаг по x для вычисления точек графика
    const double sfml_x_step = 0.1;

    // Вычисление предполагаемого количества точек
    size_t num_points = 0;
    if (sfml_x_step > 0.00001) { // Проверка шага, чтобы избежать деления на ноль
        num_points = static_cast<size_t>((sfml_x_end - sfml_x_start) / sfml_x_step) + 1;
    }

    // Вычисление точек графика
    if (num_points > 0) {
        graphPoints.reserve(num_points); // Резервирование памяти
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
            graphPoints.push_back(sf::Vector2f(static_cast<float>(current_sfml_x), static_cast<float>(sfml_y)));
        }
    }
}
