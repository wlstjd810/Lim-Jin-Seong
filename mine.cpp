#include <iostream>
#include <vector>
#include <queue>
#include <ctime>
#include <cstdlib>

using namespace std;

const int ROWS = 9;
const int COLS = 9;
const int MINES = 10;

vector<vector<char>> board(ROWS, vector<char>(COLS, '0'));
vector<vector<char>> visible(ROWS, vector<char>(COLS, '-'));
vector<vector<bool>> mine_locations(ROWS, vector<bool>(COLS, false));

void initialize_board() {
    srand(time(0));
    int placed_mines = 0;
    while (placed_mines < MINES) {
        int row = rand() % ROWS;
        int col = rand() % COLS;
        if (!mine_locations[row][col]) {
            mine_locations[row][col] = true;
            board[row][col] = 'X';
            placed_mines++;
        }
    }

    // 숫자 계산
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (board[r][c] == 'X') continue;
            int mine_count = 0;
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    int nr = r + dr, nc = c + dc;
                    if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && board[nr][nc] == 'X') {
                        mine_count++;
                    }
                }
            }
            board[r][c] = mine_count + '0';
        }
    }
}

void display_board() {
    cout << "  ";
    for (int c = 0; c < COLS; c++) {
        cout << c << " ";
    }
    cout << "\n";
    for (int r = 0; r < ROWS; r++) {
        cout << r << " ";
        for (int c = 0; c < COLS; c++) {
            cout << visible[r][c] << " ";
        }
        cout << "\n";
    }
}

void reveal_cell(int row, int col) {
    queue<pair<int, int>> q;
    vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false));

    q.push({row, col});
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        if (r < 0 || r >= ROWS || c < 0 || c >= COLS || visible[r][c] != '-' || visited[r][c]) {
            continue;
        }

        visited[r][c] = true;
        visible[r][c] = board[r][c];

        if (board[r][c] == '0') {
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    if (dr != 0 || dc != 0) {
                        q.push({r + dr, c + dc});
                    }
                }
            }
        }
    }
}

bool is_game_won() {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (visible[r][c] == '-' && board[r][c] != 'X') {
                return false;
            }
        }
    }
    return true;
}

int main() {
    initialize_board();
    int row, col;
    bool game_over = false;

    cout << "지뢰찾기 게임에 오신 것을 환영합니다!\n";
    while (!game_over) {
        display_board();
        cout << "입력 (행 열): ";
        cin >> row >> col;

        // 입력 유효성 검사
        if (cin.fail() || row < 0 || row >= ROWS || col < 0 || col >= COLS) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "잘못된 입력입니다! 유효한 좌표를 입력하세요.\n";
            continue;
        }

        if (visible[row][col] != '-') {
            cout << "이미 열려 있는 칸입니다. 다른 좌표를 선택하세요.\n";
            continue;
        }

        if (board[row][col] == 'X') {
            cout << "지뢰를 밟았습니다! 게임 오버!\n";
            display_board();
            game_over = true;
        } else {
            reveal_cell(row, col);
            if (is_game_won()) {
                cout << "축하합니다! 모든 지뢰를 피했습니다!\n";
                display_board();
                game_over = true;
            }
        }
    }

    return 0;
}
