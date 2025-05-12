#include <SFML/Graphics.hpp>
#include <cmath>

#include "../../lib/constLib.hpp"


void CraeteThicknessGraphics(sf::VertexArray& graphVertexArray, const std::vector<sf::Vector2f>& graphPoints) {
     // Генерация треугольников для толстой линии графика
    if (graphPoints.size() >= 2) {
        // Для N точек будет N-1 отрезков. Каждый отрезок - это прямоугольник.
        graphVertexArray.resize((graphPoints.size() - 1) * 6);
        size_t current_vertex = 0;

        // Проходим по всем парам соседних точек
        for (size_t i = 0; i < graphPoints.size() - 1; ++i) {
            sf::Vector2f p1 = graphPoints[i];
            sf::Vector2f p2 = graphPoints[i + 1];

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
            graphVertexArray[current_vertex++] = sf::Vertex(v1, sf::Color::Red); // Цвет графика - зеленый, как в вашем коде
            graphVertexArray[current_vertex++] = sf::Vertex(v2, sf::Color::Red);
            graphVertexArray[current_vertex++] = sf::Vertex(v3, sf::Color::Red);

            // Добавляем вершины второго треугольника (v3, v2, v4) - для создания прямоугольника
            graphVertexArray[current_vertex++] = sf::Vertex(v3, sf::Color::Red);
            graphVertexArray[current_vertex++] = sf::Vertex(v2, sf::Color::Red);
            graphVertexArray[current_vertex++] = sf::Vertex(v4, sf::Color::Red);
        }

        // Обрезаем VertexArray, если были пропущенные сегменты
        graphVertexArray.resize(current_vertex);
    }
}
