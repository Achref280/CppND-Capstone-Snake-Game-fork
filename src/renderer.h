#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"
#include "obstacle.h"

class Renderer {
 public:
  Renderer();
  ~Renderer();

  void Render(Snake const &snake, SDL_Point const &food, Obstacle const &obstacle, Snake const &ai_snake);
  void UpdateWindowTitle(int score, int fps);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;
};

#endif