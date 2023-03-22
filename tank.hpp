#ifndef TANK_HPP
#define TANK_HPP
#include <SFML/Graphics.hpp>
#include "field.hpp"
#include "definition.hpp"
#include "bullet.hpp"
namespace baho
{
  class Tank
  {
  public:
    Tank(us (&path)[FIELD_WIDTH][FIELD_WIDTH], int id = -1);
    
  public:
  
    bool containPoint(float x, float y) const;
    
    void setPosition(point_t begin);
    void setSprite(const sf::Sprite & sprite);
    Bullet shot(Bullet prototype);
    double getX() const;
    double getY() const;
    void destroy();
    bool isDestroyed() const;
    bool mayDelete() const;
    int getId() const;
    void setId(int id);
    int getTeam() const;
    void setTeam(int team);
    
    virtual void draw(sf::RenderWindow & view) = 0;
    virtual float getSpeed() = 0;
  protected:
    int m_team;
    int m_id;
    float m_tankMidWidth;
    float m_midWidthPlusEps;
    float m_time;
    bool m_isDestroyed;
    bool m_destroyAnimationFinish;
    float m_x;
    float m_y;
    sf::Sprite m_image;
    uc m_rotation;

    Field & m_field;
    us (&m_path)[FIELD_WIDTH][FIELD_WIDTH];
  };

  class Enemy: public Tank
  {
  public:
    Enemy(us (&path)[FIELD_WIDTH][FIELD_WIDTH]);
    virtual void draw(sf::RenderWindow & view) override;
    void move(float deltaTime);
    virtual float getSpeed();
    
    template< class iterator1, class iterator2 >
    void move(float deltaTime, iterator1 beginTanks, iterator2 endTanks)
    {
      iterator1 current = beginTanks;
      float wereX = m_x;
      float wereY = m_y;
      move(deltaTime);
      float left = m_x - m_tankMidWidth;
      float right = m_x + m_tankMidWidth;
      float up = m_y - m_tankMidWidth;
      float down = m_y + m_tankMidWidth;
      float left1 = m_x - m_midWidthPlusEps;
      float right1 = m_x + m_midWidthPlusEps;
      float up1 = m_y - m_midWidthPlusEps;
      float down1 = m_y + m_midWidthPlusEps;
      while ((current != endTanks))
      {
        if (current->getId() == m_id) 
        {
          current++;
          continue;
        }
        bool bad = false;
        switch (m_rotation)
        {
          case BIT_LEFTKEY:
          if (current->containPoint(left1, up) || current->containPoint(left1, down)) bad = true;
          break;
          case BIT_RIGHTKEY:
          if (current->containPoint(right1, up) || current->containPoint(right1, down)) bad = true;
          break;
          case BIT_UPKEY:
          if (current->containPoint(left, up1) || current->containPoint(right, up1)) bad = true;
          break;
          case BIT_DOWNKEY:
          if (current->containPoint(left, down1) || current->containPoint(right, down1)) bad = true;
        }
        if (bad)
        {
          square_t square = m_field.getBoundSquare();
          m_x = wereX;
          m_y = wereY;
          m_image.setPosition(square.left + wereX * m_field.getCellLength(), 
              square.top + wereY * m_field.getCellLength());
          return;
        }
        current++;
      }
    }
  };
  
  class MyTank: public Tank
  {
  public:
    MyTank(us (&path)[FIELD_WIDTH][FIELD_WIDTH]);
    virtual void draw(sf::RenderWindow & view) override;
    virtual float getSpeed();
    void move(uc type, float deltaTime);
    
    template< class iterator1, class iterator2 >
    void move(uc type, float deltaTime, iterator1 beginTanks, iterator2 endTanks)
    {
      iterator1 current = beginTanks;
      float wereX = m_x;
      float wereY = m_y;
      move(type, deltaTime);
      float left = m_x - m_tankMidWidth;
      float right = m_x + m_tankMidWidth;
      float up = m_y - m_tankMidWidth;
      float down = m_y + m_tankMidWidth;
      float left1 = m_x - m_midWidthPlusEps;
      float right1 = m_x + m_midWidthPlusEps;
      float up1 = m_y - m_midWidthPlusEps;
      float down1 = m_y + m_midWidthPlusEps;
      while ((current != endTanks))
      {
        if (current->getId() == m_id) 
        {
          current++;
          continue;
        }
        bool bad = false;
        switch (m_rotation)
        {
          case BIT_LEFTKEY:
          if (current->containPoint(left1, up) || current->containPoint(left1, down)) bad = true;
          break;
          case BIT_RIGHTKEY:
          if (current->containPoint(right1, up) || current->containPoint(right1, down)) bad = true;
          break;
          case BIT_UPKEY:
          if (current->containPoint(left, up1) || current->containPoint(right, up1)) bad = true;
          break;
          case BIT_DOWNKEY:
          if (current->containPoint(left, down1) || current->containPoint(right, down1)) bad = true;
        }
        if (bad)
        {
          square_t square = m_field.getBoundSquare();
          m_x = wereX;
          m_y = wereY;
          m_image.setPosition(square.left + wereX * m_field.getCellLength(), 
              square.top + wereY * m_field.getCellLength());
          return;
        }
        current++;
      }
    }
  };
}
#endif // TANK_HPP
