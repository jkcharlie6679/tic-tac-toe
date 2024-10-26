#include <FL/Fl_Window.H>
#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class TicTacToe {
private:
    vector<vector<string>> board;
    queue<pair<int, int>> positionQ;
    string currentPlayer;
    int n;
public:
    TicTacToe(int n) : n(n), board(n, vector<string>(n, "")), currentPlayer("X") {}

    string getCurrentPlayer() const { 
        return currentPlayer;
    }

    pair<int, int> getRelease() const {
        if (positionQ.size() == 2 * n) {
            auto res = positionQ.front();
            return res;
        }
        return {-1, -1};
    }

    pair<int, int> release() {
        if (positionQ.size() > 2 * n) {
            auto res = positionQ.front();
            positionQ.pop();
            board[res.first][res.second] = "";
            return res;
        }
        return {-1, -1};
    }

    bool makeMove(int row, int col) {
        if (board[row][col] == "") {
            board[row][col] = currentPlayer;
            positionQ.push({row, col});
            return true;
        }
        return false;
    }

    bool checkLineOrRow(int l, int mode) const {
        for (int i = 0; i < n; i++) {
            if (mode == 0) {
                if (board[l][i] != currentPlayer)
                    return false;
            } else {
                if (board[i][l] != currentPlayer)
                    return false;
            }
        }
        return true;
    }

    bool checkDiagonal(int mode) const {
        for (int i = 0; i < n && mode == 0; ++i) {
            if (board[i][i] != currentPlayer)
                return false;
        }
        for (int i = 0; i < n && mode == 1; ++i) {
            if (board[i][n - i - 1] != currentPlayer)
                return false;
        }
        return true;
    }

    bool checkWin() const {
        for (int i = 0; i < n; ++i) {
            if (checkLineOrRow(i, 0) || checkLineOrRow(i, 1))
                return true;
        }
        if (checkDiagonal(0) || checkDiagonal(1))
            return true;
        return false;
    }

    string& getCell(int row, int col) {
        return board[row][col];
    }

    void setCell(int row, int col, string c) {
        board[row][col] = c;
    }
    void switchPlayer() {

        currentPlayer = (currentPlayer == "X") ? "O" : "X";
    }

    void reset() {
        for (auto& row : board)
            for (auto& cell : row)
                cell = "";
        currentPlayer = "X";
        while (!positionQ.empty())
            positionQ.pop();
    }
};

class TicTacToeWindow : public Fl_Window {
private:
    TicTacToe game;
    vector<vector<Fl_Button*>> buttons;
    int n;
    static void button_cb(Fl_Widget* widget, void* data) {
        TicTacToeWindow* window = (TicTacToeWindow*)data;
        window->handleButtonClick(widget);
    }

    void handleButtonClick(Fl_Widget* widget) {
        int row = -1, col = -1;

        // Find which button was clicked
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (buttons[i][j] == widget) {
                    row = i;
                    col = j;
                    i = n;
                    break;
                }
            }
        }

        cout << "Button clicked at: (" << row << ", " << col << ")\n";

        if (game.makeMove(row, col)) {
            auto release = game.release();
            int rRow = release.first, rCol = release.second;
            if (rRow != -1) {
                buttons[rRow][rCol]->label("");
                buttons[rRow][rCol]->redraw();
            }

            buttons[row][col]->label(game.getCell(row, col).c_str());
            buttons[row][col]->redraw();

            auto change = game.getRelease();
            int cRow = change.first, cCol = change.second;
            if (cRow != -1) {
                game.setCell(cRow, cCol, "(" + game.getCell(cRow, cCol) + ")");
                buttons[cRow][cCol]->redraw();
            }
            
            
            if (game.checkWin()) {
                fl_message("Player %s wins!", game.getCurrentPlayer().c_str());
                resetGame();
            } else {
                game.switchPlayer();
            }
        }
    }

    void resetGame() {
        game.reset();
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                buttons[i][j]->label("");
            }
        }
    }
public:
    TicTacToeWindow(int n, int width, int height, const char* title = nullptr) : n(n), buttons(n, vector<Fl_Button*>(n)), Fl_Window(width, height, title), game(n) {
        this->begin();
        for (int i = 0; i < this->n; ++i) {
            for (int j = 0; j < this->n; ++j) {
                buttons[i][j] = new Fl_Button(100 * j, 100 * i, 100, 100, "");
                buttons[i][j]->labelsize(36);
                buttons[i][j]->callback(button_cb, this);
            }
        }
        this->end();
    }
};

int main(int argc, char** argv) {
    int n = 3;
    TicTacToeWindow win(n, n * 100, n * 100, "Tic Tac Toe");
    win.show(argc, argv);
    return Fl::run();
}
