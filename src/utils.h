#include <cmath>

#include <SFML/Graphics.hpp>

#pragma once

//some basic linear algebra utility functions
float magnitude(const sf::Vector2f& vec) {
    return float(std::sqrt(std::pow(vec.x, 2.f) + std::pow(vec.y, 2.f)));
}
sf::Vector2f normalize(const sf::Vector2f& vec) {
    return vec / magnitude(vec);
}
float dot(const sf::Vector2f& lhs, const sf::Vector2f& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
sf::Vector2f projection(const sf::Vector2f& vec, const sf::Vector2f& base) {
    return dot(vec, base) / dot(base, base) * base;
}
sf::Vector2f rotateVector(const sf::Vector2f& vec, float angle) {
    sf::Transform rotation;
    rotation.rotate(angle);
    return rotation.transformPoint(vec);
}
