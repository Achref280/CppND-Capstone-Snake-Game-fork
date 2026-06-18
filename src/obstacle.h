#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "block.h"
#include <cmath>
#include <iostream>
#include <random> // Added library for random movement
#include "config.h"
class Obstacle : public Block {
 public:
  // Constructor forwards coordinates to the base Block class constructor
    // Typed shapes enumeration
    enum class Shape {
      kSingleBlock,
      kCross,
      kGateway,
      kHollowFrame,
      kWolf,
      kPacman
    };
  
    // Constructor accepts an optional default Shape selector
    Obstacle(int x, int y, Shape shape = Shape::kCross);

  // Only override what behaves differently
  void Update() override;
 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);
  void BuildShape(int x, int y, Shape shape);
  void BuildCross(int x, int y);
  void BuildGateway();
  void BuildHollowFrame(int x, int y);
  void BuildWolf(int x, int y);
  void AddBodyPoint(int x, int y); // Helper to handle boundary wrapping safely
  void BuildPacman(int x, int y);
  Shape current_shape; // Keep track of the active shape



};

#endif