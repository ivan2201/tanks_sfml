#ifndef EVENTCONTROLLER_HPP
#define EVENTCONTROLLER_HPP
#include <thread>
#include <map>
#include <functional>
#include <list>
#include <SFML/Graphics.hpp>
#include "windowmutex.hpp"
#include "fps.hpp"

class EventController
{
private:
  class OnKeyListener
  {
  public:
    OnKeyListener();

    OnKeyListener(const OnKeyListener &) = delete;
    OnKeyListener(OnKeyListener &&) = delete;
    OnKeyListener & operator =(const OnKeyListener &) = delete;
    OnKeyListener & operator =(OnKeyListener &&) = delete;

    ~OnKeyListener();

    void addListener(sf::Keyboard::Key key, void (*onKeyDown)(), void (*onKeyUp)());
    void removeListeners(sf::Keyboard::Key key, void (*onKeyDown)());

    void onEvent(sf::Keyboard::Key key, bool pressed);

  private:
    struct node_t
    {
      std::list< void (*)() > onDownListeners;

      std::list< void (*)() > onUpListeners;
    };

    std::map< sf::Keyboard::Key, node_t > m_listeners;
  };

  EventController();
  EventController(const EventController &) = delete;
  EventController & operator =(const EventController &) = delete;
  
  ~EventController() = default;
  
  bool m_finish;
  sf::Window * m_window;
  std::thread m_eventControllerThread;
  OnKeyListener m_onKeyListener;
  void (*m_onLMouseClick)(int x, int y);
  void (*m_onChangeSizeListener)(unsigned int width, unsigned int height);
  void (*m_onClosedListener)();
  
  friend void checkEvents();
public:

  static EventController & getEventController()
  {
    static EventController instance;
    return instance;
  }
  
  void setWindow(sf::Window &);
  
  void finish();
  
  void addOnKeyListener(sf::Keyboard::Key key, void (*onKeyDown)(), void (*onKeyUp)());
  void removeOnKeyListener(sf::Keyboard::Key key, void (*onKeyDown)());

  void setOnChangeSizeListener(void (*onChangeSizeListener)(unsigned int width, unsigned int height));

  void setOnLMouseClickListener(void (*onClick)(int x, int y));
  
  void setOnClosedListener(void (*onClosed)());
  
};

#endif // EVENTCONTROLLER_HPP
