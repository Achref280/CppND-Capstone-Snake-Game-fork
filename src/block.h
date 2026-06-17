#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include "SDL.h"

class Block {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  // Base constructor initializes shared variables
  Block(float x, float y) 
      : head_x(x), head_y(y), direction(Direction::kUp), speed(0.0f), size(1){}
  
  virtual ~Block() = default;

  // Behavioral methods containing default or abstract logic
  virtual void Update() = 0; // Pure virtual: Snake & Obstacle update differently
  virtual void GrowBody() {}  // Virtual default: Obstacles don't grow, so they do nothing
  
  // Shared logic: Both Snake and Obstacle check collision the same way
  virtual bool BlockCell(int x, int y) const {
    if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
      return true;
    }
    for (auto const &item : body) {
      if (x == item.x && y == item.y) {
        return true;
      }
    }
    return false;
  }

  // Common Getters and Setters implemented in base class
  Direction GetDirection() const { return direction; }
  void SetDirection(Direction dir) { direction = dir; }
  
  float GetSpeed() const { return speed; }
  void SetSpeed(float spd) { speed = spd; }
  
  int GetSize() const { return size; }
  float GetHeadX() const { return head_x; }
  float GetHeadY() const { return head_y; }
  const std::vector<SDL_Point>& GetBody() const { return body; }

 protected:
  // Move shared state variable definitions here
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;
  Direction direction;
  float speed;
  bool growing{false};
  int size{1};
};

#endif