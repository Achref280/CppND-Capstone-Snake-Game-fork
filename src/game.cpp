#include "game.h"
#include <iostream>
#include "SDL.h"
#include "config.h"
#include <future>

Game::Game()
    : snake(std::make_unique<Snake>(Config::kGridWidth/2.0, Config::kGridHeight/2.0)), // Initialize the player snake
      obstacle(std::make_unique<Obstacle>(Config::kGridWidth/5.0, Config::kGridHeight/5.0, Obstacle::Shape::kPacman)), // Initialize the AI snake
      ai_snake(std::make_unique<Snake>(Config::kGridWidth/2.0-3, Config::kGridHeight/2.0-3)), // Initialize the AI snake  
      engine(dev()),
      random_w(0, static_cast<int>(Config::kGridWidth - 1)),
      random_h(0, static_cast<int>(Config::kGridHeight - 1)) {
  PlaceFood();
}
std::vector<std::vector<State>> Game::CreateGrid() const {
  // Initialize a grid layout with dimensions (Width x Height) filled with kEmpty states
  std::vector<std::vector<State>> grid(
      Config::kGridWidth,
      std::vector<State>(Config::kGridHeight, State::kEmpty));

  // 1. Mark Obstacle body segments on the grid
  for (const auto &item : obstacle->GetBody()) {
    // Basic bounds checking to prevent out-of-bounds writing
    if (item.x >= 0 && item.x < static_cast<int>(Config::kGridWidth) &&
        item.y >= 0 && item.y < static_cast<int>(Config::kGridHeight)) {
      grid[item.x][item.y] = State::kObstacle;
    }
  }
  /* 
  // 2. Mark Player Snake's head on the grid
  int snake_head_x = static_cast<int>(snake->GetHeadX());
  int snake_head_y = static_cast<int>(snake->GetHeadY());
  if (snake_head_x >= 0 && snake_head_x < static_cast<int>(Config::kGridWidth) &&
      snake_head_y >= 0 && snake_head_y < static_cast<int>(Config::kGridHeight)) {
    grid[snake_head_x][snake_head_y] = State::kObstacle;
  }

  // 3. Mark Player Snake's body segments on the grid
  for (const auto &item : snake->GetBody()) {
     // Basic bounds checking to prevent out-of-bounds writing
    if (item.x >= 0 && item.x < static_cast<int>(Config::kGridWidth) &&
        item.y >= 0 && item.y < static_cast<int>(Config::kGridHeight)) {
      grid[item.x][item.y] = State::kObstacle;
    }
  }
  */
   // 4. Mark AI Snake's head on the grid
  int ai_snake_head_x = static_cast<int>(ai_snake->GetHeadX());
  int ai_snake_head_y = static_cast<int>(ai_snake->GetHeadY());
  if (ai_snake_head_x >= 0 && ai_snake_head_x < static_cast<int>(Config::kGridWidth) &&
      ai_snake_head_y >= 0 && ai_snake_head_y < static_cast<int>(Config::kGridHeight)) {
    grid[ai_snake_head_x][ai_snake_head_y] = State::kObstacle;
  }
  // 5. Mark AI Snake's body segments on the grid
  for (const auto &item : ai_snake->GetBody()) {
     // Basic bounds checking to prevent out-of-bounds writing
    if (item.x >= 0 && item.x < static_cast<int>(Config::kGridWidth) &&
        item.y >= 0 && item.y < static_cast<int>(Config:: kGridHeight)) {
      grid[item.x][item.y] = State::kObstacle;
    }
  }


  return grid;
}
void Game::Run(Controller const &controller, Renderer &renderer) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  // enter user name:
  std::cout << "Please enter your name: ";
  std::string name;
  std::getline(std::cin, name);
  player_score = Score(name, 0);
  player_score.update_score();
  // Print the best score for the player
  std::cout << "Welcome " << player_score.GetPlayerName() << "! Your best previous score is : " << player_score.GetBestScore() << "\n";
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();
    // Generate the dynamic grid sequentially (Highly efficient, no locks needed)
    std::vector<std::vector<State>> dynamic_grid = CreateGrid();
    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, *snake);
    // Execute A* search and evaluate result
    if (!ai_controller.MoveAISnake(*ai_snake, food, dynamic_grid)) {
      std::lock_guard<std::mutex> lock(console_mutex);
      std::cout << "CRITICAL: No path found for AI Snake! Terminating program...\n";
      break; // Exit the game loop
    }
    Update();

    renderer.Render(*snake, food, *obstacle, *ai_snake); 

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < Config::kMsPerFrame) {
      SDL_Delay(Config::kMsPerFrame - frame_duration);
    }
  }
  // Update the score
  player_score.set_score(score);
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    
    // We pass our Block objects directly as the callable targets using std::ref!
    auto check_player = std::async(std::ref(*snake), x, y);
    auto check_obstacle = std::async(std::ref(*obstacle), x, y);
    auto check_ai = std::async(std::ref(*ai_snake), x, y);

    if (!check_player.get() && !check_obstacle.get() && !check_ai.get()) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake->IsAlive() || !ai_snake->IsAlive()) return; 
  // 1. Fire parallel updates using std::async (No shared grid, completely lock-free)
  auto ai_update_task = std::async(std::launch::async, [this]() {
    ai_snake->Update();
  });
  auto obstacle_update_task = std::async(std::launch::async, [this]() {
    obstacle->Update();
  });

  // Update player snake on main thread alongside helper tasks
  snake->Update();

  // Wait for background compute threads to safely synchronize
  ai_update_task.wait();
  obstacle_update_task.wait();
 
   // If Obstacle moved into the food, we need to place a new one before checking collisions
  if (obstacle->BlockCell(food.x, food.y)) {
    PlaceFood();
  }  
  int new_x = static_cast<int>(snake->GetHeadX());
  int new_y = static_cast<int>(snake->GetHeadY());

  int ai_new_x = static_cast<int>(ai_snake->GetHeadX());
  int ai_new_y = static_cast<int>(ai_snake->GetHeadY());

  // 1. Create a promise and get its associated future
  std::promise<bool> player_hit_promise;
  std::future<bool> player_hit_future = player_hit_promise.get_future();

  // 2. Launch a thread, moving the promise into the lambda
  std::thread player_hit_thread([&](std::promise<bool> p, int x, int y) {
    bool is_hit = obstacle->BlockCell(x, y);
    // SAFEGUARDED CONSOLE WRITE
    if (is_hit) {
      std::lock_guard<std::mutex> lock(console_mutex);
      std::cout << "CRITICAL: Collision detected on background thread!\n";
    }
    p.set_value(is_hit); // Fulfill the promise!
  }, std::move(player_hit_promise), new_x, new_y);

  // Keep AI check as async for contrast/comparison
  auto ai_hit_obstacle = std::async(std::ref(*obstacle), ai_new_x, ai_new_y);

  // 3. Wait for the exact promise value using the future
  bool hit_obstacle_result = player_hit_future.get();
  
  // Clean up the manual thread
  player_hit_thread.join();

  // Check if the snake hit the obstacle
  if (hit_obstacle_result) {
    snake->SetAlive(false); // Mark player snake as dead
    return;
  }
 
  // 2. Check if the AI snake hit the obstacle
  if (ai_hit_obstacle.get()) {
    ai_snake->SetAlive(false); // Mark AI snake as dead
    return;
  }

  // Handle food consumption for both snakes using the clean helper method
  HandleFoodCollision(*ai_snake, *snake, ai_score, 0.01f, 0.02f);
  HandleFoodCollision(*snake, *ai_snake, score, 0.02f, 0.02f);

}
void Game::HandleFoodCollision(Snake &eating_snake, Snake &other_snake, int &target_score, float eat_speed_inc, float other_speed_inc) {
  int head_x = static_cast<int>(eating_snake.GetHeadX());
  int head_y = static_cast<int>(eating_snake.GetHeadY());

  if (food.x == head_x && food.y == head_y) {
    target_score++;
    PlaceFood();
    eating_snake.GrowBody();
    eating_snake.SetSpeed(eating_snake.GetSpeed() + eat_speed_inc);
    other_snake.SetSpeed(other_snake.GetSpeed() + other_speed_inc);
  }
}