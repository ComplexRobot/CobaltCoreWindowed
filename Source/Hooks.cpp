#include "Includes.h"

namespace WindowedHook
{
  void(__stdcall* SDL_SetWindowResizable)(void* window, int resizable) = nullptr;
  int(__stdcall* SDL_SetWindowFullscreen)(void* window, uint32_t flags) = nullptr;
  uint32_t(__stdcall* SDL_GetWindowFlags)(void* window) = nullptr;
  void(__stdcall* SDL_MaximizeWindow)(void* window) = nullptr;
  
  bool CreateHooks()
  {
    log << "Waiting for main executable to load DLLs..." << std::endl;
    void* user32 = GetModuleHandle("user32.dll");
    void* SDL2 = GetModuleHandle("SDL2.dll");
    void* opengl32 = GetModuleHandle("opengl32.dll");
    while (user32 == nullptr || SDL2 == nullptr || opengl32 == nullptr)
    {
      std::this_thread::yield();
      if (user32 == nullptr)
        user32 = GetModuleHandle("user32.dll");
      if (SDL2 == nullptr)
        SDL2 = GetModuleHandle("SDL2.dll");
      if (opengl32 == nullptr)
        opengl32 = GetModuleHandle("opengl32.dll");
    }
    log << "All DLLs successfully loaded." << std::endl;
    CreateApiHook(user32, CreateWindowExW);
    CreateApiHook(user32, GetCursorPos);
    CreateApiHook(SDL2, SDL_SetWindowSize);
    CreateApiHook(opengl32, glViewport);
    MH_STATUS mhStatus = MH_EnableHook(MH_ALL_HOOKS);
    log.If(mhStatus != MH_OK).PopUp(PROJECT_TITLE " Fatal Error") << "Fatal Error: MinHook failed to enable all hooks: " << MH_StatusToString(mhStatus) << std::endl;
    SetDllFunction(SDL2, SDL_SetWindowResizable);
    SetDllFunction(SDL2, SDL_SetWindowFullscreen);
    SetDllFunction(SDL2, SDL_GetWindowFlags);
    SetDllFunction(SDL2, SDL_MaximizeWindow);
    log << "Initial hook creation complete." << std::endl;
    return true;
  }

}