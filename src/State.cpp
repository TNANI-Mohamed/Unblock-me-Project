#include "State.hpp"
#include <iostream>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <limits>

// ======= Constructors ======= //
State::State(Game G) : G(G), moves(0), h_value(0), f_value(0) {}
State::State(Game G, int moves) : G(G), moves(moves), h_value(0), f_value(0) {}
State::State(Game G, int moves, std::vector<std::vector<int>> all_moves)
    : G(G), moves(moves), all_moves(all_moves), h_value(0), f_value(0) {}

// ======= Hash ======= //
size_t State::StateHash::operator()(const State& state) const {
    Game::GameHash gameHasher;
    return gameHasher(state.G);
}

// ======= Comparisons ======= //
bool State::operator<(const State& other) const {
    return f_value > other.f_value; // Min-heap behavior for priority_queue
}

bool State::operator==(const State& other) const {
    return G == other.G;
}

// ======= Heuristic update ======= //
void State::update_heuristic(std::string method) {
    if (method == "blocking") {
        int blocks = 0;
        int row = G.positions[0][1] - 1;
        int col_end = G.positions[0][0] - 1 + G.lengths[0];
        for (int c = col_end; c < G.size; ++c) {
            if (!G.free[row][c]) blocks++;
        }
        h_value = blocks;
    } else {
        h_value = 0;
    }
    f_value = moves + h_value;
}

// ======= Get State ======= //
std::vector<std::vector<int>> State::get_State() {
    return G.positions;
}

// ======= Success checks ======= //
bool State::success() {
    int row = G.positions[0][1] - 1;
    int col_end = G.positions[0][0] - 1 + G.lengths[0];
    return col_end == G.size;
}

bool State::succeed() {
    return success();
}

// ======= Show ======= //
void State::showState() {
    for (int r = 0; r < G.size; r++) {
        for (int c = 0; c < G.size; c++) {
            std::cout << (G.free[r][c] ? "." : "#");
        }
        std::cout << "\n";
    }
    std::cout << "Moves: " << moves << "\n";
}

// ======= State expansion helpers ======= //
void State::states_in_direction(std::unordered_set<State, State::StateHash>& result, int i, bool right) {
    int step = right ? 1 : -1;
    Game new_game = G;
    while (true) {
        if (G.horiz[i]) {
            int new_col = new_game.positions[i][0] - 1 + step;
            int end_col = new_col + new_game.lengths[i] - 1;
            if (new_col < 0 || end_col >= G.size) break;
            if (!G.free[new_game.positions[i][1] - 1][right ? end_col : new_col]) break;
            new_game.positions[i][0] += step;
        } else {
            int new_row = new_game.positions[i][1] - 1 + step;
            int end_row = new_row + new_game.lengths[i] - 1;
            if (new_row < 0 || end_row >= G.size) break;
            if (!G.free[right ? end_row : new_row][new_game.positions[i][0] - 1]) break;
            new_game.positions[i][1] += step;
        }
        if (!new_game.update_free()) break;
        State new_state(new_game, moves + 1, all_moves);
        new_state.all_moves.push_back({i, step});
        result.insert(new_state);
    }
}

void State::states_in_direction_with_heuristic(std::unordered_set<State, State::StateHash>& result, int i, bool right, std::string method) {
    int step = right ? 1 : -1;
    Game new_game = G;
    while (true) {
        if (G.horiz[i]) {
            int new_col = new_game.positions[i][0] - 1 + step;
            int end_col = new_col + new_game.lengths[i] - 1;
            if (new_col < 0 || end_col >= G.size) break;
            if (!G.free[new_game.positions[i][1] - 1][right ? end_col : new_col]) break;
            new_game.positions[i][0] += step;
        } else {
            int new_row = new_game.positions[i][1] - 1 + step;
            int end_row = new_row + new_game.lengths[i] - 1;
            if (new_row < 0 || end_row >= G.size) break;
            if (!G.free[right ? end_row : new_row][new_game.positions[i][0] - 1]) break;
            new_game.positions[i][1] += step;
        }
        if (!new_game.update_free()) break;
        State new_state(new_game, moves + 1, all_moves);
        new_state.all_moves.push_back({i, step});
        new_state.update_heuristic(method);
        result.insert(new_state);
    }
}

// ======= Generate next states ======= //
std::unordered_set<State, State::StateHash> State::next_States() {
    std::unordered_set<State, State::StateHash> result;
    for (int i = 0; i < G.nbCars; ++i) {
        states_in_direction(result, i, true);
        states_in_direction(result, i, false);
    }
    return result;
}

std::unordered_set<State, State::StateHash> State::next_States_with_heuristic(std::string method) {
    std::unordered_set<State, State::StateHash> result;
    for (int i = 0; i < G.nbCars; ++i) {
        states_in_direction_with_heuristic(result, i, true, method);
        states_in_direction_with_heuristic(result, i, false, method);
    }
    return result;
}

// ======= Shortest path ======= //
std::vector<std::vector<int>> State::shortest_path() {
    std::queue<State> q;
    std::unordered_set<State, State::StateHash> visited;
    q.push(*this);
    visited.insert(*this);

    while (!q.empty()) {
        State cur = q.front();
        q.pop();

        if (cur.success()) {
            return cur.all_moves;
        }

        for (const auto& next : cur.next_States()) {
            if (!visited.count(next)) {
                visited.insert(next);
                q.push(next);
            }
        }
    }
    return {};
}

std::vector<std::vector<int>> State::shortest_path_with_heuristic(std::string method) {
    auto cmp = [](const State& a, const State& b) { return a < b; };
    std::priority_queue<State, std::vector<State>, decltype(cmp)> pq(cmp);
    std::unordered_set<State, State::StateHash> visited;

    pq.push(*this);

    while (!pq.empty()) {
        State cur = pq.top();
        pq.pop();

        if (cur.success()) {
            return cur.all_moves;
        }

        visited.insert(cur);

        for (const auto& next : cur.next_States_with_heuristic(method)) {
            if (!visited.count(next)) {
                pq.push(next);
            }
        }
    }
    return {};
}
