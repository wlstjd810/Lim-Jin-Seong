#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

const int ROWS = 9;
const int COLS = 9;
const int MINES = 10;

class Minesweeper {
private:
    vector<vector<char>> board;
    vector<vector<char>> visible;
    vector<vector<int>> mine_locations;

    void initialize_board() {
        board = vector<vector<char>>(ROWS, vector<char>(COLS, '0'));
        visible = vector<vector<char>>(ROWS, vector<char>(COLS, '-'));
        mine_locations = vector<vector<int>>(ROWS, vector<int>(COLS, 0));
    }

    void place_mines() {
        srand(static_cast<unsigned>(time(0)));
        int count = 0;
        while (count < MINES) {
            int row = rand() % ROWS;
            int col = rand() % COLS;
            if (mine_locations[row][col] == 0) {
                mine_locations[row][col] = 1;
                board[row][col] = 'X';
                count++;
            }
        }
    }

    void calculate_numbers() {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (board[i][j] == 'X') continue;
                int mine_count = 0;
                for (int x = -1; x <= 1; x++) {
                    for (int y = -1; y <= 1; y++) {
                        int ni = i + x, nj = j + y;
                        if (ni >= 0 && ni < ROWS && nj >= 0 && nj < COLS && board[ni][nj] == 'X') {
                            mine_count++;
                        }
                    }
                }
                board[i][j] = mine_count + '0';
            }
        }
    }

public:
    Minesweeper() {
        initialize_board();
        place_mines();
        calculate_numbers();
    }

    void display_board() {
        cout << "  ";
        for (int i = 0; i < COLS; i++) {
            cout << i << " ";
        }
        cout << endl;
        for (int i = 0; i < ROWS; i++) {
            cout << i << " ";
            for (int j = 0; j < COLS; j++) {
                cout << visible[i][j] << " ";
            }
            cout << endl;
        }
    }

    int reveal_cell(int row, int col) {
        if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
            cout << "잘못된 입력입니다! 유효한 좌표를 입력하세요.\n";
            return 0;
        }
        if (visible[row][col] != '-') {
            cout << "이미 열려 있는 칸입니다. 다른 좌표를 선택하세요.\n";
            return 0;
        }
        visible[row][col] = board[row][col];
        if (board[row][col] == 'X') {
            return -1; // 게임 오버
        } else if (board[row][col] == '0') {
            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    reveal_cell(row + x, col + y);
                }
            }
        }
        return 1; // 정상 진행
    }

    bool check_win() {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (visible[i][j] == '-' && board[i][j] != 'X') {
                    return false;
                }
            }
        }
        return true;
    }
};

int main() {
    Minesweeper game;

    int row, col, result;
    cout << "지뢰찾기 게임에 오신 것을 환영합니다!\n";
    cout << "좌표를 입력하여 지뢰를 찾으세요 (예: 0 0).\n";

    while (true) {
        game.display_board();
        cout << "입력 (행 열): ";
        cin >> row >> col;

        result = game.reveal_cell(row, col);
        if (result == -1) { // 지뢰를 밟은 경우
            cout << "지뢰를 밟았습니다! 게임 오버!\n";
            game.display_board(); // 마지막 상태 표시
            break;
        } else if (game.check_win()) { // 모든 칸을 열었는지 확인
            cout << "축하합니다! 모든 지뢰를 피했습니다!\n";
            game.display_board(); // 마지막 상태 표시
            break;
        }
    }

    return 0;
}
