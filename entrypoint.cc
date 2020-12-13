//bin/echo; [ $(uname) = "Darwin" ] && FLAGS="-framework Webkit" || FLAGS="$(pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0)" ; c++ "$0" $FLAGS -std=c++11 -g -o webview && ./webview ; exit
// +build ignore


#include "Application.hpp"

#ifdef _WIN32
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char* arrgv[])
#endif
{

  Application app;
  app.Run();

  return 0;
}
