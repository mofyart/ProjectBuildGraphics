#include <SFML/Graphics.hpp>

// шаблонная функция для создания осей координат
void CreateCoordiantesAxes(std::vector<sf::RectangleShape>& axe, int quantityPixels, int scale, sf::Color color, float thickness, char constAxe){
    for (int i = -5000; i < 5000; i += scale) {
        sf::RectangleShape point;

        // определяем по какой оси координата остаётся постоянной
        if (constAxe == 'x') {
            point.setSize(sf::Vector2f(thickness, quantityPixels)); // Устанавливаем размеры линии
            point.setPosition((quantityPixels / 2) - 1, i - 1); // Устанавливаем позицию линии
        } else {
            point.setSize(sf::Vector2f(quantityPixels, thickness)); // Устанавливаем размеры линии
            point.setPosition(i - 1, (quantityPixels / 2) - 1); // Устанавливаем позицию линии
        }

        // устанавливаем цвет
        point.setFillColor(color);

        // добавляем линию в массив, представляющий из себя связанные между собой квадратики
        axe.push_back(point);
    }
}
