#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main() {
  Renderer renderer;
  Controller controller;
  Game game;
  game.Run(controller, renderer);
  std::cout << "Game has terminated successfully!\n";
  std::cout << game.GetPlayerName() <<" your score is: " << game.GetScore() << "\n";
  if (game.NewRecord()) {
    std::cout << "Congratulations! You have a new record!\n";
  } else {
    std::cout << "Better luck next time! Try to beat the record of " << game.GetBestScore() << "!\n";
  }
  // std::cout << "Size: " << game.GetSize() << "\n";
  game.write_score_to_file();
  return 0;
}