#include "game.hpp"

#include <algorithm>
#include <cmath>
#include <string>

using namespace blit;

// ---------------------------------------------------------------------------
// Constants
//
// standard game developement stuff: an update() func called at some fixed rate,
// and a draw() or render() func called as fast as possible. unlike unity where update() is called every frame
// update() is called every 10 ms (100 times a second) no matter how fast
// frames are drawn, so every speed below is in pixels PER UPDATE.
// my guess is so that the game logic is not tied to the framerate, which is good for consistency across different hardware, and no need for something like deltaTime.
// 1.0 means 100 pixels per second.
// ---------------------------------------------------------------------------
namespace {
  constexpr int SCREEN_WIDTH = 160;  // lores mode is 160x120, hires mode is 320x240
  constexpr int SCREEN_HEIGHT = 120; // lores mode is 160x120, hires mode is 320x240

  constexpr int PADDLE_WIDTH = 4;
  constexpr int PADDLE_HEIGHT = 22;
  constexpr int PADDLE_MARGIN = 4; // gap between paddle and edge of screen
  constexpr int BALL_SIZE = 4;

  constexpr float PLAYER_SPEED = 1.6f;
  constexpr float CPU_SPEED = 1.1f; // slower than the player, so the CPU is beatable, idk how else to do this simply
  constexpr float BALL_START_SPEED = 1.3f;
  constexpr float BALL_MAX_SPEED = 3.0f;
  constexpr float BALL_SPEEDUP = 1.06f;    // ball gets about 6% faster on every paddle hit
  constexpr float MAX_BOUNCE_ANGLE = 1.0f; // radians (about 57 degrees)
  constexpr int WIN_SCORE = 5;

  // ---------------------------------------------------------------------------
  // Game state
  // ---------------------------------------------------------------------------
  enum class State {
    Serving,
    Playing,
    GameOver
  };

  // im still not entirely sure what a struct actually is, but it's implementation seems simple enough.
  struct Paddle {
    float x;
    float y;

    Rect rect() const { return Rect(int(x), int(y), PADDLE_WIDTH, PADDLE_HEIGHT); }
  };
  struct Ball {
    Vec2 pos;
    Vec2 vel;

    Rect rect() const { return Rect(int(pos.x), int(pos.y), BALL_SIZE, BALL_SIZE); }
  };

