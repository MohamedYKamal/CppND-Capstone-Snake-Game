#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <random>

class Game
{
public:
    Game(const std::size_t grid_width, const std::size_t grid_height, const std::size_t screen_width, const std::size_t screen_height);
    void Run(std::size_t target_frame_duration);
    int GetScore() const;
    int GetSize() const;
    ~Game();

private:
    Snake snake;
    SDL_Point food;
    //encapsulate renderer and controller inside the Game class
    Renderer renderer;
    Controller controller;
    // end of newly added encapsualtion
    std::random_device dev;
    std::mt19937 engine;
    std::uniform_int_distribution<int> random_w;
    std::uniform_int_distribution<int> random_h;
    std::vector<std::thread> RunningThreads;
    int score{0};
    Snake::Direction ControllerSnakeMapping(SDL_Keycode);
    Snake::Direction GetOpposite(SDL_Keycode in);
    void PlaceFood();
    void Update();
};

#endif