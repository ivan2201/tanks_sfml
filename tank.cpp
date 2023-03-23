#include "tank.hpp"

baho::Tank::Tank(us (&path)[FIELD_WIDTH][FIELD_WIDTH], int id):
  m_tankMidWidth(0.45),
  m_midWidthPlusEps(0.46),
  m_time(0.f),
  m_isDestroyed(false),
  m_destroyAnimationFinish(false),
  m_rotation(BIT_UPKEY),
  m_field(Field::getField()),
  m_path(path)
{
  if (id == -1)
  {
    static int current_id = -1;
    m_id = ++current_id;
    m_team = m_id;
  }
}

bool baho::Tank::containPoint(float x, float y) const
{
  if (x <= (m_x + m_tankMidWidth) && x >= (m_x - m_tankMidWidth)
      && y <= (m_y + m_tankMidWidth) && y >= (m_y - m_tankMidWidth))
        return true;
  return false;
}

int baho::Tank::getId() const
{
  return m_id;
}

void baho::Tank::setId(int id)
{
  m_id = id;
}

int baho::Tank::getTeam() const
{
  return m_team;
}

void baho::Tank::setTeam(int team)
{
  m_team = team;
}

Bullet baho::Tank::shot(Bullet prototype)
{
  point2f_t point;
  int length = Field::getField().getCellLength();
  float width = static_cast< float >(prototype.getFrameRect().width) / 2 / length;
  float height = static_cast< float >(prototype.getFrameRect().height) / 2 / length;
  switch (m_rotation)
  {
  case BIT_LEFTKEY:
    point.x = m_x - m_tankMidWidth - width;
    point.y = m_y;
    break;
  case BIT_RIGHTKEY:
    point.x = m_x + m_tankMidWidth + width;
    point.y = m_y;
    break;
  case BIT_UPKEY:
    point.x = m_x;
    point.y = m_y - m_tankMidWidth - height;
    break;
  case BIT_DOWNKEY:
    point.x = m_x;
    point.y = m_y + m_tankMidWidth + height;
  }
  prototype.setPosition(point, m_rotation);
  prototype.setTankId(m_id);
  return prototype;
}

void baho::Tank::destroy()
{
  m_isDestroyed = true;
}

bool baho::Tank::isDestroyed() const
{
  return m_isDestroyed;
}

bool baho::Tank::mayDelete() const
{
  return m_destroyAnimationFinish;
}

void baho::Tank::setPosition(point_t begin)
{
  m_x = begin.x + 0.5f;
  m_y = begin.y + 0.5f;
  square_t square = m_field.getBoundSquare();
  m_image.setPosition(square.left + m_x * m_field.getCellLength(), 
      square.top + m_y * m_field.getCellLength());
}

float baho::Tank::getX() const
{
  return m_x;
}

float baho::Tank::getY() const
{
  return m_y;
}

void baho::Tank::setSprite(const sf::Sprite & sprite)
{
  m_image = sprite;
  m_image.setOrigin(sf::Vector2f(m_image.getTexture()->getSize()) / 2.f);
  auto bound = m_image.getLocalBounds();
  square_t square = m_field.getBoundSquare();
  m_image.setPosition(square.left + m_x * m_field.getCellLength(), 
      square.top + m_y * m_field.getCellLength());
  m_image.setScale(2 * m_tankMidWidth * m_field.getCellLength() / bound.width, 
      2 * m_tankMidWidth * m_field.getCellLength() / bound.height);
  m_isDestroyed = false;
  m_destroyAnimationFinish = false;
}

baho::Enemy::Enemy(us (&path)[FIELD_WIDTH][FIELD_WIDTH]):
  Tank(path)
{
}

void baho::Enemy::draw(sf::RenderWindow & view)
{
  view.draw(m_image);
}

float baho::Enemy::getSpeed()
{
  return 1.6f;
}

baho::MyTank::MyTank(us (&path)[FIELD_WIDTH][FIELD_WIDTH]):
  Tank(path)
{
}

