namespace WindowedHook
{
  class Logger
  {
    std::stringstream sstream;
    std::binary_semaphore semaphore{1};
    bool ignoreInput = false;
    bool ifResult = true;
    std::string popUpTitle;
    UINT messageBoxFlags = 0;
  public:
    Logger& Start();
    Logger& PopUp(const std::string& title, UINT popUpFlags = MB_ICONERROR);
    Logger& If(bool condition);
    Logger& ElseIf(bool condition);
    Logger& Else();
    int operator<<=(Logger&);
    
    template<typename T, std::enable_if_t<!std::is_pointer_v<T> && !std::is_array_v<T>, bool> = true>
    Logger& operator<<(const T& rhs)
    {
      if (!ignoreInput)
        sstream << rhs;
      return *this;
    }

    template<typename T>
    Logger& operator<<(const T* rhs)
    {
      if (!ignoreInput)
        sstream << rhs;
      return *this;
    }
    
    template<> Logger& operator<<(const wchar_t& rhs);
    template<> Logger& operator<<(const wchar_t* rhs);
    Logger& operator<<(std::ostream& (*rhs)(std::ostream&));
  };

  extern Logger logger;
  void InitLog();
}

#define log logger <<= logger.Start()