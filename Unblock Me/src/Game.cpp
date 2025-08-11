#include "Game.hpp"
#include <sstream>
#include <iostream>

// ======== Vector Hash Implementations ======== //
template <typename T>
size_t VectorHash<T>::operator()(const std::vector<T>& vec) const {
    size_t seed = vec.size();
    for (const auto& i : vec) {
        seed ^= std::hash<T>()(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

template <typename T>
size_t Vector2DHash<T>::operator()(const std::vector<std::vector<T>>& vec) const {
    size_t seed = vec.size();
    VectorHash<T> hasher;
    for (const auto& v : vec) {
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

// ======== Helper Functions ======== //
std::vector<std::string> Splitbynewline(const std::string& input) {
    std::vector<std::string> lines;
    std::stringstream ss(input);
    std::string line;
    while (std::getline(ss, line)) {
        lines.push_back(line);
    }
    return lines;
}

// ======== Game Implementation ======== //
Game::Game(int n, int N, std::vector<std::vector<int>> pos,
           std::vector<bool> hori, std::vector<int> l)
    : nbCars(n), size(N), positions(pos), horiz(hori), lengths(l),
      free(N, std::vector<bool>(N, true)) {}

bool Game::operator==(const Game& other) const {
    return nbCars == other.nbCars && size == other.size &&
           positions == other.positions && horiz == other.horiz &&
           lengths == other.lengths && free == other.free;
}

size_t Game::GameHash::operator()(const Game& g) const {
    size_t h1 = std::hash<int>()(g.nbCars);
    size_t h2 = std::hash<int>()(g.size);
    Vector2DHash<int> positionsHasher;
    VectorHash<bool> horizHasher;
    VectorHash<int> lengthsHasher;
    Vector2DHash<bool> freeHasher;
    size_t h3 = positionsHasher(g.positions);
    size_t h4 = horizHasher(g.horiz);
    size_t h5 = lengthsHasher(g.lengths);
    size_t h6 = freeHasher(g.free);
    return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4) ^ (h6 << 5);
}

bool Game::update_free() {
    std::vector<std::vector<bool>> new_free(size, std::vector<bool>(size, true));
    for (int i = 0; i < nbCars; i++) {
        int row = positions[i][1] - 1;
        int col = positions[i][0] - 1;
        if (row < 0 || row >= size || col < 0 || col >= size) return false;
        if (horiz[i]) {
            for (int j = col; j < col + lengths[i]; j++) {
                if (j >= size || !new_free[row][j]) return false;
                new_free[row][j] = false;
            }
        } else {
            for (int j = row; j < row + lengths[i]; j++) {
                if (j >= size || !new_free[j][col]) return false;
                new_free[j][col] = false;
            }
        }
    }
    free = new_free;
    return true;
}

Game::Game(std::string s) {
    auto lines = Splitbynewline(s);
    size = std::stoi(lines[0]);
    nbCars = std::stoi(lines[1]);
    for (int i = 2; i < lines.size(); i++) {
        if (lines[i].empty()) continue;
        std::istringstream iss(lines[i]);
        int index, length, row, col;
        char orientation;
        if (iss >> index >> orientation >> length >> row >> col) {
            horiz.push_back(orientation == 'h');
            lengths.push_back(length);
            positions.push_back({row, col});
        } else {
            std::cerr << "Error: Invalid data in line: " << lines[i] << std::endl;
        }
    }
}
