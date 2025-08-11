#ifndef STATE_HPP
#define STATE_HPP

#include "Game.hpp"
#include <unordered_set>
#include <queue>
#include <string>
#include <vector>

class State {
public:
    Game G; 
    int moves; 
    std::vector<std::vector<int>> all_moves; 
    int h_value; 
    int f_value; 

    State(Game G);
    State(Game G, int moves);
    State(Game G, int moves, std::vector<std::vector<int>> all_moves);

    struct StateHash {
        size_t operator()(const State& state) const;
    };

    bool operator<(const State& other) const;
    bool operator==(const State& other) const;

    void update_heuristic(std::string method);
    std::vector<std::vector<int>> get_State();
    bool success();
    void showState();
    bool succeed();

    std::unordered_set<State, StateHash> next_States();
    std::unordered_set<State, StateHash> next_States_with_heuristic(std::string method);

    std::vector<std::vector<int>> shortest_path();
    std::vector<std::vector<int>> shortest_path_with_heuristic(std::string method);

private:
    void states_in_direction(std::unordered_set<State, StateHash>& result, int i, bool right);
    void states_in_direction_with_heuristic(std::unordered_set<State, StateHash>& result, int i, bool right, std::string method);
};

#endif

