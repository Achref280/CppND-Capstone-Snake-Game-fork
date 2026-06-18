#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <string>

// Ensure State is visible globally
enum class State { kEmpty, kObstacle, kClosed, kPath, kStart, kFinish };

namespace Astar {
  std::vector<State> ParseLine(std::string line);
  std::vector<std::vector<State>> ReadBoardFile(std::string path);
  bool Compare(const std::vector<int> a, const std::vector<int> b);
  void CellSort(std::vector<std::vector<int>> *v);
  int Heuristic(int x1, int y1, int x2, int y2);
  bool CheckValidCell(int x, int y, std::vector<std::vector<State>> &grid);
  void AddToOpen(int x, int y, int g, int h, std::vector<std::vector<int>> &openlist, std::vector<std::vector<State>> &grid);
  void ExpandNeighbors(const std::vector<int> &current, int goal[2], std::vector<std::vector<int>> &openlist, std::vector<std::vector<State>> &grid);
  std::vector<std::vector<State>> Search(std::vector<std::vector<State>> grid, int init[2], int goal[2]);
  std::string CellString(State cell);
  void PrintBoard(const std::vector<std::vector<State>> board);
}

#endif