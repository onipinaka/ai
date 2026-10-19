/*
 * =============================================================
 *  SUDOKU SOLVER — Backtracking + Constraint Propagation
 *  Techniques used:
 *    1. Constraint Propagation  : before trying a digit, we check
 *       its row, column, and 3x3 box (arc consistency lite).
 *    2. MRV Heuristic           : always pick the empty cell that
 *       has the FEWEST legal values remaining.
 *    3. Backtracking            : undo bad placements and try next.
 * =============================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <iomanip>

using namespace std;

// ───────────────────────── Constants ─────────────────────────
const int N     = 9;
const int EMPTY = 0;

// ─────────────────────── Global Counters ─────────────────────
int stepCount      = 0;
int backtrackCount = 0;

// ─────────────────────────── Board ───────────────────────────
typedef vector<vector<int>> Board;

// ─────────────────── Pretty-Print the Board ──────────────────
void printBoard(const Board& board, int highlightRow = -1, int highlightCol = -1)
{
    cout << "  +---------+---------+---------+\n";
    for (int r = 0; r < N; ++r)
    {
        if (r == 3 || r == 6)
            cout << "  +---------+---------+---------+\n";

        cout << "  |";
        for (int c = 0; c < N; ++c)
        {
            if (c == 3 || c == 6) cout << " |";
            if (r == highlightRow && c == highlightCol)
                cout << " [" << board[r][c] << "]";
            else if (board[r][c] == EMPTY)
                cout << "  . ";
            else
                cout << "  " << board[r][c] << " ";
        }
        cout << " |\n";
    }
    cout << "  +---------+---------+---------+\n";
}

// ──────────────── Constraint Check (is digit safe?) ──────────
bool isSafe(const Board& board, int row, int col, int digit)
{
    // Row check
    for (int c = 0; c < N; ++c)
        if (board[row][c] == digit) return false;

    // Column check
    for (int r = 0; r < N; ++r)
        if (board[r][col] == digit) return false;

    // 3x3 Box check
    int boxRow = (row / 3) * 3;
    int boxCol = (col / 3) * 3;
    for (int r = boxRow; r < boxRow + 3; ++r)
        for (int c = boxCol; c < boxCol + 3; ++c)
            if (board[r][c] == digit) return false;

    return true;
}

// ─────── Count Legal Values for a Cell (for MRV) ─────────────
int countLegal(const Board& board, int row, int col)
{
    int count = 0;
    for (int d = 1; d <= 9; ++d)
        if (isSafe(board, row, col, d)) ++count;
    return count;
}

// ────────── MRV: Pick Cell with Fewest Legal Values ──────────
pair<int,int> selectCell(const Board& board)
{
    int bestRow = -1, bestCol = -1;
    int minOptions = INT_MAX;

    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            if (board[r][c] == EMPTY)
            {
                int opts = countLegal(board, r, c);
                if (opts < minOptions)
                {
                    minOptions = opts;
                    bestRow = r;
                    bestCol = c;
                }
            }
    return {bestRow, bestCol};
}

// ──────────────────── Core Solver ────────────────────────────
bool solve(Board& board)
{
    // MRV: choose the most constrained empty cell
    auto [row, col] = selectCell(board);

    // Base case: no empty cell left -> puzzle solved
    if (row == -1) return true;

    // Constraint Propagation: collect legal digits for this cell
    vector<int> candidates;
    for (int d = 1; d <= 9; ++d)
        if (isSafe(board, row, col, d))
            candidates.push_back(d);

    ++stepCount;

    if (candidates.empty())
    {
        ++backtrackCount;
        return false;
    }

    for (int digit : candidates)
    {
        board[row][col] = digit;

        if (solve(board))
            return true;

        board[row][col] = EMPTY;
        ++backtrackCount;
    }

    return false;
}

// ─────────────────────────── main ────────────────────────────
int main()
{
    cout << "\n";
    cout << "##############################################\n";
    cout << "#           SUDOKU SOLVER                    #\n";
    cout << "#  Backtracking + MRV + Constraint Prop.     #\n";
    cout << "##############################################\n";

    // Hardcoded sample puzzle (0 = empty)
    Board puzzle = {
        {5, 3, 0,  0, 7, 0,  0, 0, 0},
        {6, 0, 0,  1, 9, 5,  0, 0, 0},
        {0, 9, 8,  0, 0, 0,  0, 6, 0},

        {8, 0, 0,  0, 6, 0,  0, 0, 3},
        {4, 0, 0,  8, 0, 3,  0, 0, 1},
        {7, 0, 0,  0, 2, 0,  0, 0, 6},

        {0, 6, 0,  0, 0, 0,  2, 8, 0},
        {0, 0, 0,  4, 1, 9,  0, 0, 5},
        {0, 0, 0,  0, 8, 0,  0, 7, 9}
    };

    cout << "\n=== UNSOLVED PUZZLE ===\n";
    printBoard(puzzle);

    bool solved = solve(puzzle);

    if (solved)
    {
        cout << "\n=== SOLVED PUZZLE ===\n";
        printBoard(puzzle);
    }
    else
    {
        cout << "\n[!] No solution exists for this puzzle.\n";
    }

    cout << "\n=== STATISTICS ===\n";
    cout << "  Total placement attempts : " << stepCount      << "\n";
    cout << "  Total backtracks         : " << backtrackCount << "\n";
    cout << "==================\n\n";

    return 0;
}
