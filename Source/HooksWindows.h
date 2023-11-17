#define ApiHookDeclaration(RETURN, NAME, ...) extern void* NAME;\
  extern RETURN (__stdcall* NAME##Original)(__VA_ARGS__);\
  RETURN __stdcall NAME##Detour(__VA_ARGS__)

#define ApiHookDefinition(RETURN, NAME, ...) void* NAME = nullptr;\
  RETURN (__stdcall* NAME##Original)(__VA_ARGS__) = nullptr;\
  RETURN __stdcall NAME##Detour(__VA_ARGS__)

namespace WindowedHook
{
  ApiHookDeclaration(HWND, CreateWindowExW, DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu,
    HINSTANCE hInstance, LPVOID lpParam);
  ApiHookDeclaration(LRESULT, WindowProc, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  ApiHookDeclaration(void, SDL_SetWindowSize, void* window, int w, int h);
  ApiHookDeclaration(void, glViewport, int x, int y, int width, int height);
  ApiHookDeclaration(BOOL, GetCursorPos, LPPOINT lpPoint);
}