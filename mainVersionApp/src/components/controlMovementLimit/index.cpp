#include <SFML/Graphics.hpp>

void ControlMovementLimit(sf::Vector2f delta, sf::Vector2f movementLimit, sf::Vector2f& newCenter, sf::Vector2f initialViewCenter) {
    // Если происходит выход за пределы границы, то происходит смещения центра в пределы границы по оси OX
    if (std::abs(delta.x) > movementLimit.x) {
        newCenter.x = initialViewCenter.x + (delta.x > 0 ? movementLimit.x : -movementLimit.x);
    }
    // Если происходит выход за пределы границы, то происходит смещения центра в пределы границы по оси OY
    if (std::abs(delta.y) > movementLimit.y) {
        newCenter.y = initialViewCenter.y + (delta.y > 0 ? movementLimit.y : -movementLimit.y);
    }
}
