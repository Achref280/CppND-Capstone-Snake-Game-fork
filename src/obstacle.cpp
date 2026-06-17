#include "obstacle.h"
#include <cmath>
#include <iostream>
#include "config.h"

Obstacle::Obstacle(int x, int y, Shape shape) 
    : Block(static_cast<float>(x), static_cast<float>(y)), current_shape(shape) {
  speed = 0.05f; // Set a movement speed for the shape
  direction = Block::Direction::kRight; // Set initial movement direction
  BuildShape(x, y, current_shape);
}

void Obstacle::AddBodyPoint(int x, int y) {
  // Gracefully wrap individual parts when moving through the screen limits
  int wrapped_x = (x + Config::kGridWidth) % Config::kGridWidth;
  int wrapped_y = (y + Config::kGridHeight) % Config::kGridHeight;
  body.push_back(SDL_Point{wrapped_x, wrapped_y});
}

void Obstacle::BuildShape(int x, int y, Shape shape) {
  body.clear();
  switch (shape) {
    case Shape::kSingleBlock:
      AddBodyPoint(x, y);
      break;
    case Shape::kCross:
      BuildCross(x, y);
      break;
    case Shape::kGateway:
      BuildGateway();
      break;
    case Shape::kHollowFrame:
      BuildHollowFrame(x, y);
      break;
    case Shape::kWolf:
      BuildWolf(x, y);
      break;
  }
}

void Obstacle::BuildWolf(int x, int y) {
  std::vector<SDL_Point> offsets = {
    // Left Ear
    {-2, -3}, {-2, -2},
    // Right Ear
    {2, -3}, {2, -2},
    // Forehead / Top of Head
    {-1, -1}, {0, -1}, {1, -1},
    // Head / Eye Level
    {-3, 0}, {-2, 0}, {-1, 0}, {1, 0}, {2, 0}, {3, 0},
    // Center Face
    {0, 0}, {0, 1},
    // Muzzle / Cheeks
    {-2, 1}, {-1, 1}, {1, 1}, {2, 1},
    // Nose
    {-1, 2}, {0, 2}, {1, 2},
    // Chin / Neck Base
    {-1, 3}, {0, 3}, {1, 3}, {0, 4}
  };

  for (auto const &offset : offsets) {
    AddBodyPoint(x + offset.x, y + offset.y);
  }
}

void Obstacle::BuildCross(int x, int y) {
  int length = 3;
  // Horizontal line
  for (int i = -length; i <= length; ++i) {
    AddBodyPoint(x + i, y);
  }
  // Vertical line
  for (int i = -length; i <= length; ++i) {
    if (i != 0) { 
      AddBodyPoint(x, y + i);
    }
  }
}

void Obstacle::BuildGateway() {
  int size = 4;
  int w = Config::kGridWidth;
  int h = Config::kGridHeight;

  // L-bracket in the top-left corner
  for (int i = 2; i < 2 + size; ++i) {
    AddBodyPoint(i, 2);
    AddBodyPoint(2, i);
  }
  // L-bracket in the bottom-right corner
  for (int i = 2; i < 2 + size; ++i) {
    AddBodyPoint(w - i - 1, h - 3);
    AddBodyPoint(w - 3, h - i - 1);
  }
}

void Obstacle::BuildHollowFrame(int x, int y) {
  int half_size = 3;
  for (int dx = -half_size; dx <= half_size; ++dx) {
    for (int dy = -half_size; dy <= half_size; ++dy) {
      if (dx == -half_size || dx == half_size || dy == -half_size || dy == half_size) {
        AddBodyPoint(x + dx, y + dy);
      }
    }
  }
}
void Obstacle::Update() {
  UpdateHead();
  
  // Rebuild the shape at the shifted float-to-int coordinate space 
  BuildShape(static_cast<int>(head_x), static_cast<int>(head_y), current_shape);
}
void Obstacle::UpdateHead() {
  switch (direction) {
    case Block::Direction::kUp:    head_y -= speed; break;
    case Block::Direction::kDown:  head_y += speed; break;
    case Block::Direction::kLeft:  head_x -= speed; break;
    case Block::Direction::kRight: head_x += speed; break;
  }

  // Wrap the Obstacle pivot around screen borders
  head_x = fmod(head_x + Config::kGridWidth, Config::kGridWidth);
  head_y = fmod(head_y + Config::kGridHeight, Config::kGridHeight);
}

void Obstacle::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }
}