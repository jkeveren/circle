#include <iostream>
#include <sys/ioctl.h>
#include <signal.h>
#include <math.h>

using std::cout;

void render(int) {
  struct winsize ws;
  ioctl(0, TIOCGWINSZ, &ws);

  // Clear terminal and set forground to white.
  cout << "\033[0m\033[2J\033[47m";

  // Roughly calculate number of points required for a full circle.
  int pointCount = ws.ws_row * 2 + ws.ws_col * 2;

  // Center the circle and account for the top-leftedness of the character coordinates.
  float centerX = (float)(ws.ws_row - 1) / 2 + 1;
  float centerY = (float)(ws.ws_col - 1) / 2 + 1;

  // Draw the dang circle.
  for (int i = 0; i < pointCount; i++) {
    float portion = (float)i / pointCount;
    int x = round(centerX + sin(portion * M_PI * 2) * ws.ws_row * 0.4);
    int y = round(centerY + cos(portion * M_PI * 2) * ws.ws_col * 0.4);
    cout << "\033[" << x << ";" << y << "f ";
  }

  // Reset style in case of crash and move cursor to 0,0 for neatness.
  cout << "\033[0m\033[H" << std::flush;
}

void cleanExit(int) {
  // Reset style, just to be sure, and clear the terminal.
  cout << "\033[0m\033[2J\n";
  exit(0);
}

int main() {
  // Render now and on window change.
  render(0);
  signal(SIGWINCH, render);

  // Exit cleanly when asked to.
  signal(SIGINT, cleanExit);

  // Don't exit. Wait for signnals.
  std::cin.ignore();
  return 0;
}
