#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <algorithm>
using namespace std;

// Grid size
const int ROWS = 5, COLS = 5;

// 4-directional movement: Down, Right, Up, Left
int dr[] = {1, 0, -1, 0};
int dc[] = {0, 1, 0, -1};

// ─────────────────────────────────────────
// BFS: Returns the shortest path from start to dest
// avoiding cells marked 1 (obstacles)
// ─────────────────────────────────────────
vector<pair<int,int>> bfs(int grid[ROWS][COLS],
                          pair<int,int> start,
                          pair<int,int> dest) {
    // visited tracks if a cell was already explored
    bool visited[ROWS][COLS] = {};
    // parent stores where we came from for each cell
    map<pair<int,int>, pair<int,int>> parent;

    queue<pair<int,int>> q;
    q.push(start);
    visited[start.first][start.second] = true;
    parent[start] = {-1, -1};  // start has no parent

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        // Reached destination — stop
        if (make_pair(r, c) == dest) break;

        // Explore all 4 neighbours
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            // Check bounds, not obstacle, not visited
            if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS
                && grid[nr][nc] == 0 && !visited[nr][nc]) {
                visited[nr][nc] = true;
                parent[{nr, nc}] = {r, c};
                q.push({nr, nc});
            }
        }
    }

    // Reconstruct path by walking back from dest to start
    vector<pair<int,int>> path;
    if (!visited[dest.first][dest.second]) return path; // no path

    pair<int,int> cur = dest;
    while (cur != make_pair(-1, -1)) {
        path.push_back(cur);
        cur = parent[cur];
    }
    reverse(path.begin(), path.end());
    return path;
}

// ─────────────────────────────────────────
// Print the grid with path highlighted
// S = Start, D = Destination, # = Obstacle
// * = path cell, . = empty cell
// ─────────────────────────────────────────
void printGrid(int grid[ROWS][COLS],
               vector<pair<int,int>>& path,
               pair<int,int> start,
               pair<int,int> dest) {
    // Mark path cells in a lookup grid
    bool onPath[ROWS][COLS] = {};
    for (auto& p : path) onPath[p.first][p.second] = true;

    string border = "+---+---+---+---+---+";
    for (int r = 0; r < ROWS; r++) {
        cout << border << "\n";
        for (int c = 0; c < COLS; c++) {
            cout << "| ";
            if (make_pair(r, c) == start)       cout << "S";
            else if (make_pair(r, c) == dest)   cout << "D";
            else if (grid[r][c] == 1)            cout << "#";
            else if (onPath[r][c])               cout << "*";
            else                                 cout << ".";
            cout << " ";
        }
        cout << "|\n";
    }
    cout << border << "\n";
}

// ─────────────────────────────────────────
// Run one example: print path + grid
// ─────────────────────────────────────────
void runExample(int num, int grid[ROWS][COLS],
                pair<int,int> start, pair<int,int> dest,
                string title) {
    cout << "Example " << num << ": " << title << "\n";
    cout << "Start: (" << start.first << ", " << start.second << ")  "
         << "Destination: (" << dest.first << ", " << dest.second << ")\n\n";

    vector<pair<int,int>> path = bfs(grid, start, dest);

    if (path.empty()) {
        cout << "No path found!\n\n";
        return;
    }

    // Print shortest path
    cout << "Shortest Path: ";
    for (int i = 0; i < (int)path.size(); i++) {
        cout << "(" << path[i].first << ", " << path[i].second << ")";
        if (i + 1 < (int)path.size()) cout << " -> ";
    }
    cout << "\nTotal Steps: " << path.size() - 1 << "\n\n";

    cout << "Grid Representation:\n";
    printGrid(grid, path, start, dest);
    cout << "\n";
}

// ─────────────────────────────────────────
// MAIN — hardcoded grids (0 = free, 1 = obstacle)
// Change grids/start/dest here if needed
// ─────────────────────────────────────────
int main() {
    cout << "ROBOT PATH PLANNING USING BFS\n";
    cout << string(40, '=') << "\n\n";

    // ── Example 1 ──────────────────────────
    // Obstacles block row 1 (cols 1-3) and row 3 (cols 1-2)
    int grid1[ROWS][COLS] = {
        {0, 0, 0, 0, 0},
        {1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}
    };
    pair<int,int> s1 = {0, 0}, d1 = {4, 4};
    runExample(1, grid1, s1, d1, "Standard BFS Grid Path");

    // ── Example 2 ──────────────────────────
    // Different obstacle layout
    int grid2[ROWS][COLS] = {
        {0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}
    };
    pair<int,int> s2 = {0, 0}, d2 = {4, 4};
    runExample(2, grid2, s2, d2, "Alternate Path with Obstacles");

    return 0;
}
