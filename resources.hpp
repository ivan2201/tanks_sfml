#ifndef RESOURCES_HPP
#define RESOURCES_HPP
#include <map>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "definition.hpp"

static constexpr ull TTF_FILE_EXP = ('t' * 26 + 't') * 26 + 'f';
static constexpr ull PNG_FILE_EXP = ('p' * 26 + 'n') * 26 + 'g';
static constexpr ull JPG_FILE_EXP = ('j' * 26 + 'p') * 26 + 'g';
static constexpr ull WAV_FILE_EXP = ('w' * 26 + 'a') * 26 + 'v';

class Resources
{
  private:
  Resources(const std::string & fileResourcePath);
  Resources(char * fileResourcePath);
  Resources(Resources &);
  Resources & operator =(const Resources &) = default;
  std::map < std::string, std::string > resources;
public:
  static Resources & getResources()
  {
    static Resources resources(resources_file_path);
    return resources;
  }
  
  sf::Texture getTexture(const std::string & name);
  sf::Font getFont(const std::string & fontName);
  sf::SoundBuffer getSoundBuffer(const std::string & musicName);
};

#endif // RESOURCES_HPP
