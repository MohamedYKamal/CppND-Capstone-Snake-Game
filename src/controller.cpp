#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"
#include "MessageQueue.h"
// if key is pressed, send the key (other than Quit)
void Controller::HandleInput(bool& TermainateFlag) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      Keys.Send(SDLK_RETURN);
    } else if (e.type == SDL_KEYDOWN) {
      Keys.Send(e.key.keysym.sym);
    }
  }
}
// The first argument is the vector that will hold the controller thread, and
// the thread will run while the other flag is true
void controller::Launch(std::vector<std::thread>& RunningThreads,
                        bool& RunningFlag);
{
  RunningThreads.emplace_back(
      std::thread(&Controller::thread, this, RunningFlag));
}
void Controller::thread(bool& RunningFlag) {
  while (RunningFlag == true) {
    HandleInput();
  }
}
SDL_Keycode Controller::GetKey() {
  SDL_Keycode Key = Keys.receive();
  if (Key == (T)cv_status::timeout) {
    Key = SDLK_UNKNOWN;
  }
  return Key;
}