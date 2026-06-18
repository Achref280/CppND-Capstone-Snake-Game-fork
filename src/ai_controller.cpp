#include "ai_controller.h"
#include "config.h"

bool AIController::MoveAISnake(Snake &ai_snake, const SDL_Point &food, const std::vector<std::vector<State>> &grid) {
  int init[2] = {static_cast<int>(ai_snake.GetHeadX()), static_cast<int>(ai_snake.GetHeadY())};
  int goal[2] = {food.x, food.y};

  std::vector<std::vector<State>> path = Astar::Search(grid, init, goal);

  if (!path.empty()) {
    int head_x = init[0];
    int head_y = init[1];

    const int neighbors[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    const Block::Direction dirs[4] = {
        Block::Direction::kLeft,
        Block::Direction::kRight,
        Block::Direction::kUp,
        Block::Direction::kDown
    };

    for (int i = 0; i < 4; ++i) {
      int nx = (head_x + neighbors[i][0] + static_cast<int>(Config::kGridWidth)) % Config::kGridWidth;
      int ny = (head_y + neighbors[i][1] + static_cast<int>(Config::kGridHeight)) % Config::kGridHeight;

      State neighbor_state = path[nx][ny];
      if (neighbor_state == State::kPath || neighbor_state == State::kFinish) {
        ai_snake.SetDirection(dirs[i]);
        return true; // Move direction set successfully
      }
    }
  }
  return false; // No valid move found
}