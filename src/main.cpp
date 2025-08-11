// main.cpp
#include "Game.hpp"
#include "State.hpp"

#include <iostream>
#include <fstream>
#include <iterator>
#include <chrono>
#include <vector>
#include <string>

// ======== Helper: Select Heuristics ======== //
std::vector<std::string> get_heuristic_selection() {
    std::vector<std::string> heuristic_methods = { "trivial", "blocking_cars", "distance_to_exit" };
    std::vector<std::string> selected_heuristics;
    std::string input;
    std::cout << "Choose heuristics to apply (1 for yes, 0 for no):" << std::endl;
    std::cout << "1. trivial" << std::endl;
    std::cout << "2. blocking_cars" << std::endl;
    std::cout << "3. distance_to_exit" << std::endl;
    std::cout << "Enter a 3-digit code (e.g., '101'): ";
    std::cin >> input;
    // Basic validation (if user entered fewer chars, we avoid UB)
    for (int i = 0; i < 3 && i < static_cast<int>(input.size()); ++i) {
        if (input[i] == '1') selected_heuristics.push_back(heuristic_methods[i]);
    }
    return selected_heuristics;
}

// ======== Run on a Specific Board ======== //
void run_on_specific_board() {
    std::string board_data;
    int use_heuristics;
    std::cout << "Enter the board game data (multiple lines allowed). Finish with an empty line or EOF:" << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear leftover newline
    // Read entire input until a single empty line (user must press Enter twice)
    std::string line;
    std::string full;
    while (true) {
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) break;
        full += line + '\n';
    }
    board_data = full;

    std::cout << "Use heuristics? (1=yes, 0=no): ";
    std::cin >> use_heuristics;

    Game g(board_data);
    if (!g.update_free()) {
        std::cerr << "Invalid board configuration.\n";
        return;
    }
    State init_state(g);

    if (use_heuristics) {
        auto selected_heuristics = get_heuristic_selection();
        for (const auto& method : selected_heuristics) {
            auto start = std::chrono::high_resolution_clock::now();
            auto shortest_path = init_state.shortest_path_with_heuristic(method);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;

            std::cout << "Method: " << method << "\n";
            if (shortest_path.empty() || (shortest_path.size() == 1 && shortest_path[0].empty())) {
                std::cout << "No solution found.\n";
            } else {
                std::cout << "Shortest path:\n";
                for (const auto& move : shortest_path) {
                    if (move.size() >= 3)
                        std::cout << move[0] << " >> (" << move[1] << "," << move[2] << ")\n";
                }
                std::cout << "Number of moves: " << shortest_path.size() << "\n";
            }
            std::cout << "Execution time: " << duration.count() << " seconds\n\n";
        }
    } else {
        auto start = std::chrono::high_resolution_clock::now();
        auto shortest_path = init_state.shortest_path();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        if (shortest_path.empty() || (shortest_path.size() == 1 && shortest_path[0].empty())) {
            std::cout << "No solution found.\n";
        } else {
            std::cout << "Shortest path:\n";
            for (const auto& move : shortest_path) {
                if (move.size() >= 3)
                    std::cout << move[0] << " >> (" << move[1] << "," << move[2] << ")\n";
            }
            std::cout << "Number of moves: " << shortest_path.size() << "\n";
        }
        std::cout << "Execution time: " << duration.count() << " seconds\n";
    }
}

// ======== Run on Database Cases ======== //
void run_on_database_cases() {
    int start_case, end_case;
    char delimiter;
    std::cout << "Enter the range of cases to test (e.g., '4 > 18'): ";
    std::cin >> start_case >> delimiter >> end_case;

    if (start_case < 1 || end_case < start_case) {
        std::cerr << "Invalid range. Please ensure the start case is <= end case.\n";
        return;
    }

    int use_heuristics;
    std::cout << "Use heuristics? (1=yes, 0=no): ";
    std::cin >> use_heuristics;

    std::vector<std::string> selected_heuristics;
    if (use_heuristics == 1) selected_heuristics = get_heuristic_selection();

    for (int i = start_case; i <= end_case; ++i) {
        std::string filename = (i < 10) ? ("..data/ExRushHour/GameP0" + std::to_string(i) + ".txt")
                                        : ("..data/ExRushHour/GameP" + std::to_string(i) + ".txt");

        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Error opening file: " << filename << ". Skipping this case.\n";
            continue;
        }

        std::string s((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        std::cout << "Puzzle: " << filename << "\n" << s << "\n";

        try {
            Game g(s);
            if (!g.update_free()) {
                std::cerr << "Invalid game configuration in file: " << filename << ". Skipping this case.\n";
                continue;
            }

            State init_state(g);
            init_state.showState();
            std::cout << std::endl;

            if (use_heuristics == 1) {
                for (const auto& method : selected_heuristics) {
                    auto start = std::chrono::high_resolution_clock::now();
                    auto shortest_path = init_state.shortest_path_with_heuristic(method);
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> duration = end - start;

                    std::cout << "Method: " << method << "\n";
                    if (shortest_path.empty() || (shortest_path.size() == 1 && shortest_path[0].empty())) {
                        std::cout << "No solution found.\n";
                    } else {
                        std::cout << "Shortest path:\n";
                        for (const auto& move : shortest_path) {
                            if (move.size() >= 3)
                                std::cout << move[0] << " >> (" << move[1] << "," << move[2] << ")\n";
                        }
                        std::cout << "Number of moves: " << shortest_path.size() << "\n";
                    }
                    std::cout << "Execution time: " << duration.count() << " seconds\n\n";
                }
            } else {
                auto start = std::chrono::high_resolution_clock::now();
                auto shortest_path = init_state.shortest_path();
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;

                if (shortest_path.empty() || (shortest_path.size() == 1 && shortest_path[0].empty())) {
                    std::cout << "No solution found.\n";
                } else {
                    std::cout << "Shortest path:\n";
                    for (const auto& move : shortest_path) {
                        if (move.size() >= 3)
                            std::cout << move[0] << " >> (" << move[1] << "," << move[2] << ")\n";
                    }
                    std::cout << "Number of moves: " << shortest_path.size() << "\n";
                }
                std::cout << "Execution time: " << duration.count() << " seconds\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "An error occurred while processing file: " << filename << ". Error: " << e.what() << "\n";
        }

        std::cout << "----------------------------------------\n";
    }
}

// ======== main ======== //
int main() {
    int choice;
    std::cout << "Choose how to run the program:\n";
    std::cout << "0 - Run on a specific board\n";
    std::cout << "1 - Run on cases from the database\n";
    std::cout << "Enter your choice (0 or 1): ";
    std::cin >> choice;

    if (choice == 0) {
        run_on_specific_board();
    } else if (choice == 1) {
        run_on_database_cases();
    } else {
        std::cout << "Invalid choice. Exiting...\n";
    }
    return 0;
}

