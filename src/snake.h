#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"
#include "block.h"

class Snake : public Block {
 public:
  Snake(int head_x, int head_y)
      : Block(head_x, head_y){
    speed = 0.1f; // Override default speed from Block constructor
  }

  // Only override the methods that have Snake-specific logic
  void Update() override;
  void GrowBody() override;
  //Setter and getter for alive status
  bool IsAlive() const { return alive; }
  void SetAlive(bool status) { alive = status; }
 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);
  bool alive{true}; // Snake starts alive, and can die by colliding with itself or the obstacle
};

#endif