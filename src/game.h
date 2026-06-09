#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "score.h"
#include <string>

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const { return player_score.GetScore();}
  std::string GetPlayerName() const { return player_score.GetPlayerName(); } 
  bool NewRecord() const {
      return player_score.is_new_record();
  }
  int GetSize() const { return snake.size; }
  int GetBestScore() const { return player_score.GetBestScore(); }
  void write_score_to_file() {
      player_score.write_score_to_file();
  }
 private:
  Snake snake;
  SDL_Point food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  Score player_score;

  void PlaceFood();
  void Update();
};

#endif