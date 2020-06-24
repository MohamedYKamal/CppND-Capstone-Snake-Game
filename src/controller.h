#ifndef CONTROLLER_H
#define CONTROLLER_H
//Now the controller class can be used separately, with any game
#include <iostream>
#include "SDL.h"
#include "snake.h"
#include "MessageQueue.h"
#include <vector>
#include <thread>
class Controller
{
public:
    void Launch(std::vector<std::thread> &RunningThreads, bool &TermainateCondition);
    SDL_Keycode GetKey();
    Controller():Keys(10){}

private:
    void thread(bool* RunningFlag);
    void HandleInput();
    MessageQueue<SDL_Keycode> Keys;
};

#endif