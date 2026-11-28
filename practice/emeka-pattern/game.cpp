#include "game.hpp"
#include <cstdio>
#include <vector>

using namespace blit;

// Pack 8-bit r, g, b into one RGB565 value: RRRRRGGGGGGBBBBB
uint16_t pack_rgb565(uint8_t r, uint8_t g, uint8_t b) {
  uint16_t r5 = r >> 3;
  uint16_t g6 = g >> 2;
  uint16_t b5 = b >> 3;
  return (r5 << 11) | (g6 << 5) | b5;
}

// Exchange the high byte and the low byte of a 16-bit value
uint16_t swap_bytes(uint16_t v) {
  return ((v & 0xFF) << 8) | (v >> 8);
}

// The test pattern as byte-swapped RGB565 pixels, one entry per pixel
std::vector<uint16_t> frame;

void build_frame(int w, int h) {
  frame.resize(w * h);
  int half_w = w / 2;
  int half_h = h / 2;

  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      uint8_t r = 0, g = 0, b = 0;

      if (y < half_h && x < half_w) {
        r = 255; // top left: red
      } else if (y < half_h) {
        g = 255; // top right: green
      } else if (x >= half_w) {
        b = 255; // bottom right: blue
      } else {
        uint8_t v = x * 255 / (half_w - 1);
        r = g = b = v; // bottom left: grey fade
      }

      frame[y * w + x] = swap_bytes(pack_rgb565(r, g, b));
    }
  }
}

void init() {
  set_screen_mode(ScreenMode::hires);

  build_frame(screen.bounds.w, screen.bounds.h);
  printf("frame %dx%d = %d bytes\n", screen.bounds.w, screen.bounds.h,
         (int)(frame.size() * sizeof(uint16_t)));
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

  // top right: greeen
  screen.pen = Pen(0, 255, 0);
  screen.rectangle(Rect(half_w, 0, w - half_w, half_h));

  // bottom right: blue
  screen.pen = Pen(0, 0, 255);
  screen.rectangle(Rect(half_w, half_h, w - half_w, h - half_h));

  // bottom left: grey fade, one 1 pixel wide column per x
  for (int x = 0; x < half_w; x++) {
    uint8_t v = x * 255 / (half_w - 1);
    screen.pen = Pen(v, v, v);
    screen.rectangle(Rect(x, half_h, 1, h - half_h));
  }
}