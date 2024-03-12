#include "Particle.h"

#include <iostream>
#include <numbers>
#include <random>

namespace
{
std::random_device gDevice;
std::default_random_engine gEngine{gDevice()};
std::uniform_int_distribution gAngleDistr{-180, 180};
std::uniform_int_distribution gSpeedDistr{1, 100};
std::uniform_int_distribution gLifeTimeDistr{5, 10};
std::uniform_int_distribution gColorDistr{0, 255};

const sf::Vector2f ACCELERATION = {0.0f, 10.f};

float degToRad(const float angle) { return angle * std::numbers::pi / 180.f; }

sf::Vector2f getRandomVelocity()
{
  const auto angle = degToRad(static_cast<float>(gAngleDistr(gEngine)));
  const auto tmp = static_cast<float>(gSpeedDistr(gEngine));
  return sf::Vector2f(::cosf(angle), ::sinf(angle)) * tmp;
}
}  // namespace

Particle::Particle(float posX, float posY)
    : position(posX, posY),
      mColor({255,
              static_cast<sf::Uint8>(gColorDistr(gEngine)),
              static_cast<sf::Uint8>(gColorDistr(gEngine))}),
      speed(getRandomVelocity()),
      lifetime(gLifeTimeDistr(gEngine)),
      secondsElapsed(0)
{
}

void Particle::update(float deltaTime)
{
  if (deltaTime <= 0 || !isAlive())
  {
    return;
  }

  if (!tail.empty() && tail.size() > 20)
  {
    tail.pop_front();
  }

  tail.emplace_back(position, 0);
  for (size_t i = 0; i < tail.size(); ++i)
  {
    tail[tail.size() - 1 - i].second = mColor;
    tail[tail.size() - 1 - i].second.a =
        mColor.a * (static_cast<float>(tail.size() - i) / tail.size());
  }

  position += speed * (deltaTime);
  speed += ACCELERATION * (deltaTime);

  if (secondsElapsed < lifetime)
  {
    mColor.a = (lifetime - secondsElapsed) / lifetime * 255;
  }
  else
  {
    mColor.a = 0;
  }

  secondsElapsed += deltaTime;
}

bool Particle::isAlive() const
{
  return secondsElapsed <= lifetime &&
         (position.x <= 800 && position.y <= 600 && position.x >= 0 &&
          position.y >= 0);
}

sf::Vector2f Particle::getPosition() const { return position; }

sf::Color Particle::getColor() const { return mColor; }

const std::deque<std::pair<sf::Vector2f, sf::Color>>& Particle::getTail() const
{
  return tail;
}