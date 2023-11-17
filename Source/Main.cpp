#include "Includes.h"

using namespace WindowedHook;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  switch(fdwReason) 
  { 
  case DLL_PROCESS_ATTACH:
    mainDllAddress = hinstDLL;
    mainExeAddress = GetModuleHandle(NULL);
    return MainInitialize();
    break;
  case DLL_PROCESS_DETACH:
    MainExit();
    break;
  }
  return TRUE;
}

namespace WindowedHook
{
  void* mainDllAddress = nullptr;
  void* mainExeAddress = nullptr;
  BOOL MainInitialize()
  {
    InitLog();
    log << "Initializing " PROJECT_TITLE "..." << std::endl;
    InitConfig();
    if (!WrapDll())
      return FALSE;
#ifdef _DEBUG
    std::this_thread::sleep_for(std::chrono::seconds{5}); // Sleep in debug mode to give time to attach to the process for debug breaks
#endif
    MH_STATUS mhInitStatus = MH_Initialize();
    log.If(mhInitStatus == MH_OK) << "MinHook successfully initialized." << std::endl;
    log.Else().PopUp(PROJECT_TITLE " Fatal Error") << "Fatal Error: MinHook initialize failed: " << MH_StatusToString(mhInitStatus) << std::endl;
    std::thread{[] ()
    {
      CreateHooks();
      log << PROJECT_TITLE " initialization complete." << std::endl;
    }}.detach();
    return TRUE;
  }

  void MainExit()
  {
    log << "Exiting " PROJECT_TITLE "..." << std::endl;
    MH_STATUS mhStatus = MH_DisableHook(MH_ALL_HOOKS);
    log.If(mhStatus != MH_OK) << "MinHook disable all hooks failed: " << MH_StatusToString(mhStatus) << std::endl;
    mhStatus = MH_Uninitialize();
    log.If(mhStatus != MH_OK) << "MinHook uninitialize failed: " << MH_StatusToString(mhStatus) << std::endl;
    log << PROJECT_TITLE " exit complete." << std::endl;
  }
}