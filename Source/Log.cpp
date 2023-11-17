#include "Includes.h"

#define LOG_FILENAME PROJECT_TITLE ".log"
namespace WindowedHook
{
  wchar_t LOG_FILE_PATH[MAX_PATH] = {0};
  Logger logger;

  void InitLog()
  {
    if (GetModuleFileNameW(NULL, LOG_FILE_PATH, MAX_PATH) == 0)
    {
      MessageBox(0, "Failed to get file path for log.", PROJECT_TITLE " Error", MB_ICONWARNING);
      return;
    }
    PathRemoveFileSpecW(LOG_FILE_PATH);
    std::wcscat(LOG_FILE_PATH, L"\\" LOG_FILENAME);
    std::ofstream logFile(LOG_FILE_PATH, std::ofstream::trunc);
    if (!logFile.is_open())
    {
      MessageBox(0, "Failed to open log file.", PROJECT_TITLE " Error", MB_ICONWARNING);
    }
  }

  Logger& Logger::Start()
  {
    semaphore.acquire();
    ignoreInput = false;
    popUpTitle.clear();
    return *this;
  }

  Logger& Logger::PopUp(const std::string& title, UINT popUpFlags)
  {
    if (!ignoreInput)
    {
      popUpTitle = title;
      messageBoxFlags = popUpFlags;
    }
    return *this;
  }

  Logger& Logger::If(bool condition)
  {
    ignoreInput = !condition;
    ifResult = condition;
    return *this;
  }

  Logger& Logger::ElseIf(bool condition)
  {
    if (!ifResult)
    {
      ignoreInput = !condition;
      ifResult = condition;
    }
    else
    {
      ignoreInput = true;
    }
    return *this;
  }

  Logger& Logger::Else()
  {
    ignoreInput = ifResult;
    return *this;
  }
  
  int Logger::operator<<=(Logger&)
  {
    int buttonPressed = 0;
    if (!ignoreInput)
    {
      OutputDebugString(sstream.str().c_str());
      std::ofstream logFile(LOG_FILE_PATH, std::ofstream::app);
      if (logFile.is_open())
        logFile << sstream.str();
      if (!popUpTitle.empty())
      {
        std::string str{ sstream.str() };
        std::string title{ popUpTitle };
        UINT flags = messageBoxFlags;
        sstream.str(std::string{});
        semaphore.release();
        buttonPressed = MessageBox(0, str.c_str(), title.c_str(), flags);
      }
      else
      {
        sstream.str(std::string{});
        semaphore.release();
      }
    }
    else
      semaphore.release();
    return buttonPressed;
  }

  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> wideCharConvert;
  template<>
  Logger& Logger::operator<<(const wchar_t& rhs)
  {
    if (!ignoreInput)
      sstream << wideCharConvert.to_bytes(rhs);
    return *this;
  }

  template<>
  Logger& Logger::operator<<(const wchar_t* rhs)
  {
    if (!ignoreInput)
      sstream << wideCharConvert.to_bytes(rhs);
    return *this;
  }

  Logger& Logger::operator<<(std::ostream& (*rhs)(std::ostream&))
  {
    if (!ignoreInput)
      sstream << rhs;
    return *this;
  }
}