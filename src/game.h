#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "score.h"
#include <string>
#include <memory.h>
#include <obstacle.h>
#include "astar.h"
#include "ai_controller.h"

class Game {
 public:
  Game();
  void Run(Controller const &controller, Renderer &renderer);
  int GetScore() const { return player_score.GetScore();}
  std::string GetPlayerName() const { return player_score.GetPlayerName(); } 
  bool NewRecord() const {
      return player_score.is_new_record();
  }
  int GetSize() const { return snake->GetSize(); }
  int GetBestScore() const { return player_score.GetBestScore(); }
  void write_score_to_file() {
      player_score.write_score_to_file();
  }
 private:

  std::unique_ptr<Obstacle> obstacle; // Static obstacle that the player snake must avoid

  std::unique_ptr<Snake> snake;
  std::unique_ptr<Snake> ai_snake; // AI snake that will use A* algorithm to find the food
  SDL_Point food;
  AIController ai_controller;


  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  int ai_score{0};
  Score player_score;
  void HandleFoodCollision(Snake &eating_snake, Snake &other_snake, int &target_score, float eat_speed_inc, float other_speed_inc);

  void PlaceFood();
  void Update();
  std::vector<std::vector<State>> CreateGrid() const; // Helper to create a grid representation of the current game state for A* pathfinding
};

#endif