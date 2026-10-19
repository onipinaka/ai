#include <iostream>
#include <climits>
using namespace std;

char board[3][3];

void initBoard() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = ' ';
}

void printBoard() {
    cout << "\n";
    cout << "  +-------+-------+-------+\n";
    for (int i = 0; i < 3; i++) {
        cout << "  |       |       |       |\n";
        cout << "  |   " << board[i][0] << "   |   " << board[i][1] << "   |   " << board[i][2] << "   |\n";
        cout << "  |       |       |       |\n";
        cout << "  +-------+-------+-------+\n";
    }
    cout << "\n";
}

char checkWinner() {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') return board[i][0];
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ') return board[0][i];
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') return board[0][0];
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') return board[0][2];
    return ' ';
}

bool isFull() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == ' ') return false;
    return true;
}

int minimax(bool isMaximizing) {
    char winner = checkWinner();
    if (winner == 'O') return +10;
    if (winner == 'X') return -10;
    if (isFull()) return 0;

    if (isMaximizing) {
        int best = INT_MIN;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (board[i][j] == ' ') {
                    board[i][j] = 'O';
                    best = max(best, minimax(false));
                    board[i][j] = ' ';
                }
        return best;
    } else {
        int best = INT_MAX;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (board[i][j] == ' ') {
                    board[i][j] = 'X';
                    best = min(best, minimax(true));
                    board[i][j] = ' ';
                }
        return best;
    }
}

void aiMove() {
    int bestVal = INT_MIN, bestRow = -1, bestCol = -1;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == ' ') {
                board[i][j] = 'O';
                int val = minimax(false);
                board[i][j] = ' ';
                if (val > bestVal) { bestVal = val; bestRow = i; bestCol = j; }
            }
    board[bestRow][bestCol] = 'O';
    cout << "  >> AI (O) played at position (" << bestRow+1 << ", " << bestCol+1 << ")\n";
}

int main() {
    cout << "\n";
    cout << "  ╔══════════════════════════════════╗\n";
    cout << "  ║      TIC-TAC-TOE  w/ MINIMAX     ║\n";
    cout << "  ║   You = X  |  AI Agent = O       ║\n";
    cout << "  ║   Enter row & col (1-3) to play   ║\n";
    cout << "  ╚══════════════════════════════════╝\n";

    initBoard();
    printBoard();

    while (true) {
        // --- Player Move ---
        int r, c;
        while (true) {
            cout << "  Your move (row col): ";
            cin >> r >> c;
            r--; c--;
            if (r >= 0 && r < 3 && c >= 0 && c < 3 && board[r][c] == ' ') break;
            cout << "  [!] Invalid move. Try again.\n";
        }
        board[r][c] = 'X';
        printBoard();

        if (checkWinner() == 'X') { cout << "  ★  You win! Congratulations!\n\n"; break; }
        if (isFull())              { cout << "  ●  It's a draw!\n\n"; break; }

        // --- AI Move ---
        cout << "  Thinking...\n";
        aiMove();
        printBoard();

        if (checkWinner() == 'O') { cout << "  ✗  AI wins! Better luck next time.\n\n"; break; }
        if (isFull())              { cout << "  ●  It's a draw!\n\n"; break; }
    }

    return 0;
}
