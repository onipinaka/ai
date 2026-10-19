
#include <iostream>
using namespace std;

int pos[8];

bool safe(int row, int col) {
    for (int i = 0; i < row; i++)
        if (pos[i] == col || abs(pos[i] - col) == row - i)
            return false;
    return true;
}

bool solve(int row) {
    if (row == 8) return true;

    for (int col = 0; col < 8; col++) {
        if (safe(row, col)) {
            pos[row] = col;
            if (solve(row + 1)) return true;
        }
    }
    return false;
}

int main() {
    if (solve(0)) {
        cout << "Solution Found!\n\n";
        cout << "Queen Positions (Row, Column):\n";

        for (int i = 0; i < 8; i++)
            cout << "(" << i << "," << pos[i] << ") ";

        cout << "\n\nChessboard:\n";

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++)
                cout << (pos[i] == j ? " Q " : " . ");
            cout << "\n";
        }
    } else {
        cout << "No solution found.\n";
    }

    return 0;
}
