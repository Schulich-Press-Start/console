#include "game.hpp"

using namespace blit;

void init() {
  set_screen_mode(ScreenMode::hires);
}

void update(uint32_t time) {
}

void render(uint32_t time) {
  screen.pen = Pen(0, 0, 0);
  screen.clear();

  int w = screen.bounds.w;
  int h = screen.bounds.h;
  int half_w = w / 2;
  int half_h = h / 2;

  // top left: red
  screen.pen = Pen(255, 0, 0);
  screen.rectangle(Rect(0, 0, half_w, half_h));

  // top right: green
  screen.pen = Pen(0, 255, 0);
  screen.rectangle(Rect(half_w, 0, w - half_w, half_h));

  // bottom right: blue
  screen.pen = Pen(0, 0, 255);
  screen.rectangle(Rect(half_w, half_h, w - half_w, h - half_h));

  // bottom-left: grey fade, one 1-pixel-wide column per x
  for (int x = 0; x < half_w; x++) {
    uint8_t v = x * 255 / (half_w - 1);
    screen.pen = Pen(v, v, v);
    screen.rectangle(Rect(x, half_h, 1, h - half_h));
  }
}