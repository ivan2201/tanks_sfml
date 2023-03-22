#include "bullet.hpp"

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
  
}

bool Bullet::containPoint(float x, float y) const
{
  float fieldWidth = Field::getField().getCellLength();
  if (x <= (m_x + getFrameRect().width / 2 / fieldWidth) && x >= (m_x - getFrameRect().width / 2 / fieldWidth)
      && y <= (m_y + getFrameRect().height / 2  / fieldWidth)
        && y >= (m_y - getFrameRect().height / 2  / fieldWidth))
        return true;
  return false;
}

void Bullet::setTankId(int id)
{
  m_tankId = id;
}

int Bullet::getTankId() const
{
  return m_tankId;
}

void Bullet::setSprite(sf::Sprite image)
{
  Field & m_field = Field::getField();
  m_image = image;
  m_image.setOrigin(sf::Vector2f(m_image.getTexture()->getSize()) / 2.f);
  auto bound = m_image.getGlobalBounds();
  square_t square = Field::getField().getBoundSquare();
  m_image.setPosition(square.left + m_x * m_field.getCellLength(), 
      square.top + m_y * m_field.getCellLength());
  m_image.setScale( m_field.getCellLength() / 8 / bound.width, 
      m_field.getCellLength() / 4 / bound.height);
}

void Bullet::setPosition(point2f_t begin, uc rotation)
{
  m_x = begin.x;
  m_y = begin.y;
  Field & m_field = Field::getField();
  square_t square = m_field.getBoundSquare();
  m_image.setPosition(square.left + m_x * m_field.getCellLength(), 
      square.top + m_y * m_field.getCellLength());
  m_rotation = rotation;
  switch (m_rotation)
  {
  case BIT_LEFTKEY:
    m_image.setRotation(270.f);
    break;
  case BIT_RIGHTKEY:
    m_image.setRotation(90.f);
    break;
  case BIT_UPKEY:
    m_image.setRotation(0.f);
    break;
  case BIT_DOWNKEY:
    m_image.setRotation(180.f);
  }
}

sf::FloatRect Bullet::getFrameRect() const
{
  return m_image.getGlobalBounds();
}
  
void Bullet::move(float deltaTime)
{
   Field & m_field = Field::getField();
  switch (m_rotation)
  {
    case BIT_LEFTKEY:
    m_x -= speed * deltaTime;
    break;
    case BIT_RIGHTKEY:
    m_x += speed * deltaTime;
    break;
    case BIT_UPKEY:
    m_y -= speed * deltaTime;
    break;
    case BIT_DOWNKEY:
    m_y += speed * deltaTime;
  }
  square_t square = m_field.getBoundSquare();
  m_image.setPosition(square.left + m_x * m_field.getCellLength(), 
      square.top + m_y * m_field.getCellLength());
}

void Bullet::draw(sf::RenderWindow & view)
{
  view.draw(m_image);
}

float Bullet::getX() const
{
  return m_x;
}

float Bullet::getY() const
{
  return m_y;
}
