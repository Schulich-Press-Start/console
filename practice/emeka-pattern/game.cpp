#include "game.hpp"
#include <cstdio>
#include <vector>

using namespace blit;

// 8-bit r,g,b  ->  one 16-bit value laid out as RRRRRGGGGGGBBBBB
uint16_t pack_rgb565(uint8_t r, uint8_t g, uint8_t b) {
  uint16_t r5 = r >> 3;
  uint16_t g6 = g >> 2;
  uint16_t b5 = b >> 3;
  return (r5 << 11) | (g6 << 5) | b5;
}

// exchange the high byte and the low byte of a 16-bit value
uint16_t swap_bytes(uint16_t v) {
  return ((v & 0xFF) << 8) | (v >> 8);
}

void show(const char *name, uint8_t r, uint8_t g, uint8_t b) {
  uint16_t p = pack_rgb565(r, g, b);
  printf("%-6s packed %04X  swapped %04X\n", name, (unsigned)p, (unsigned)swap_bytes(p));
}

std::vector<uint16_t> frame;

// fill with the test pattern as byte-swapped RGB565
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
        r = g = b = v; // bottom left: grey faded
      }

      frame[y * w + x] = swap_bytes(pack_rgb565(r, g, b));
    }
  }
}

void sample(const char *name, int w, int x, int y) {
  printf("%-12s (%3d,%3d)  stored %04X\n", name, x, y, (unsigned)frame[y * w + x]);
}

void init() {
  set_screen_mode(ScreenMode::hires);

  int w = screen.bounds.w;
  int h = screen.bounds.h;
  build_frame(w, h);

  printf("frame %dx%d = %d pixels = %d bytes\n",
         w, h, (int)frame.size(), (int)(frame.size() * sizeof(uint16_t)));
  sample("top-left", w, 0, 0);
  sample("top-right", w, w - 1, 0);
  sample("bottom-right", w, w - 1, h - 1);
  sample("fade start", w, 0, h - 1);
  sample("fade middle", w, 80, h - 1);
  sample("fade end", w, 159, h - 1);
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

  // bottom left: grey fade, one one pixel-wide column per x
  for (int x = 0; x < half_w; x++) {
    uint8_t v = x * 255 / (half_w - 1);
    screen.pen = Pen(v, v, v);
    screen.rectangle(Rect(x, half_h, 1, h - half_h));
  }
}