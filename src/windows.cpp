#include "windows.h"

Window::Window(HINSTANCE hInstance, LPCTSTR windowTitle, int width, int height)
  : m_hInstance(hInstance), m_windowTitle(windowTitle), m_width(width), m_height(height) {
  WNDCLASS wc = {};
  wc.lpfnWndProc = Window::WindowProc;
  wc.hInstance = m_hInstance;
  wc.lpszClassName = m_windowTitle;

  RegisterClass(&wc);

  m_hwnd = CreateWindow(
    m_windowTitle,
    m_windowTitle,
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    m_width, m_height,
    nullptr,
    nullptr,
    m_hInstance,
    this
  );

  if (m_hwnd == NULL) {
    throw "Failed to create window";
  }
}

Window::~Window() {
  DestroyWindow(m_hwnd);
}

void Window::Show(int nCmdShow) {
  ShowWindow(m_hwnd, nCmdShow);
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
  case WM_SIZE:
    // Handle resizing here
    // You can use GetClientRect or GetWindowRect to get the new size
    // and then adjust the window content accordingly
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;

  case WM_PAINT:
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
    EndPaint(hwnd, &ps);
    return 0;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
