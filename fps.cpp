#include "fps.hpp"

void baho::waitNextFrame(float coef) {
  // Create a clock for measuring elapsed time
  sf::Clock clock;
  auto endWait = clock.getElapsedTime().asSeconds() + (coef / FRAMERATE_LIMIT);
  while (clock.getElapsedTime().asSeconds() < endWait);
}
