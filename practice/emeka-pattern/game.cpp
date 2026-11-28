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

// Turn an RGB565 value back into a colour with 8 bits per channel
Pen unpack_rgb565(uint16_t p) {
  uint8_t r5 = (p >> 11) & 0x1F;
  uint8_t g6 = (p >> 5) & 0x3F;
  uint8_t b5 = p & 0x1F;
  // copy the top bits into the empty low bits so the maximum becomes 255
  return Pen((r5 << 3) | (r5 >> 2), (g6 << 2) | (g6 >> 4), (b5 << 3) | (b5 >> 2));
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

// Draw the stored frame pixel by pixel.
// fix_order = true undoes the byte swap first (correct colours);
// false decodes the stored value as it is (wrong colours).
void draw_frame(int w, int h, bool fix_order) {
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      uint16_t p = frame[y * w + x];
      if (fix_order) {
        p = swap_bytes(p);
      }
      screen.pen = unpack_rgb565(p);
      screen.pixel(Point(x, y));
    }
  }
}

// 0 = rectangles, 1 = frame with byte order fixed, 2 = frame with wrong byte order
int mode = 0;

void init() {
  set_screen_mode(ScreenMode::hires);

  build_frame(screen.bounds.w, screen.bounds.h);
  printf("frame %dx%d = %d bytes\n", screen.bounds.w, screen.bounds.h,
         (int)(frame.size() * sizeof(uint16_t)));
}

void update(uint32_t time) {
  if (buttons.pressed & Button::A) {
    mode = (mode + 1) % 3;
  }
}

void render(uint32_t time) {
  screen.pen = Pen(0, 0, 0);
  screen.clear();

  int w = screen.bounds.w;
  int h = screen.bounds.h;

  if (mode != 0) {
    draw_frame(w, h, mode == 1);
    return;
  }

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

  // bottom left: grey fade, one pixel wide column per x
  for (int x = 0; x < half_w; x++) {
    uint8_t v = x * 255 / (half_w - 1);
    screen.pen = Pen(v, v, v);
    screen.rectangle(Rect(x, half_h, 1, h - half_h));
  }
}