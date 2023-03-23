#ifndef WINDOWMUTEX_HPP
#define WINDOWMUTEX_HPP
#include <mutex>

class WindowMutex
{
  class StaticMutex
  {
    private:
    StaticMutex() {};
    StaticMutex(const WindowMutex &) = delete;
    StaticMutex(WindowMutex &&) = delete;
    StaticMutex & operator =(const WindowMutex &) = delete;
    StaticMutex & operator =(WindowMutex &&) = delete;
    ~StaticMutex() = default;
    
    std::mutex m_mutex;
    public:
    static StaticMutex & getInstance();
    
    std::mutex & getMutex();
  };
  private:
  WindowMutex(const WindowMutex &) = delete;
  WindowMutex(WindowMutex &&) = delete;
  WindowMutex & operator =(const WindowMutex &) = delete;
  WindowMutex & operator =(WindowMutex &&) = delete;
  
  StaticMutex & m_mutex;
  std::lock_guard<std::mutex> m_lock;
  public:
  WindowMutex();
  
  ~WindowMutex() = default;
  
};

#endif
