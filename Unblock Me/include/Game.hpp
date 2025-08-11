#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <string>
#include <functional>


template <typename T>
struct VectorHash {
    size_t operator()(const std::vector<T>& vec) const;
};

template <typename T>
struct Vector2DHash {
    size_t operator()(const std::vector<std::vector<T>>& vec) const;
};

std::vector<std::string> Splitbynewline(const std::string& input);


class Game {
public:
    int nbCars; 
    int size;   
    std::vector<std::vector<int>> positions; 
    std::vector<bool> horiz;            
    std::vector<int> lengths;           
    std::vector<std::vector<bool>> free;     

    Game(int n, int N, std::vector<std::vector<int>> pos,
         std::vector<bool> hori, std::vector<int> l);
    Game(std::string s);

    bool operator==(const Game& other) const;

    struct GameHash {
        size_t operator()(const Game& g) const;
    };

    bool update_free();
};

#endif
