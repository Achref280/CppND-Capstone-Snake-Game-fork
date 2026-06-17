#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "block.h"

class Obstacle : public Block {
 public:
  // Constructor forwards coordinates to the base Block class constructor
  Obstacle(int x, int y) : Block(static_cast<float>(x), static_cast<float>(y)) {}

  // Only override what behaves differently
  void Update() override;
 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);
};

#endif