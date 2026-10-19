/*
 * Tower of Hanoi - State Space Search (BFS)
 * Finds the optimal solution using Breadth-First Search.
 */

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <tuple>
#include <algorithm>
#include <string>

using namespace std;

// State: three pegs, each is a vector of disks (front = bottom, back = top)
typedef tuple<vector<int>, vector<int>, vector<int>> State;

// Prints a peg's disks bottom-to-top
string pegToString(const vector<int>& peg) {
    if (peg.empty()) return "-";
    string s;
    for (int i = 0; i < (int)peg.size(); i++) {
        if (i > 0) s += " ";
        s += to_string(peg[i]);
    }
    return s;
}

void printState(const State& s) {
    cout << "A: " << pegToString(get<0>(s)) << "\n";
    cout << "B: " << pegToString(get<1>(s)) << "\n";
    cout << "C: " << pegToString(get<2>(s)) << "\n";
}

// Get all valid next states from current state, along with move description
vector<pair<State, string>> getSuccessors(const State& state) {
    vector<pair<State, string>> successors;
    const vector<int>* pegs[3] = { &get<0>(state), &get<1>(state), &get<2>(state) };
    char names[3] = { 'A', 'B', 'C' };

    for (int from = 0; from < 3; from++) {
        if (pegs[from]->empty()) continue;
        int disk = pegs[from]->back(); // top disk (smallest exposed)

        for (int to = 0; to < 3; to++) {
            if (from == to) continue;
            // Valid if destination empty or its top disk is larger
            if (pegs[to]->empty() || pegs[to]->back() > disk) {
                vector<int> newPegs[3];
                for (int i = 0; i < 3; i++) newPegs[i] = *pegs[i];
                newPegs[from].pop_back();
                newPegs[to].push_back(disk);

                State newState = make_tuple(newPegs[0], newPegs[1], newPegs[2]);
                string move = string(1, names[from]) + " -> " + string(1, names[to]);
                successors.push_back({ newState, move });
            }
        }
    }
    return successors;
}

int main() {
    cout << "Tower of Hanoi - State Space Search\n";
    cout << "-------------------------------------\n";

    int n;
    cout << "Enter number of disks: ";
    cin >> n;
    cout << "\n";

    if (n <= 0) {
        cout << "Number of disks must be positive.\n";
        return 1;
    }

    // Initial state: all disks on peg A (largest at bottom)
    vector<int> pegA, pegB, pegC;
    for (int i = n; i >= 1; i--) pegA.push_back(i); // e.g. [3, 2, 1] for n=3

    State initial = make_tuple(pegA, pegB, pegC);

    // Goal state: all disks on peg C
    vector<int> goalC;
    for (int i = n; i >= 1; i--) goalC.push_back(i);
    State goal = make_tuple(vector<int>(), vector<int>(), goalC);

    cout << "Initial State:\n";
    printState(initial);
    cout << "\n";

    // BFS state space search
    map<State, pair<State, string>> parent;
    queue<State> q;

    q.push(initial);
    parent[initial] = { initial, "" }; // sentinel

    bool found = false;
    while (!q.empty()) {
        State current = q.front();
        q.pop();

        if (current == goal) {
            found = true;
            break;
        }

        for (auto& [nextState, move] : getSuccessors(current)) {
            if (parent.find(nextState) == parent.end()) {
                parent[nextState] = { current, move };
                q.push(nextState);
            }
        }
    }

    if (!found) {
        cout << "No solution found.\n";
        return 1;
    }

    // Reconstruct path by backtracking from goal to initial
    vector<pair<State, string>> path;
    State cur = goal;
    while (!(cur == initial)) {
        auto& [prev, move] = parent[cur];
        path.push_back({ cur, move });
        cur = prev;
    }
    reverse(path.begin(), path.end());

    cout << "Solution:\n\n";
    int moveNum = 1;
    for (auto& [state, move] : path) {
        cout << "Move " << moveNum++ << ": " << move << "\n";
        printState(state);
        cout << "\n";
    }

    cout << "Total moves: " << path.size() << "\n";
    cout << "Minimum possible moves for " << n << " disk(s): "
         << ((1 << n) - 1) << "\n";

    return 0;
}