void baho::Enemy::move(float deltaTime)
{
  if (m_isDestroyed)
  {
    if (m_time >= 3.0f)
    {
      m_destroyAnimationFinish = true;
      return;
    }
    m_image.setColor(sf::Color(0xFF,0xFF,0xFF,static_cast < uc >(255 - 255 / 3 * m_time)));
    m_time += deltaTime;
    return;
  }
  uc min = 0;
  us minDirection = Field::NOPATH;
  int left = static_cast< uc >(m_x - 0.51);
  int right = static_cast< uc >(m_x + 0.51);
  int up = static_cast< uc >(m_y - 0.51);
  int down = static_cast< uc >(m_y + 0.51);
  int left1 = static_cast< uc >(m_x - m_tankMidWidth);
  int right1 = static_cast< uc >(m_x + m_tankMidWidth);
  int up1 = static_cast< uc >(m_y - m_tankMidWidth);
  int down1 = static_cast< uc >(m_y + m_tankMidWidth);
  bool were_barrier = false;
  if ((m_x >= 1) && (minDirection >= m_path[left][up1] || minDirection >= m_path[left][down1])
      && (Field::NOPATH != m_path[left][up1] && Field::NOPATH != m_path[left][down1]))
  {
    if (!(m_field[left][up1].type & BIT_LOCK) && !(m_field[left][down1].type & BIT_LOCK))
    {
      were_barrier = true;
    }
    if (m_path[left][up1] < m_path[left][down1])
    {
      minDirection = m_path[left][up1];
    }
    else
    {
      minDirection = m_path[left][down1];
    }
    min = BIT_LEFTKEY;
  }
  if (((m_x + 1) < FIELD_WIDTH) && ((minDirection > m_path[right][up1] || minDirection > m_path[right][down1])
      || (were_barrier && (minDirection == m_path[right][up1] || minDirection == m_path[right][down1])))
        && (Field::NOPATH != m_path[right][up1] && Field::NOPATH != m_path[right][down1]))
  {
    if (!(m_field[right][up1].type & BIT_LOCK) && !(m_field[right][down1].type & BIT_LOCK))
    {
      were_barrier = true;
    }
    if (m_path[right][up1] < m_path[right][down1])
    {
      minDirection = m_path[right][up1];
    }
    else
    {
      minDirection = m_path[right][down1];
    }
    min = BIT_RIGHTKEY;
  }
  if ((m_y >= 1) && ((minDirection > m_path[left1][up] || minDirection > m_path[right1][up]) 
      || (were_barrier && (minDirection == m_path[left1][up] || minDirection == m_path[right1][up])))
      && (Field::NOPATH != m_path[left1][up] && Field::NOPATH != m_path[right1][up]))
  {
    if (!(m_field[left1][up].type & BIT_LOCK) && !(m_field[right1][up].type & BIT_LOCK))
    {
      were_barrier = true;
    }
    if (m_path[left1][up] < m_path[right1][up])
    {
      minDirection = m_path[left1][up];
    }
    else
    {
      minDirection = m_path[right1][up];
    }
    min = BIT_UPKEY;
  }
  if (((m_y + 1) < FIELD_WIDTH) && ((minDirection > m_path[left1][down] || minDirection > m_path[right1][down]) 
      || (were_barrier && (minDirection == m_path[left1][down] || minDirection == m_path[right1][down])))
      && ((Field::NOPATH != m_path[left1][down]) && (Field::NOPATH != m_path[right1][down])))
  {
    if (m_path[left1][down] < m_path[right1][down])
    {
      minDirection = m_path[left1][down];
    }
    else
    {
      minDirection = m_path[right1][down];
    }
    min = BIT_DOWNKEY;
  }
  if (minDirection == Field::NOPATH) return;
  float speed = getSpeed();
  if (minDirection < 2) speed = 0;
  left = static_cast< uc >(m_x - m_tankMidWidth);
  right = static_cast< uc >(m_x + m_tankMidWidth);
  up = static_cast< uc >(m_y - m_tankMidWidth);
  down = static_cast< uc >(m_y + m_tankMidWidth);
  switch (min)
  {
  case BIT_LEFTKEY:
    if (m_rotation != BIT_LEFTKEY)
    {
      m_image.setRotation(270.f);
      m_rotation = BIT_LEFTKEY;
    }
    if ((m_x > 1) && !(m_field[left][up].type & BIT_LOCK) && !(m_field[left][down].type & BIT_LOCK))
    {
      m_x -= speed * deltaTime;
      if ((m_field[static_cast< uc >(m_x - m_tankMidWidth)][up].type & BIT_LOCK) ||
          (m_field[static_cast< uc >(m_x - m_tankMidWidth)][down].type & BIT_LOCK))
            m_x = static_cast< uc >(m_x) + m_midWidthPlusEps;
    }
    else return;
    break;
  case BIT_RIGHTKEY:
    if (m_rotation != BIT_RIGHTKEY)
    {
      m_image.setRotation(90.f);
      m_rotation = BIT_RIGHTKEY;
    }
    if (((m_x + 1) < FIELD_WIDTH) && !(m_field[right][up].type & BIT_LOCK) &&
                                     !(m_field[right][down].type & BIT_LOCK))
    {
      m_x += speed * deltaTime;
      if ((m_field[static_cast< uc >(m_x + m_tankMidWidth)][up].type & BIT_LOCK) ||
          (m_field[static_cast< uc >(m_x + m_tankMidWidth)][down].type & BIT_LOCK))
      {
        m_x = static_cast< uc >(m_x + m_midWidthPlusEps) - m_midWidthPlusEps;
      }
    }
    else return;
    break;
  case BIT_UPKEY:
    if (m_rotation != BIT_UPKEY)
    {
      m_image.setRotation(0.f);
      m_rotation = BIT_UPKEY;
    }
    if ((m_y > 1) && !(m_field[left][up].type & BIT_LOCK) && !(m_field[right][up].type & BIT_LOCK))
    {
      m_y -= speed * deltaTime;
      if ((m_field[left][static_cast< uc >(m_y - m_tankMidWidth)].type & BIT_LOCK) ||
          (m_field[right][static_cast< uc >(m_y - m_tankMidWidth)].type & BIT_LOCK))
      {
        m_y = static_cast< uc >(m_y) + m_midWidthPlusEps;
      }
    }
    else return;
    break;
  case BIT_DOWNKEY:
    if (m_rotation != BIT_DOWNKEY)
    {
      m_image.setRotation(180.f);
      m_rotation = BIT_DOWNKEY;
    }
    if (((m_y + 1) < FIELD_WIDTH) && !(m_field[left][down].type & BIT_LOCK)
          && !(m_field[right][down].type & BIT_LOCK))
    {
      m_y += speed * deltaTime;
      if ((m_field[left][static_cast< uc >(m_y + m_tankMidWidth)].type & BIT_LOCK) ||
          (m_field[right][static_cast< uc >(m_y + m_tankMidWidth)].type & BIT_LOCK))
      {
        m_y = static_cast< uc >(m_y + m_midWidthPlusEps) - m_midWidthPlusEps;
      }
    }
    else return;
    break;
  default:
    return;
  }
  square_t square = m_field.getBoundSquare();
  m_image.setPosition(square.left + m_x * m_field.getCellLength(), 
      square.top + m_y * m_field.getCellLength());
}

