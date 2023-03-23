#include "resources.hpp"

ull hash(std::string str)
{
  ull hash = 0;
  for (ull i = 0, curPower = 1; i < str.length(); i++)
  {
    hash += str.at(i) * curPower;
    curPower *= 26;
  }
  return  hash;
}

Resources::Resources(const std::string & fileResourcePath)
{
  std::ifstream fileWithResource;
  fileWithResource.open(fileResourcePath);
  if (!fileWithResource.is_open())
  {
    throw std::invalid_argument("file not found");
  }
  while (!fileWithResource.eof())
  {
    std::string name, path;
    fileWithResource >> name >> path;
    resources.insert({name, path});
  }
}

Resources::Resources(char * fileResourcePath):
  Resources(std::string(fileResourcePath))
{
}

sf::Texture Resources::getTexture(const std::string & name)
{
  auto iter = resources.find(name);
  if (iter == resources.cend())
  {
    throw std::invalid_argument("texture not found: " + name);
  }
  sf::Texture texture;
  if (!texture.loadFromFile(iter->second))
    throw std::invalid_argument("resource not found by path: " + iter->second);
  return texture;
}

sf::Font Resources::getFont(const std::string & fontName)
{
  auto iter = resources.find(fontName);
  if (iter == resources.cend())
  {
    throw std::invalid_argument("font not found: " + fontName);
  }
  sf::Font font;
  if (!font.loadFromFile(iter->second))
    throw std::invalid_argument("resource not found by path: " + iter->second);
  return font;
}

sf::SoundBuffer Resources::getSoundBuffer(const std::string & musicName)
{
  auto iter = resources.find(musicName);
  if (iter == resources.cend())
  {
    throw std::invalid_argument("font not found: " + musicName);
  }
  sf::SoundBuffer music;
  if (!(music.loadFromFile(iter->second)))
    throw std::invalid_argument("resource not found by path: " + iter->second);
  return music;
}
