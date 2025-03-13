#include <SFML/Graphics.hpp>

void CreateCoordinateGrid(sf::VertexArray& nameAxe, sf::Color color, int scale, int quantityPixels, int coordinateSecondPoint, char constCoordinate) {
    for (int i = 0, pixel = 0; i < (quantityPixels / scale) * 2; i += 2, pixel += scale){

        if (constCoordinate == 'x') {
            nameAxe[i].position = sf::Vector2f(pixel - coordinateSecondPoint / 2, -quantityPixels);
            nameAxe[i + 1].position = sf::Vector2f(pixel - coordinateSecondPoint / 2, coordinateSecondPoint);
        } else {
            nameAxe[i].position = sf::Vector2f(-quantityPixels, pixel - coordinateSecondPoint / 2);
            nameAxe[i + 1].position = sf::Vector2f(coordinateSecondPoint, pixel - coordinateSecondPoint / 2);
        }

        nameAxe[i].color =  color;
        nameAxe[i + 1].color = color;
    }
}