  Paddle player{PADDLE_MARGIN, (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2.0f};
  Paddle cpu{SCREEN_WIDTH - PADDLE_MARGIN - PADDLE_WIDTH, (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2.0f};
  Ball ball;
  int player_score = 0;
  int cpu_score = 0;
  int serve_dir = 1;          // +1 serves toward the CPU, -1 toward the player
  int cpu_reaction_time = 50; // delay the cpu's reaction by this amount in ms
  int delay_count = 10;       // counter to track how long since the cpu last reacted
  float cpu_target = ball.pos.y + BALL_SIZE / 2.0f - PADDLE_HEIGHT / 2.0f;
  State state = State::Serving;

  // ---------------------------------------------------------------------------
  // Helpers
  // ---------------------------------------------------------------------------
  float clamp_paddle_y(float y) {
    return std::clamp(y, 0.0f, float(SCREEN_HEIGHT - PADDLE_HEIGHT));
  }

  // Put the ball in the middle of the screen.
  void reset_ball() {
    ball.pos = Vec2((SCREEN_WIDTH - BALL_SIZE) / 2.0f, (SCREEN_HEIGHT - BALL_SIZE) / 2.0f);
    ball.vel = Vec2(0.0f, 0.0f); // stationary until served
  }

  void launch_ball() {
    // blit::random() must be qualified: plain random() also exists in the C
    // library, and the compiler can't tell which one you mean.
    float vy = (int(blit::random() % 161) - 80) / 100.0f; // -0.8 .. +0.8
    ball.vel = Vec2(BALL_START_SPEED * serve_dir, vy);
  }

  void new_game() {
    player_score = 0;
    cpu_score = 0;
    serve_dir = 1;
    player.y = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2.0f;
    cpu.y = player.y;
    reset_ball();
    state = State::Serving;
  }

  void score_point(bool player_scored) {
    if (player_scored) {
      player_score++;
      serve_dir = 1; // next serve goes toward whoever just lost the point
    } else {
      cpu_score++;
      serve_dir = -1;
    }
    reset_ball();
    state = (player_score >= WIN_SCORE || cpu_score >= WIN_SCORE) ? State::GameOver : State::Serving;
  }

  // Reflect the ball off a paddle. Where it hits decides the new angle:
  // the middle of the paddle sends it straight back, the edges send it steeply. nothing fancy here.
  void bounce_off(const Paddle &p) {
    float hit = ((ball.pos.y + BALL_SIZE / 2.0f) - (p.y + PADDLE_HEIGHT / 2.0f)) / (PADDLE_HEIGHT / 2.0f);
    hit = std::clamp(hit, -1.0f, 1.0f);

    float speed = std::min(std::hypot(ball.vel.x, ball.vel.y) * BALL_SPEEDUP, BALL_MAX_SPEED);
    float angle = hit * MAX_BOUNCE_ANGLE;
    float dir = (p.x < SCREEN_WIDTH / 2) ? 1.0f : -1.0f; // away from the paddle we hit

    ball.vel = Vec2(dir * speed * std::cos(angle), speed * std::sin(angle));

    // Move the ball out of the paddle so it can't be hit twice in a row.
    ball.pos.x = (dir > 0.0f) ? p.x + PADDLE_WIDTH : p.x - BALL_SIZE;
  }
} // namespace

// ---------------------------------------------------------------------------
// init(): called once at startup, equivalent to unity's Start() or Awake() functions
// I'm only just noticing the sdk uses snake_case nomenclature which I've subconsciously been matching, not sure if this is a convention.
// ---------------------------------------------------------------------------
void init() {
  set_screen_mode(ScreenMode::lores);
  new_game();
}

// ---------------------------------------------------------------------------
// update(time): game logic, called every 10 ms, closer to FixedUpdate() in unity than Update().
// ---------------------------------------------------------------------------
void update(uint32_t time) {
  // `buttons` is the held state (a bit per button).
  // `buttons.pressed` is only set on the update where a button went down.
  if (buttons & Button::DPAD_UP)
    player.y -= PLAYER_SPEED;
  if (buttons & Button::DPAD_DOWN)
    player.y += PLAYER_SPEED;
  player.y = clamp_paddle_y(player.y);

  if (state != State::Playing) {
    if (buttons.pressed & Button::A) {
      if (state == State::GameOver) {
        new_game();
      } else {
        launch_ball();
        state = State::Playing;
      }
    }
    return;
  }

  // cpu should chase the middle of the ball, but never faster than CPU_SPEED.
  if (delay_count >= cpu_reaction_time) { // only react every 5 updates (50 ms)
    cpu_target = ball.pos.y + BALL_SIZE / 2.0f - PADDLE_HEIGHT / 2.0f;

    delay_count = 10; // reset the counter so it only reacts every 5 updates (50 ms).
  } else {
    delay_count += 10;
  }

  // these are out here because we want the cpu to be slow to react not slow to move.
  cpu.y += std::clamp(cpu_target - cpu.y, -CPU_SPEED, CPU_SPEED);
  cpu.y = clamp_paddle_y(cpu.y);

  // Move ball.
  ball.pos += ball.vel;

  // Bounce off the top and bottom walls.
  if (ball.pos.y < 0.0f) {
    ball.pos.y = 0.0f;
    ball.vel.y = -ball.vel.y; // crude reflection
  } else if (ball.pos.y > SCREEN_HEIGHT - BALL_SIZE) {
    ball.pos.y = float(SCREEN_HEIGHT - BALL_SIZE);
    ball.vel.y = -ball.vel.y;
  }

  // Paddles. Only bounce if the ball is moving toward that paddle, otherwise
  // it could get stuck flipping direction every update.
  if (ball.vel.x < 0.0f && ball.rect().intersects(player.rect())) {
    bounce_off(player);
  } else if (ball.vel.x > 0.0f && ball.rect().intersects(cpu.rect())) {
    bounce_off(cpu);
  }

  // Ball went off either side: someone scored.
  if (ball.pos.x < -BALL_SIZE) {
    score_point(false);
  } else if (ball.pos.x > SCREEN_WIDTH) {
    score_point(true);
  }
}

// ---------------------------------------------------------------------------
// render(time): drawing only.
// ---------------------------------------------------------------------------
void render(uint32_t time) {
  // clear() fills with the CURRENT pen colour, so set the pen first.
  screen.pen = Pen(0, 0, 0);
  screen.clear();

  // Dashed centre line.
  screen.pen = Pen(90, 90, 90);
  for (int y = 0; y < SCREEN_HEIGHT; y += 8) {
    screen.rectangle(Rect(SCREEN_WIDTH / 2 - 1, y, 2, 4));
  }

  // Paddles and ball.
  screen.pen = Pen(255, 255, 255);
  screen.rectangle(player.rect());
  screen.rectangle(cpu.rect());
  screen.rectangle(ball.rect());

  // Scores.
  screen.text(std::to_string(player_score), minimal_font, Point(SCREEN_WIDTH / 4, 4), true, TextAlign::top_center);
  screen.text(std::to_string(cpu_score), minimal_font, Point(SCREEN_WIDTH * 3 / 4, 4), true, TextAlign::top_center);

  // Messages.
  Rect whole_screen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  if (state == State::Serving) {
    screen.text("Up/Down to move, A (Z key) to serve", minimal_font,
                Point(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 12), true, TextAlign::top_center);
  } else if (state == State::GameOver) {
    screen.text(player_score > cpu_score ? "You win!" : "CPU wins", minimal_font,
                whole_screen, true, TextAlign::center_center);
    screen.text("A (Z key) to play again", minimal_font,
                Point(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 12), true, TextAlign::top_center);
  }
}