namespace WindowedHook
{
#define CONFIG_KEY(KEY) Key KEY{L#KEY}
#define CONFIG_APPNAME "Settings"
#define CONFIG_BUFFER_SIZE 512
  class Config;
  extern Config config;
  class Config
  {
    std::wstring filePath;
    friend void InitConfig();
  public:
    class Key
    {
      const std::wstring name;
      Key(const std::wstring& name) : name(name) { }
    public:
      template <typename T>
      void operator<<(const T& rhs)
      {
        std::wstringstream strstr;
        strstr << rhs;
        if (WritePrivateProfileStringW(L"" CONFIG_APPNAME, name.c_str(), strstr.str().c_str(), config.filePath.c_str()) == FALSE)
          log << "Error: Failed to write to ini file." << std::endl;
      }

      template <typename T>
      void operator<<(const T* rhs)
      {
        std::wstringstream strstr;
        strstr << rhs;
        if (WritePrivateProfileStringW(L"" CONFIG_APPNAME, name.c_str(), strstr.str().c_str(), config.filePath.c_str()) == FALSE)
          log << "Error: Failed to write to ini file." << std::endl;
      }

      template <typename T>
      void operator>>(T& rhs) const
      {
        wchar_t buffer[CONFIG_BUFFER_SIZE];
        DWORD charsWritten = GetPrivateProfileStringW(L"" CONFIG_APPNAME, name.c_str(), L"", buffer, CONFIG_BUFFER_SIZE, config.filePath.c_str());
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
          log.PopUp(PROJECT_TITLE " Fatal Error") << "Fatal Error: Failed to open config file." << std::endl;
          return;
        }
        if (charsWritten == 0)
          return;
        std::wstringstream strstr;
        strstr.str(buffer);
        strstr >> rhs;
      }

      template <typename T>
      void operator>>(T* rhs) const
      {
        wchar_t buffer[CONFIG_BUFFER_SIZE];
        DWORD charsWritten = GetPrivateProfileStringW(L"" CONFIG_APPNAME, name.c_str(), L"", buffer, CONFIG_BUFFER_SIZE, config.filePath.c_str());
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
          log.PopUp(PROJECT_TITLE " Fatal Error") << "Fatal Error: Failed to open config file." << std::endl;
          return;
        }
        if (charsWritten == 0)
          return;
        std::wstringstream strstr;
        strstr.str(buffer);
        strstr >> rhs;
      }
      friend class Config;
    };
    CONFIG_KEY(WindowX);
    CONFIG_KEY(WindowY);
    CONFIG_KEY(WindowWidth);
    CONFIG_KEY(WindowHeight);
    CONFIG_KEY(WindowMaximized);
    CONFIG_KEY(Letterboxing);
  };

  void InitConfig();
}