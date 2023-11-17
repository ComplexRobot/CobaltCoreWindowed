#include "Includes.h"

namespace WindowedHook
{
  HWND windowHandle = nullptr;
  void* sdlWindow = nullptr;
  bool loaded = false;
  bool letterbox = false;
  int internalWidth = 0;
  int internalHeight = 0;
  int currentFramebuffer = 0;
  int drawX = 0;
  int drawY = 0;
  int drawWidth = 0;
  int drawHeight = 0;

  ApiHookDefinition(HWND, CreateWindowExW, DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu,
    HINSTANCE hInstance, LPVOID lpParam)
  {
    if (lpWindowName != nullptr && std::wcscmp(lpWindowName, L"") == 0)
    {
      WNDCLASSEXW classInfo; ZeroMemory(&classInfo, sizeof(classInfo));
      if (GetClassInfoExW(hInstance, lpClassName, &classInfo) == FALSE)
        log.PopUp(PROJECT_TITLE " Fatal Error") << "Fatal Error: Failed to get window class info.";
      else
      {
        WindowProc = classInfo.lpfnWndProc;
        MH_STATUS mhStatus = MH_CreateHook(WindowProc, WindowProcDetour, (void**) &WindowProcOriginal);
        LogCreateHook(WindowProc);
        mhStatus = MH_EnableHook(WindowProc);
        log.If(mhStatus != MH_OK).PopUp(PROJECT_TITLE " Fatal Error") << "Fatal Error: MinHook failed to enable WindowProc hook: " << MH_StatusToString(mhStatus) << std::endl;
      }
      return windowHandle = CreateWindowExWOriginal(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    }
    return CreateWindowExWOriginal(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
  }

  ApiHookDefinition(LRESULT, WindowProc, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    switch (uMsg)
    {
    case WM_SIZE:
      if (!loaded)
        break;
      if (wParam == SIZE_RESTORED)
      {
        RECT rect = { 0, 0, 0, 0 };
        if (GetWindowRect(hwnd, &rect) == TRUE)
        {
          config.WindowWidth << (rect.right - rect.left);
          config.WindowHeight << (rect.bottom - rect.top);
        }
        config.WindowMaximized << false;
      }
      else if (wParam == SIZE_MAXIMIZED)
      {
        config.WindowMaximized << true;
      }
      break;
    case WM_MOVE:
      {
        if (!loaded)
          break;
        WINDOWPLACEMENT windowPlacement; ZeroMemory(&windowPlacement, sizeof(windowPlacement)); windowPlacement.length = sizeof(windowPlacement);
        if (GetWindowPlacement(hwnd, &windowPlacement) == TRUE && windowPlacement.showCmd != SW_MAXIMIZE && windowPlacement.showCmd != SW_SHOWMINIMIZED && windowPlacement.showCmd != SW_MINIMIZE
          && windowPlacement.showCmd != SW_FORCEMINIMIZE)
        {
          RECT rect = { 0, 0, 0, 0 };
          if (GetWindowRect(hwnd, &rect) == TRUE)
          {
            config.WindowX << rect.left;
            config.WindowY << rect.top;
          }
        }
      }
      break;
    case WM_PAINT:
      if (SDL_GetWindowFlags(sdlWindow) & 1/*SDL_WINDOW_FULLSCREEN*/)
      {
        RECT rect = { 0, 0, 0, 0 };
        if (GetWindowRect(hwnd, &rect) == TRUE)
        {
          internalWidth = rect.right - rect.left;
          internalHeight = rect.bottom - rect.top;
        }
        SDL_SetWindowFullscreen(sdlWindow, 0);
        SDL_SetWindowResizable(sdlWindow, 1);
        int x;
        int y;
        int width;
        int height;
        bool maximized;
        config.WindowX >> x;
        config.WindowY >> y;
        config.WindowWidth >> width;
        config.WindowHeight >> height;
        config.WindowMaximized >> maximized;
        config.Letterboxing >> letterbox;
        if (SetWindowPos(hwnd, HWND_TOP, x, y, width, height, 0) == FALSE)
          log << "Error: Failed to set window position." << std::endl;
        if (maximized)
          SDL_MaximizeWindow(sdlWindow);
        loaded = true;
      }
      break;
    case WM_KEYDOWN:
      if (wParam == VK_OEM_3)
        config.Letterboxing << (letterbox = !letterbox);
      break;
    }
    return WindowProcOriginal(hwnd, uMsg, wParam, lParam);
  }

  ApiHookDefinition(void, SDL_SetWindowSize, void* window, int w, int h)
  {
    sdlWindow = window;
    internalWidth = w;
    internalHeight = h;
    SDL_SetWindowSizeOriginal(window, w, h);
  }

  ApiHookDefinition(void, glViewport, int x, int y, int width, int height)
  {
    RECT rect = { 0, 0, 0, 0 };
    if (GetClientRect(windowHandle, &rect) == TRUE)
    {
      int clientWidth = rect.right - rect.left;
      int clientHeight = rect.bottom - rect.top;
      if (++currentFramebuffer % 2 == 0)
      {
        float aspectRatio = float(internalWidth) / internalHeight;
        if ((letterbox && clientWidth <= aspectRatio * clientHeight) || (!letterbox && clientWidth >= aspectRatio * clientHeight))
        {
          width = clientWidth;
          height = std::ceil(clientWidth / aspectRatio);
          y = std::lround(0.5f * (clientHeight - height));
        }
        else
        {
          width = std::ceil(clientHeight * aspectRatio);
          height = clientHeight;
          x = std::lround(0.5f * (clientWidth - width));
        }
        drawX = x;
        drawY = y;
        drawWidth = width;
        drawHeight = height;
      }
      else
      {
        width = clientWidth;
        height = clientHeight;
      }
    }
    glViewportOriginal(x, y, width, height);
  }

  ApiHookDefinition(BOOL, GetCursorPos, LPPOINT lpPoint)
  {
    BOOL result;
    if ((result = GetCursorPosOriginal(lpPoint)) == TRUE)
    {
      POINT point = { 0, 0 };
      if (ClientToScreen(windowHandle, &point) == TRUE)
      {
        lpPoint->x = std::lround((lpPoint->x - point.x - drawX) * (float(internalWidth) / drawWidth) + point.x);
        lpPoint->y = std::lround((lpPoint->y - point.y - drawY) * (float(internalHeight) / drawHeight) + point.y);
      }
    }
    return result;
  }

}