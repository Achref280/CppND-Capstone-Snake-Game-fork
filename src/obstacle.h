#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "block.h"

class Obstacle : public Block {
 public:
  // Constructor forwards coordinates to the base Block class constructor
  Obstacle(int x, int y) : Block(static_cast<float>(x), static_cast<float>(y)) {
    // Body initialization can be done here if needed, but for a simple obstacle, we might just want to keep it as a single block.
    // Body is a rectangular area 4*3 that the obstacle occupies, so we can initialize it with the starting position of the obstacle.
    for (int i = 0; i <= 2; ++i) {
      body.push_back({x, y + i}); // Add a second segment to make the obstacle more visible
      body.push_back({x + 1, y + i}); // Add a second segment to make the obstacle more visible
      body.push_back({x + 2, y + i}); // Add a second segment to make the obstacle more visible
      body.push_back({x + 3, y + i}); // Add a second segment to make the obstacle more visible
    }
    direction = Direction::kRight; // Initial direction (can be changed later)
    speed = 0.1f; // Initial speed (can be adjusted as needed)
  }

  // Only override what behaves differently
  void Update() override;
 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);
};

#endif