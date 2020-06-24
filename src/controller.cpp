#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"
#include "MessageQueue.h"
#include <chrono>
#include <vector>
#include <thread>
// if key is pressed, send the key (other than Quit)
void Controller::HandleInput() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      Keys.send(std::move(SDLK_RETURN));
    } else if (e.type == SDL_KEYDOWN) {
      Keys.send(std::move(e.key.keysym.sym));
    }
  }
}
// The first argument is the vector that will hold the controller thread, and
// the thread will run while the other flag is true
void Controller::Launch(std::vector<std::thread>& RunningThreads,
                        bool& RunningFlag)
{
  RunningThreads.emplace_back(
      std::thread(&Controller::thread, this, &RunningFlag));
}
void Controller::thread(bool* RunningFlag) {
  while (*RunningFlag == true) {
    HandleInput();
    std::this_thread::sleep_for(std::chrono::milliseconds(10) );
  }
}
SDL_Keycode Controller::GetKey() {
  SDL_Keycode Key = Keys.recieve();
  if (Key == (SDL_Keycode)std::cv_status::timeout) {
    Key = SDLK_UNKNOWN;
  }
  return Key;
}