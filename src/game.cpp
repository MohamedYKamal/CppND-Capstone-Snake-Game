#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(const std::size_t grid_width, const std::size_t grid_height,
           const std::size_t screen_width, const std::size_t screen_height)
    : snake(grid_width, grid_height),
      renderer(screen_width, screen_height, grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)) {
  PlaceFood();
}
~Game::Game() {
  for (auto &i : RunningThreads) {
    i.join();
  }
}
void Game::Run(std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  controller.Launch(RunningThreads, running);
  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    Snake::Direction PressedKey = ControllerSnakeMapping(controller.GetKey());
    if (PressedKey == Snake::Direction::kTerminate) {
      running = false;
    } else {
      snake.ChangeDirection(PressedKey, GetOpposite(PressedKey));
    }

    Update();
    renderer.Render(snake, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

Snake::Direction Game::ControllerSnakeMapping(SDL_Keycode in) {
  switch (in) {
    case SDLK_UP:
      return Snake::Direction::kUp;
      break;
    case SDLK_DOWN:
      return Snake::Direction::kDown;
      break;
    case SDLK_LEFT:
      return Snake::Direction::kLeft;
      break;
    case SDLK_RIGHT:
      return Snake::Direction::kRight;
      break;
    case SDLK_UNKNOWN:
      return Snake::Direction::kTimeout;
      break;
    case SDLK_RETURN:
      return Snake::Direction::kTerminate;
      break;
    default:
      // discard
      break;
  }
}

Snake::Direction Game::GetOpposite(SDL_Keycode in) {
  switch (in) {
    case Snake::Direction::kUp:
      return Snake::Direction::kDown;
      break;
    case Snake::Direction::kDown:
      return Snake::Direction::kUp;
      break;
    case Snake::Direction::kLeft:
      return Snake::Direction::kRight;
      break;
    case Snake::Direction::kRight:
      return Snake::Direction::kLeft;
      break;
    default:
      // discard
      break;
  }
}