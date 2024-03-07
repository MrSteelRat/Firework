#ifndef WINDOWS_H
#define WINDOWS_H

#include <windows.h>

class Window {
public:
  Window(HINSTANCE hInstance, LPCTSTR windowTitle, int width, int height);
  ~Window();

  void Show(int nCmdShow);
  HWND GetHandle() const { return m_hwnd; }

private:
  static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  HWND m_hwnd;
  HINSTANCE m_hInstance;
  LPCTSTR m_windowTitle;
  int m_width;
  int m_height;
};

#endif // WINDOWS_H
