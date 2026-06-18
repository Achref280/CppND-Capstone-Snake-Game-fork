#ifndef AI_CONTROLLER_H
#define AI_CONTROLLER_H

#include <vector>
#include "snake.h"
#include "astar.h"

class AIController {
 public:
  AIController() = default;
  bool MoveAISnake(Snake &ai_snake, const SDL_Point &food, const std::vector<std::vector<State>> &grid);
};

#endif