#include "Includes.h"

#define CONFIG_FILENAME PROJECT_TITLE ".ini"
namespace WindowedHook
{
  wchar_t CONFIG_FILE_PATH[MAX_PATH] = {0};
  Config config;
  void InitConfig()
  {
    if (GetModuleFileNameW(NULL, CONFIG_FILE_PATH, MAX_PATH) == 0)
    {
      log.PopUp(PROJECT_TITLE " Fatal Error") << "Fatal Error: Failed to get file path for ini." << std::endl;
      return;
    }
    PathRemoveFileSpecW(CONFIG_FILE_PATH);
    std::wcscat(CONFIG_FILE_PATH, L"\\" CONFIG_FILENAME);
    config.filePath = CONFIG_FILE_PATH;
    std::ofstream configFile(CONFIG_FILE_PATH, std::ofstream::ate | std::ofstream::app);
    if (configFile.is_open())
    {
      if (configFile.tellp() == 0)
      {
        const char initialText[] =
          "[" CONFIG_APPNAME "]\n"
          "WindowX=100\n"
          "WindowY=100\n"
          "WindowWidth=800\n"
          "WindowHeight=480\n"
          "WindowMaximized=0\n"
          "Letterboxing=0";
        configFile.write(initialText, sizeof(initialText) - 1);
        log << "Default config file " CONFIG_FILENAME " created." << std::endl;
        log.PopUp(PROJECT_TITLE " First Run", MB_ICONINFORMATION)
          << "*** IMPORTANT INFORMATION ***" << std::endl
          << "Set the in-game setting to fullscreen." << std::endl
          << "Press the tilde key (`~) to toggle letterboxing." << std::endl;
      }
    }
    else
      log.PopUp(PROJECT_TITLE " Fatal Error") << "Fatal Error: Failed to open config file." << std::endl;
  }
}