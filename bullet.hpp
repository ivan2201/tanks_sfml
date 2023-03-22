#ifndef BULLET_HPP
#define BULLET_HPP
#include <SFML/Graphics.hpp>
#include "definition.hpp"
#include "field.hpp"

class Bullet
{
public:
  Bullet();
  ~Bullet();
  
  static constexpr float speed = 5.0f;
  
  void setSprite(sf::Sprite image);
  void setPosition(point2f_t begin, uc rotation);
  void setTankId(int id);
  int getTankId() const;
  
  bool containPoint(float x, float y) const;
  void move(float deltaT);
  void draw(sf::RenderWindow & view);
  float getX() const;
  float getY() const;
  sf::FloatRect getFrameRect() const;
  int getTeam() const;
  void setTeam(int team);
  
private:
  int m_tankId;
  float m_x;
  float m_y;
  short m_height;
  short m_width;
  uc m_rotation;
  sf::Sprite m_image;
};

#endif // BULLET_HPP
