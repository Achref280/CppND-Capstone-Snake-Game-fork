#include <string>
#include "score.h"
#include <fstream>
#include <sstream>
#include <vector>
void Score::update_score() {
    // open file an read the best score for a given player
    std::ifstream infile("./../scores.txt");
    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string name;
        int score;
        if (!(iss >> name >> score)) { break; } // error
        if (name == _player_name) {
            _best_score = score;
            break;
        }
    }
    infile.close();
}

void Score::write_score_to_file() {
    std::ofstream outfile("scores.txt", std::ios_base::app);
    // find old score for the player and update it if the new score is better
    std::ifstream infile("scores.txt");
    std::string line;
    std::vector<std::pair<std::string, int>> scores;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string name;
        int score;
        if (!(iss >> name >> score)) { break; } // error
        if (name == _player_name) {
            score = std::max(score, _score);
        }
        scores.push_back({name, score});
    }
    infile.close();
    // write all scores back to the file
    std::ofstream outfile2("./../scores.txt");
    // If the player is not in the file, add them with their score
    if (std::find_if(scores.begin(), scores.end(), [this](const std::pair<std::string, int>& pair) { return pair.first == _player_name; }) == scores.end()) {
        scores.push_back({_player_name, _score});
        }
    for (const auto& pair : scores) {
        outfile2 << pair.first << " " << pair.second << std::endl;
    }
    outfile2.close();
}