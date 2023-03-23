#ifndef DEFINITION_HPP
#define DEFINITION_HPP
#include "SFML/Graphics.hpp"

typedef unsigned long long ull;
typedef unsigned short us;
typedef unsigned char uc;

static constexpr const char * resources_file_path = "resources/resource.txt";

static constexpr uc FIELD_WIDTH = 25;

static constexpr uc ENEMY_SPAWNER_DELTA = 4;

//за вычетом крайних
static constexpr uc ENEMY_SPAWNER_NUMBER = FIELD_WIDTH / ENEMY_SPAWNER_DELTA - 1;

static constexpr int BEGIN_ENEMY_NUMBER = 10;

static constexpr uc BIT_UNDESTROYED = 0x04;
static constexpr uc BIT_LOCK = 0x01;
static constexpr uc BIT_UNSHOOTED = 0x02;
static constexpr uc BIT_ISSPAWN = 0x08;

static constexpr uc TYPE_BETON = BIT_LOCK | BIT_UNDESTROYED | BIT_UNSHOOTED;
static constexpr uc TYPE_BRICK = BIT_LOCK | BIT_UNSHOOTED;
static constexpr uc TYPE_GROUND = BIT_UNDESTROYED;
static constexpr uc TYPE_HADGEHOG = BIT_LOCK | BIT_UNDESTROYED;
static constexpr uc TYPE_SPAWN = BIT_ISSPAWN | BIT_UNDESTROYED;

static constexpr uc BIT_LEFTKEY = 0x01;
static constexpr uc BIT_UPKEY = 0x02;
static constexpr uc BIT_RIGHTKEY = 0x04;
static constexpr uc BIT_DOWNKEY = 0x08;

static constexpr us FRAMERATE_LIMIT = 60;


struct square_t
{
  us left;
  us top;
  us length;
};

struct point2f_t
{
  float x;
  float y;
};
struct point_t
{
  us x;
  us y;
};

struct cell_t
{
  us type;
  uc health;
  sf::Sprite image;
};

#endif // DEFINITION_HPP
