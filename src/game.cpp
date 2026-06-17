#include "game.h"
#include <iostream>
#include "SDL.h"
#include "config.h"

Game::Game()
    : snake(std::make_unique<Snake>(Config::kGridWidth/2.0, Config::kGridHeight/2.0)), // Initialize the player snake
      obstacle(std::make_unique<Obstacle>(Config::kGridWidth/5.0, Config::kGridHeight/5.0)), // Initialize the AI snake
      engine(dev()),
      random_w(0, static_cast<int>(Config::kGridWidth - 1)),
      random_h(0, static_cast<int>(Config::kGridHeight - 1)) {
  PlaceFood();
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

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, *snake);
    Update();
    renderer.Render(*snake, food, *obstacle); 

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
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake->SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake->IsAlive()) return;

  snake->Update();

  int new_x = static_cast<int>(snake->GetHeadX());
  int new_y = static_cast<int>(snake->GetHeadY());

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake->GrowBody();
    snake->SetSpeed(snake->GetSpeed() + 0.02);
  }
}