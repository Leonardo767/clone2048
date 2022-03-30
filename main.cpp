#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h> 

class SwipeBoard {
    // handles swipe commands and merges powers of 2 (actual value converted later)
private:
    int rows;
    int cols;

public:
    std::vector<std::vector<int>> board;
    std::vector<int> merge_line(std::vector<int> curr_line) {
        // function invariant: within this helper function, we will always merge to front of vector
        // [1, 0, 0, 1, 0, 1] -> [2, 1, 0, 0, 0, 0]
        // curr_line             merged_line

        int p0 = 0;
        std::vector<int> merged_line;
        // push p0 to next empty slot
        // while (p0 < curr_line.size() && curr_line[p0]) {
        //     merged_line.push_back(curr_line[p0]);
        //     p0++;
        // }
        bool just_merged = false;
        while (p0 < curr_line.size()) {
            int ele = curr_line[p0];
            // only operate on non-empty
            if (!ele);
            // merge equal elements, but only once (bool flip)
            else if (!just_merged && merged_line.size() && ele == merged_line.back()) {
                merged_line[merged_line.size() - 1] += 1;
                just_merged = true;
            } 
            // push back without merging, either bc we just merged or not equal
            else {
                merged_line.push_back(ele);
                just_merged = false;
            }
            p0++;
        }
        // resize quickly, default values now 0
        merged_line.resize(curr_line.size());
        return merged_line;
    }

    void swipe(char direction) {
    // direction only matters for mapping board to a temp line vector and then back to board
    std::vector<int> temp_line; // used to rotate columns to fit this->merge_line()
    switch (direction) {
        case 'a': // left, merge row by row
            for (int r=0; r<board.size(); r++) {
                board[r] = merge_line(board[r]);
            }
            break;
        case 'd': // right, merge row by row reversed
            for (int r=0; r<board.size(); r++) {
                std::reverse(board[r].begin(), board[r].end());
                board[r] = merge_line(board[r]);
                std::reverse(board[r].begin(), board[r].end());
            }
            break;
        case 'w': // up, merge col by col
            for (int c=0; c<board[0].size(); c++) {
                temp_line.clear();
                for (int r=0; r<board.size(); r++) {
                    temp_line.push_back(board[r][c]);
                }
                temp_line = merge_line(temp_line);
                for (int r=0; r<board.size(); r++) {
                    board[r][c] = temp_line[r];
                }
            }
            break;
        case 's': // down, merge col by col reversed
            for (int c=0; c<board[0].size(); c++) {
                temp_line.clear();
                for (int r=board.size()-1; r>=0; r--) {
                    temp_line.push_back(board[r][c]);
                }
                temp_line = merge_line(temp_line);
                for (int r=board.size()-1; r>=0; r--) {
                    board[r][c] = temp_line[temp_line.size() - 1 - r];
                }
            }
            break;
       }
    }

    void spawn_random(int val) {
        // game must not be lost
        // can optimize by using 1D rand and modulo into 2D
        srand(time(NULL));
        int r = rand() % rows;
        int c = rand() % cols;
        while (board[r][c]) {
            r = rand() % rows;
            c = rand() % cols;
        }
        board[r][c] = val;
    }
    
    SwipeBoard(int n_rows, int n_cols) : rows(n_rows), cols(n_cols) {
        board.resize(rows, std::vector<int>(cols, 0));
    }
};


class Game2048 {
private:
    int win_base_pow;
    int init_base_pow;
    SwipeBoard* currBoard;

    bool is_win() {
        // check if 2^highest_pow == win_val
        return false;
    }

    int log2(int x) {
        int pow=1;
        while (x > 2) {
            x /= 2;
            pow += 1;
        } 
        return pow;
    }

    int exp2(int y) {
        // only needed for display
        // maintain table for small y at compile time for quick display

        // calculate for larger values
        int z = y;
        return z;
    }

public:
    bool is_won;
    bool is_lost;
    Game2048(int n_rows=4, int n_cols=4, int win_val=2048, int init_base_pow=1) {
        // validate inputs
        win_base_pow = log2(win_val);
        init_base_pow = init_base_pow;
        is_won = false;
        is_lost = false;

        // build board
        currBoard = new SwipeBoard(n_rows, n_cols);
        currBoard->spawn_random(1);
    }

    void check_game_state() {
        is_lost = true;
        for (auto row : currBoard->board) {
            for (auto ele : row) {
                if (!ele) is_lost = false;
                if (ele == win_base_pow) is_won = true;
            }
        }
    }

    void step(char dir) {
        // exposed step so that main() can control execution along with user input
        // swipe in direction
        currBoard->swipe(dir);
        // determine if win/lose
        check_game_state();
        // randomly place new base 1 on board
        if (!is_lost && !is_won) currBoard->spawn_random(1);
    }

    void display() {
        // expose UI function
        for (auto row : currBoard->board) {
            for (auto ele : row) {
                std::cout << exp2(ele) << " ";
            }
            std::cout << std::endl;
        }
    }

};


int main() {
    // std::cout << "hi" << std::endl;
    /*
    std::vector<int> test = {1, 0, 0, 1, 0, 1, 1, 1};
    auto test_board = SwipeBoard(4, 4);
    auto test_out = test_board.merge_line(test);
    for (auto i : test_out) {
        std::cout << i << " ";
    }
    */
    auto test_game = Game2048();
    char dir;
    while (!test_game.is_won && !test_game.is_lost) {
        test_game.display();
        std::cout << "Direction: ";
        std::cin >> dir;
        test_game.step(dir);
    }
    if (test_game.is_won) {
        std::cout << "YOU WIN!";
    } else {
        std::cout << "GAME OVER";
    }

    return 0;
}
