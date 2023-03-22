#ifndef EVENTLISTENER_HPP
#define EVENTLISTENER_HPP
#include <SFML/Window.hpp>
#include <list>
#include <tuple>
#define WPARAM unsigned short
#define LPARAM unsigned short
#define UINT unsigned int

class EventListener
{
public:
  EventListener();

  EventListener(const EventListener &) = delete;
  EventListener(EventListener &&) = delete;
  EventListener & operator =(const EventListener &) = delete;
  EventListener & operator =(EventListener &&) = delete;

  ~EventListener();

  void addListener(sf::Window view, void (*onClick)());
  void removeListeners(void (*onClick)());

  void onEvent(UINT message, LPARAM lParam);

private:
  struct node_t
  {
    sf::Window view;
    void (*onClick)();
  };
  std::list< node_t > listeners;
};

#endif // EVENTLISTENER_HPP