void baho::MyTank::draw(sf::RenderWindow & view)
{
  view.draw(m_image);
}

void baho::MyTank::move(uc type, float deltaTime)
{
  if (m_isDestroyed)
  {
    if (m_time >= 3.0f)
    {
      m_destroyAnimationFinish = true;
      return;
    }
    m_image.setColor(sf::Color(0xFF, 0xFF, 0xFF, static_cast < uc >(255 - 255 / 3 * m_time)));
    m_time += deltaTime;
    return;
  }
  float wereX = m_x;
  const uc left = static_cast< uc >(m_x - m_tankMidWidth);
  const uc right = static_cast< uc >(m_x + m_tankMidWidth);
  const uc up = static_cast< uc >(m_y - m_tankMidWidth);
  const uc down = static_cast< uc >(m_y + m_tankMidWidth);
  if (type == 0) return;
  float speed = getSpeed();
  if (type & BIT_LEFTKEY)
  {
    if (m_rotation != BIT_LEFTKEY)
    {
      m_image.setRotation(270.f);
      m_rotation = BIT_LEFTKEY;
    }
    if ((m_x > 1) && !(m_field[left][up].type & BIT_LOCK) && !(m_field[left][down].type & BIT_LOCK))
    {
      m_x -= speed * deltaTime;
      if ((m_field[static_cast< uc >(m_x - m_midWidthPlusEps)][up].type & BIT_LOCK) ||
          (m_field[static_cast< uc >(m_x - m_midWidthPlusEps)][down].type & BIT_LOCK))
      {
        m_x = static_cast< uc >(m_x) + m_midWidthPlusEps;
      }
    }
  }
  if (type & BIT_RIGHTKEY)
  {
    if (m_rotation != BIT_RIGHTKEY)
    {
      m_image.setRotation(90.f);
      m_rotation = BIT_RIGHTKEY;
    }
    if ((m_x + 1 < FIELD_WIDTH) && !(m_field[right][up].type & BIT_LOCK) && 
                                   !(m_field[right][down].type & BIT_LOCK))
    {
      m_x += speed * deltaTime;
      if ((m_field[static_cast< uc >(m_x + m_midWidthPlusEps)][up].type & BIT_LOCK) ||
          (m_field[static_cast< uc >(m_x + m_midWidthPlusEps)][down].type & BIT_LOCK))
      {
        m_x = static_cast< uc >(m_x + m_midWidthPlusEps) - m_midWidthPlusEps; 
      }
    }
  }
  if (type & BIT_UPKEY)
  {
    if (m_rotation != BIT_UPKEY)
    {
      m_image.setRotation(0.f);
      m_rotation = BIT_UPKEY;
    }
    if ((m_y > 1) && !(m_field[left][up].type & BIT_LOCK) &&
                     !(m_field[right][up].type & BIT_LOCK))
    {
      m_y -= speed * deltaTime;
      if ((m_field[left][static_cast< uc >(m_y - m_midWidthPlusEps)].type & BIT_LOCK) ||
          (m_field[right][static_cast< uc >(m_y - m_midWidthPlusEps)].type & BIT_LOCK))
      {
        m_y = static_cast< uc >(m_y) + m_midWidthPlusEps;
      }
      else
      {
        m_x = wereX;
      }
    }
  }
  if (type & BIT_DOWNKEY)
  {
    uc wereRotation = m_rotation;
    if (m_rotation != BIT_DOWNKEY)
    {
      m_image.setRotation(180.f);
      m_rotation = BIT_DOWNKEY;
    }
    if ((m_y + 1 < FIELD_WIDTH) && !(m_field[left][down].type & BIT_LOCK) &&
                                   !(m_field[right][down].type & BIT_LOCK))
    {
      m_y += speed * deltaTime;
      if((m_field[left][static_cast< uc >(m_y + m_midWidthPlusEps)].type & BIT_LOCK) ||
         (m_field[right][static_cast< uc >(m_y + m_midWidthPlusEps)].type & BIT_LOCK))
      {
        m_y = static_cast< uc >(m_y + m_midWidthPlusEps) - m_midWidthPlusEps;
      }
      else
      {
        m_x = wereX;
      }
    }
  }
  square_t square = m_field.getBoundSquare();
  m_image.setPosition(square.left + m_x * m_field.getCellLength(), 
      square.top + m_y * m_field.getCellLength());
}

float baho::MyTank::getSpeed()
{
  return 2.0f;
} 
