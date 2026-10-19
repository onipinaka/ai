/*
 * Water Jug Problem - DFS Solution
 * Given two jugs of capacity CAP_A and CAP_B liters,
 * find a sequence of operations to measure TARGET liters.
 *
 * Operations:
 *  1. Fill jug A
 *  2. Fill jug B
 *  3. Empty jug A
 *  4. Empty jug B
 *  5. Pour A -> B
 *  6. Pour B -> A
 */

#include <iostream>
#include <set>
#include <vector>
#include <stack>
using namespace std;

const int CAP_A  = 4;   // capacity of jug A (liters)
const int CAP_B  = 3;   // capacity of jug B (liters)
const int TARGET = 2;   // desired measurement (liters)

struct State {
    int a, b;           // current water in jug A and jug B
    string action;      // operation that led to this state
};

bool dfs() {
    set<pair<int,int>> visited;
    stack<State> stk;

    // Start: both jugs empty
    stk.push({0, 0, "Initial State"});

    while (!stk.empty()) {
        State cur = stk.top(); stk.pop();

        // Skip already visited
        if (visited.count({cur.a, cur.b})) continue;
        visited.insert({cur.a, cur.b});

        // Print current step
        cout << "  Action : " << cur.action << "\n";
        cout << "  State  : (A=" << cur.a << ", B=" << cur.b << ")\n\n";

        // Goal check
        if (cur.a == TARGET || cur.b == TARGET) {
            cout << "==> Goal reached! " << TARGET << "L measured.\n";
            return true;
        }

        // Generate next states (push in reverse priority order)
        // 6. Pour B -> A
        {
            int pour = min(cur.b, CAP_A - cur.a);
            State ns = {cur.a + pour, cur.b - pour, "Pour B -> A"};
            if (!visited.count({ns.a, ns.b})) stk.push(ns);
        }
        // 5. Pour A -> B
        {
            int pour = min(cur.a, CAP_B - cur.b);
            State ns = {cur.a - pour, cur.b + pour, "Pour A -> B"};
            if (!visited.count({ns.a, ns.b})) stk.push(ns);
        }
        // 4. Empty B
        {
            State ns = {cur.a, 0, "Empty B"};
            if (!visited.count({ns.a, ns.b})) stk.push(ns);
        }
        // 3. Empty A
        {
            State ns = {0, cur.b, "Empty A"};
            if (!visited.count({ns.a, ns.b})) stk.push(ns);
        }
        // 2. Fill B
        {
            State ns = {cur.a, CAP_B, "Fill B"};
            if (!visited.count({ns.a, ns.b})) stk.push(ns);
        }
        // 1. Fill A
        {
            State ns = {CAP_A, cur.b, "Fill A"};
            if (!visited.count({ns.a, ns.b})) stk.push(ns);
        }
    }

    cout << "No solution found.\n";
    return false;
}

int main() {
    cout << "==============================\n";
    cout << "   Water Jug Problem (DFS)\n";
    cout << "==============================\n";
    cout << "Jug A capacity : " << CAP_A << "L\n";
    cout << "Jug B capacity : " << CAP_B << "L\n";
    cout << "Target         : " << TARGET << "L\n";
    cout << "------------------------------\n\n";

    dfs();

    cout << "==============================\n";
    return 0;
}
