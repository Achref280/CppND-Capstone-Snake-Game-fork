#include "astar.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "config.h"

using std::cout;
using std::ifstream;
using std::istringstream;
using std::sort;
using std::string;
using std::vector;
using std::abs;

// directional deltas can stay anonymous in the cpp file
namespace {
  const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
}

namespace Astar {

vector<State> ParseLine(string line) {
  istringstream sline(line);
  int n;
  char c;
  vector<State> row;
  while (sline >> n >> c && c == ',') {
    if (n == 0) {
      row.push_back(State::kEmpty);
    } else {
      row.push_back(State::kObstacle);
    }
  }
  return row;
}

vector<vector<State>> ReadBoardFile(string path) {
  ifstream myfile(path);
  vector<vector<State>> board{};
  if (myfile) {
    string line;
    while (getline(myfile, line)) {
      vector<State> row = ParseLine(line);
      board.push_back(row);
    }
  }
  return board;
}

bool Compare(const vector<int> a, const vector<int> b) {
  int f1 = a[2] + a[3]; 
  int f2 = b[2] + b[3]; 
  return f1 > f2; 
}

void CellSort(vector<vector<int>> *v) {
  sort(v->begin(), v->end(), Compare);
}

int Heuristic(int x1, int y1, int x2, int y2) {
  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  
  // Calculate shortest path accounting for wrap-around
  dx = std::min(dx, static_cast<int>(Config::kGridWidth) - dx);
  dy = std::min(dy, static_cast<int>(Config::kGridHeight) - dy);
  
  return dx + dy;
}

bool CheckValidCell(int x, int y, vector<vector<State>> &grid) {
  // Since coordinates are pre-wrapped in ExpandNeighbors, we just check grid State
  return grid[x][y] == State::kEmpty;
}

void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openlist, vector<vector<State>> &grid) {
  openlist.push_back(vector<int>{x, y, g, h});
  grid[x][y] = State::kClosed;
}

void ExpandNeighbors(const vector<int> &current, int goal[2], vector<vector<int>> &openlist, vector<vector<State>> &grid) {
  int x = current[0];
  int y = current[1];
  int g = current[2];

  int width = static_cast<int>(grid.size());
  int height = static_cast<int>(grid[0].size());

  for (int i = 0; i < 4; i++) {
    // Apply modulo arithmetic to gracefully wrap search coordinates across borders
    int x2 = (x + delta[i][0] + width) % width;
    int y2 = (y + delta[i][1] + height) % height;

    if (CheckValidCell(x2, y2, grid)) {
      int g2 = g + 1;
      int h2 = Heuristic(x2, y2, goal[0], goal[1]);
      AddToOpen(x2, y2, g2, h2, openlist, grid);
    }
  }
}

vector<vector<State>> Search(vector<vector<State>> grid, int init[2], int goal[2]) {
  vector<vector<int>> open {};
  
  int x = init[0];
  int y = init[1];
  int g = 0;
  int h = Heuristic(x, y, goal[0], goal[1]);
  AddToOpen(x, y, g, h, open, grid);

  while (!open.empty()) {
    CellSort(&open);
    auto current = open.back();
    open.pop_back();
    x = current[0];
    y = current[1];
    grid[x][y] = State::kPath;

    if (x == goal[0] && y == goal[1]) {
      grid[init[0]][init[1]] = State::kStart;
      grid[goal[0]][goal[1]] = State::kFinish;
      return grid;
    }
    
    ExpandNeighbors(current, goal, open, grid);
  }
  
  cout << "No path found!" << "\n";
  return std::vector<vector<State>>{};
}

string CellString(State cell) {
  switch(cell) {
    case State::kObstacle: return "⛰️   ";
    case State::kPath: return "🚗   ";
    case State::kStart: return "🚦   ";
    case State::kFinish: return "🏁   ";
    default: return "0   "; 
  }
}

void PrintBoard(const vector<vector<State>> board) {
  for (size_t i = 0; i < board.size(); i++) {
    for (size_t j = 0; j < board[i].size(); j++) {
      cout << CellString(board[i][j]);
    }
    cout << "\n";
  }
}

} // namespace Astar