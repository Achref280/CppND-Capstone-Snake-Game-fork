#include "obstacle.h"
#include <cmath>
#include <iostream>
#include "config.h"
void Obstacle::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the Obstacle head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Obstacle::UpdateHead() {
  switch (direction) {
    case Block::Direction::kUp:
      head_y -= speed;
      break;

    case Block::Direction::kDown:
      head_y += speed;
      break;

    case Block::Direction::kLeft:
      head_x -= speed;
      break;

    case Block::Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Obstacle around to the beginning if going off of the screen.
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

  // Check if the Obstacle has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}