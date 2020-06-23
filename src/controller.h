#ifndef CONTROLLER_H
#define CONTROLLER_H
//Now the controller class can be used separately, with any game

class Controller
{
public:
    void Launch(std::vector<std::thread> &RunningThreads, bool &TermainateCondition);
    SDL_Keycode GetKey();

private:
    void thread();
    void HandleInput(bool &TermainateFlag);
    MessageQueue<SDL_Keycode> Keys(25);
};

#endif