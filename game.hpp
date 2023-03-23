#ifndef GAME_HPP
#define GAME_HPP
#include <SFML/Graphics.hpp>
#include "SFML/Audio.hpp"
#include <list>
#include <memory>
#include <array>
#include <queue>
#include <math.h>
#include <iostream>
#include "resources.hpp"
#include "mymath.hpp"
#include "eventcontroller.hpp"
#include "definition.hpp"
#include "tank.hpp"
#include "bullet.hpp"
#include "windowmutex.hpp"
#include "fps.hpp"

class Game
{
public:
  ~Game();

  void start(sf::RenderWindow & window);
  
  sf::Vector2u getWindowRect();
  void setWindowRect(sf::Vector2u windowRect);
  
  friend void resizeGame(const sf::Vector2u rect);
  
  static Game & getGame()
  {
    static Game game;
    return game;
  }

private:
  Game();
  void generateField(const ull & seed);
  void updatePath();
  void updatePath(us (&path)[FIELD_WIDTH][FIELD_WIDTH], point_t begin);
  void redrawField(sf::RenderWindow & window);

  sf::Vector2u m_windowRect;
  Resources & m_resources;
  Field & m_field;
  us m_path[FIELD_WIDTH][FIELD_WIDTH];
  baho::MyTank m_myTank;
  sf::Texture m_textures[10];
  std::list< baho::Enemy > m_enemys;
  std::list< Bullet > m_bullets;
  std::array<point_t, ENEMY_SPAWNER_NUMBER> m_spawners;
  std::array<float, BEGIN_ENEMY_NUMBER> m_lastShotTime;
  Bullet m_preBullet;
  baho::Enemy m_preEnemy;
  sf::SoundBuffer m_buffer[3];
  sf::Sound m_tankBang;
  sf::Sound m_tankShot;
  sf::Sound m_tankMoving;
};

#endif
