namespace WindowedHook
{
  extern void(__stdcall* SDL_SetWindowResizable)(void* window, int resizable);
  extern int(__stdcall* SDL_SetWindowFullscreen)(void* window, uint32_t flags);
  extern uint32_t(__stdcall* SDL_GetWindowFlags)(void* window);
  extern void(__stdcall* SDL_MaximizeWindow)(void* window);

  bool CreateHooks();
}

#define CreateApiHook(MODULE, FUNCTION) FUNCTION = GetProcAddress((HMODULE) MODULE, #FUNCTION);\
  if (FUNCTION == nullptr)\
    log << "*** Hook Failed: " #MODULE ".dll::" #FUNCTION " failed to get address." << std::endl;\
  else\
  {\
    MH_STATUS FUNCTION##Status = MH_CreateHook(FUNCTION, FUNCTION##Detour, (void**) &FUNCTION##Original);\
    log.If(FUNCTION##Status == MH_OK) << "*** Hook Created: " #MODULE ".dll::" #FUNCTION << std::endl;\
    log.Else() << "*** Hook Failed: " #MODULE ".dll::" #FUNCTION " with error: " << MH_StatusToString(FUNCTION##Status) << std::endl;\
  }

#define LogCreateHook(FUNCTION) log.If(mhStatus == MH_OK) << "*** Hook Created: " #FUNCTION << std::endl;\
log.Else() << "*** Hook Failed: " #FUNCTION " with error: " << MH_StatusToString(mhStatus) << std::endl;

#define SetDllFunction(MODULE, FUNCTION) FUNCTION = (decltype(FUNCTION)) GetProcAddress((HMODULE) MODULE, #FUNCTION);\
log.If(FUNCTION != nullptr) << "*** Function Set: " #MODULE ".dll::" #FUNCTION << std::endl;\
log.Else().PopUp(PROJECT_TITLE " Fatal Error") << "Fatal Error: Failed to get address of function " #MODULE ".dll::" #FUNCTION << std::endl;
