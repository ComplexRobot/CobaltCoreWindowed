#include "Includes.h"

namespace WindowedHook
{
#define XINPUT_CAPABILITIES void
#define XINPUT_STATE void
#define XINPUT_VIBRATION void
#define XINPUT_BATTERY_INFORMATION void
#define PXINPUT_KEYSTROKE void*

  DWORD(__stdcall* XInputGetStateOriginal)(DWORD dwUserIndex, XINPUT_STATE* pState) = nullptr;
  DWORD(__stdcall* XInputSetStateOriginal)(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration) = nullptr;
  DWORD(__stdcall* XInputGetCapabilitiesOriginal)(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities) = nullptr;
  void(__stdcall* XInputEnableOriginal)(BOOL enable) = nullptr;
  DWORD(__stdcall* XInputGetBatteryInformationOriginal)(DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION* pBatteryInformation) = nullptr;
  DWORD(__stdcall* XInputGetKeystrokeOriginal)(DWORD dwUserIndex, DWORD dwReserved, PXINPUT_KEYSTROKE pKeystroke) = nullptr;
  DWORD(__stdcall* XInputGetAudioDeviceIdsOriginal)(DWORD dwUserIndex, LPWSTR pRenderDeviceId, UINT* pRenderCount, LPWSTR pCaptureDeviceId, UINT* pCaptureCount) = nullptr;

  bool WrapDll()
  {
    log << "Wrapping original DLL..." << std::endl;
    wchar_t systemDllPath[MAX_PATH * 2] = { 0 };
    wchar_t* folderEnd = systemDllPath + GetSystemDirectoryW(systemDllPath, MAX_PATH * 2);
    if (folderEnd == systemDllPath)
    {
      log.PopUp(PROJECT_TITLE " Fatal Error") << "Fatal Error: Failed to get system directory." << std::endl;
      return false;
    }
    if (*(folderEnd - 1) != L'\\')
      *folderEnd++ = L'\\';
    if (GetModuleFileNameW((HMODULE) mainDllAddress, folderEnd, MAX_PATH * 2 - (folderEnd - systemDllPath)) == 0)
    {
      log.PopUp(PROJECT_TITLE " Fatal Error") << "Fatal Error: Failed to get file path of main DLL." << std::endl;
      return false;
    }
    PathStripPathW(folderEnd);
    log << "Attempting to wrap '" << systemDllPath << "'" << std::endl;
    void* originalDllAddress = LoadLibraryW(systemDllPath);
    if (originalDllAddress == nullptr)
    {
      log.PopUp(PROJECT_TITLE " Fatal Error") << "Fatal Error: Failed to load default DLL for wrap." << std::endl;
      return false;
    }
#define DLL_FUNCTION_WRAP(NAME) NAME##Original = (decltype(NAME##Original)) GetProcAddress((HMODULE) originalDllAddress, #NAME);\
      if (NAME##Original == nullptr)\
      {\
        log.PopUp(PROJECT_TITLE " Fatal Error") << "Fatal Error: Failed to get address of " #NAME ".";\
        return false;\
      }
    DLL_FUNCTION_WRAP(XInputGetState);
    DLL_FUNCTION_WRAP(XInputSetState);
    DLL_FUNCTION_WRAP(XInputGetCapabilities);
    DLL_FUNCTION_WRAP(XInputEnable);
    DLL_FUNCTION_WRAP(XInputGetBatteryInformation);
    DLL_FUNCTION_WRAP(XInputGetKeystroke);
    DLL_FUNCTION_WRAP(XInputGetAudioDeviceIds);
    log << "DLL wrapping complete." << std::endl;
    return true;
  }
}

extern "C" __declspec(dllexport) DWORD __stdcall XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState)
{
  return WindowedHook::XInputGetStateOriginal(dwUserIndex, pState);
}

extern "C" __declspec(dllexport) DWORD __stdcall XInputSetState(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
{
  return WindowedHook::XInputSetStateOriginal(dwUserIndex, pVibration);
}

extern "C" __declspec(dllexport) DWORD __stdcall XInputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities)
{
  return WindowedHook::XInputGetCapabilitiesOriginal(dwUserIndex, dwFlags, pCapabilities);
}

extern "C" __declspec(dllexport) void __stdcall XInputEnable(BOOL enable)
{
  WindowedHook::XInputEnableOriginal(enable);
}

extern "C" __declspec(dllexport) DWORD __stdcall XInputGetBatteryInformation(DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION* pBatteryInformation)
{
  return WindowedHook::XInputGetBatteryInformationOriginal(dwUserIndex, devType, pBatteryInformation);
}

extern "C" __declspec(dllexport) DWORD __stdcall XInputGetKeystroke(DWORD dwUserIndex, DWORD dwReserved, PXINPUT_KEYSTROKE pKeystroke)
{
  return WindowedHook::XInputGetKeystrokeOriginal(dwUserIndex, dwReserved, pKeystroke);
}

extern "C" __declspec(dllexport) DWORD __stdcall XInputGetAudioDeviceIds(DWORD dwUserIndex, LPWSTR pRenderDeviceId, UINT* pRenderCount, LPWSTR pCaptureDeviceId, UINT* pCaptureCount)
{
  return WindowedHook::XInputGetAudioDeviceIdsOriginal(dwUserIndex, pRenderDeviceId, pRenderCount, pCaptureDeviceId, pCaptureCount);
}