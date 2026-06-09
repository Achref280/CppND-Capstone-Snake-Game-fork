#ifndef SCORE_H
#define SCORE_H
#include <string>
class Score {
public:
    Score() = default;
    Score(std::string name, int score) : _player_name(name), _score(score){}
    int GetScore() const {
        return _score;
    }
    std::string GetPlayerName() const {
        return _player_name;
    }
    void update_best_score(int new_score) {
        if (new_score > _best_score) {
            _best_score = new_score;
        }
    }
    int GetBestScore() const {
        return _best_score;
    }
    void set_score(int score) {
        _score = score;
    }
    void write_score_to_file();
    void update_score();

    bool is_new_record() const {
        return _score > _best_score;
    }

private:
    int _score{0};
    std::string _player_name;
    int _best_score{0};
};
#endif