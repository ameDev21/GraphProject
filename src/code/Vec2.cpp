#include "../include/Vec2.h"
#include <cmath>

Vec2::Vec2() : x(0), y(0) {}

Vec2::Vec2(float xin, float yin) : x(xin), y(yin) {}

inline bool Vec2::operator==(const Vec2 &rhs) const {
  return (x == rhs.x && y == rhs.y);
}

inline bool Vec2::operator==(const float val) const {
  return (x == val && y == val);
}

inline bool Vec2::operator!=(const Vec2 &rhs) const {
  return (x != rhs.x && y != rhs.y);
}

Vec2 Vec2::operator+(const Vec2 &rhs) const { return {x + rhs.x, y + rhs.y}; }

Vec2 Vec2::operator-(const Vec2 &rhs) const { return {x - rhs.x, y - rhs.y}; }

Vec2 Vec2::operator/(const float val) const { return {x / val, y / val}; }

Vec2 Vec2::operator*(const float val) const { return {x * val, y * val}; }

bool Vec2::operator>(const float rhs) const { return ((x > rhs) && (y > rhs)); }

inline void Vec2::operator+=(const Vec2 &rhs) {
  x += rhs.x;
  y += rhs.y;
}

inline void Vec2::operator-=(const Vec2 &rhs) {
  x -= rhs.x;
  y -= rhs.y;
}

inline void Vec2::operator*=(const float val) {
  x *= val;
  y *= val;
}

inline void Vec2::operator/=(const float val) {
  x /= val;
  y /= val;
}

inline float Vec2::dist(const Vec2 &rhs) const {
  return sqrt(pow((x - rhs.x), 2) + pow(y - rhs.y, 2));
}

void Vec2::operator=(const sf::Vector2f vec) {
  x = vec.x;
  y = vec.y;
}

void Vec2::operator=(const sf::Vector2i vec) {
  x = vec.x;
  y = vec.y;
}

int Vec2::dist(const sf::Vector2f &lhs, const sf::Vector2f &rhs) {
  return std::sqrt(std::pow(lhs.x - rhs.x, 2) + std::pow(lhs.y - rhs.y, 2));
}
