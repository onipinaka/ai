/*
 * Adversarial Search with Alpha-Beta Pruning
 * Implements a generic game tree with MAX/MIN nodes,
 * interactive input, step-by-step trace, and result summary.
 */

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

// ──────────────────────────────────────────────
// Node structure
// ──────────────────────────────────────────────
struct Node {
    string name;
    bool   isMax;          // true = MAX, false = MIN
    bool   isTerminal;
    int    terminalValue;
    vector<Node*> children;
};

// ──────────────────────────────────────────────
// Globals for bookkeeping
// ──────────────────────────────────────────────
vector<string> visitedNodes;
vector<string> prunedNodes;

struct BestMove {
    string nodeFrom;
    string nodeTo;
};
vector<BestMove> bestMoves;

// ──────────────────────────────────────────────
// Helper: print indentation
// ──────────────────────────────────────────────
string indent(int depth) {
    return string(depth * 4, ' ');
}

// ──────────────────────────────────────────────
// Helper: format infinity values for display
// ──────────────────────────────────────────────
string fmtVal(int v) {
    if (v <= numeric_limits<int>::min() + 1) return "-inf";
    if (v >= numeric_limits<int>::max() - 1) return "inf";
    return to_string(v);
}

// ──────────────────────────────────────────────
// Alpha-Beta search
// ──────────────────────────────────────────────
int alphaBeta(Node* node, int alpha, int beta, int depth) {

    visitedNodes.push_back(node->name);

    if (node->isMax)
        cout << indent(depth) << "MAX Node: " << node->name << "\n";
    else
        cout << indent(depth) << "MIN Node: " << node->name << "\n";

    cout << indent(depth) << "  Alpha = " << fmtVal(alpha)
         << "   Beta = " << fmtVal(beta) << "\n";

    if (node->isTerminal) {
        cout << indent(depth) << "  Terminal Node " << node->name
             << " -> Value = " << node->terminalValue << "\n";
        return node->terminalValue;
    }

    int bestVal;
    string bestChild = "";

    if (node->isMax) {
        bestVal = numeric_limits<int>::min();

        for (size_t i = 0; i < node->children.size(); ++i) {
            Node* child = node->children[i];
            cout << indent(depth) << "  Checking child: " << child->name << "\n";

            int val = alphaBeta(child, alpha, beta, depth + 1);

            if (val > bestVal) {
                bestVal   = val;
                bestChild = child->name;
            }

            alpha = max(alpha, bestVal);

            cout << indent(depth) << "  After " << child->name
                 << ": Value = " << val
                 << ", Alpha = " << fmtVal(alpha)
                 << ", Beta = "  << fmtVal(beta) << "\n";

            if (alpha >= beta) {
                cout << indent(depth) << "*** PRUNING OCCURRED ***\n";
                cout << indent(depth) << "  At Node       : " << node->name << "\n";
                cout << indent(depth) << "  Reason        : Alpha >= Beta\n";
                cout << indent(depth) << "  Alpha         : " << fmtVal(alpha) << "\n";
                cout << indent(depth) << "  Beta          : " << fmtVal(beta) << "\n";

                string pruned = "";
                for (size_t j = i + 1; j < node->children.size(); ++j) {
                    prunedNodes.push_back(node->children[j]->name);
                    if (!pruned.empty()) pruned += ", ";
                    pruned += node->children[j]->name;
                }
                cout << indent(depth) << "  Pruned Nodes  : " << pruned << "\n";
                cout << indent(depth) << "  Remaining children will NOT be evaluated.\n";
                break;
            }
        }
    } else {
        bestVal = numeric_limits<int>::max();

        for (size_t i = 0; i < node->children.size(); ++i) {
            Node* child = node->children[i];
            cout << indent(depth) << "  Checking child: " << child->name << "\n";

            int val = alphaBeta(child, alpha, beta, depth + 1);

            if (val < bestVal) {
                bestVal   = val;
                bestChild = child->name;
            }

            beta = min(beta, bestVal);

            cout << indent(depth) << "  After " << child->name
                 << ": Value = " << val
                 << ", Alpha = " << fmtVal(alpha)
                 << ", Beta = "  << fmtVal(beta) << "\n";

            if (beta <= alpha) {
                cout << indent(depth) << "*** PRUNING OCCURRED ***\n";
                cout << indent(depth) << "  At Node       : " << node->name << "\n";
                cout << indent(depth) << "  Reason        : Alpha >= Beta\n";
                cout << indent(depth) << "  Alpha         : " << fmtVal(alpha) << "\n";
                cout << indent(depth) << "  Beta          : " << fmtVal(beta) << "\n";

                string pruned = "";
                for (size_t j = i + 1; j < node->children.size(); ++j) {
                    prunedNodes.push_back(node->children[j]->name);
                    if (!pruned.empty()) pruned += ", ";
                    pruned += node->children[j]->name;
                }
                cout << indent(depth) << "  Pruned Nodes  : " << pruned << "\n";
                cout << indent(depth) << "  Remaining children will NOT be evaluated.\n";
                break;
            }
        }
    }

    if (!bestChild.empty()) {
        bestMoves.push_back({node->name, bestChild});
    }

    return bestVal;
}

// ──────────────────────────────────────────────
// Build optimal path by following bestMoves
// ──────────────────────────────────────────────
string buildOptimalPath(const string& root) {
    string path = root;
    string cur  = root;

    while (true) {
        bool found = false;
        for (const auto& bm : bestMoves) {
            if (bm.nodeFrom == cur) {
                path += " -> " + bm.nodeTo;
                cur   = bm.nodeTo;
                found = true;
                break;
            }
        }
        if (!found) break;
    }
    return path;
}

// ──────────────────────────────────────────────
// Free tree memory
// ──────────────────────────────────────────────
void freeTree(Node* node) {
    if (!node) return;
    for (Node* c : node->children) freeTree(c);
    delete node;
}

// ──────────────────────────────────────────────
// Hardcoded tree:
//   A (MAX) -> B (MIN), C (MIN)
//   B (MIN) -> D (TERM=3), E (TERM=5)
//   C (MIN) -> G (TERM=7), H (TERM=3), I (TERM=2), J (TERM=1)
// ──────────────────────────────────────────────
Node* buildTree() {
    auto makeTerminal = [](const string& name, bool isMax, int val) {
        Node* n = new Node();
        n->name = name; n->isMax = isMax;
        n->isTerminal = true; n->terminalValue = val;
        return n;
    };
    auto makeInner = [](const string& name, bool isMax) {
        Node* n = new Node();
        n->name = name; n->isMax = isMax;
        n->isTerminal = false;
        return n;
    };

    Node* D = makeTerminal("D", true,  3);
    Node* E = makeTerminal("E", true,  5);
    Node* G = makeTerminal("G", true,  7);
    Node* H = makeTerminal("H", true,  3);
    Node* I = makeTerminal("I", true,  2);
    Node* J = makeTerminal("J", true,  1);

    Node* B = makeInner("B", false);
    B->children = {D, E};

    Node* C = makeInner("C", false);
    C->children = {G, H, I, J};

    Node* A = makeInner("A", true);
    A->children = {B, C};

    return A;
}

// ──────────────────────────────────────────────
// Main
// ──────────────────────────────────────────────
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Node* root = buildTree();

    cout << "\n---\n";

    int INF    = numeric_limits<int>::max();
    int result = alphaBeta(root, -INF, INF, 0);

    string sep(30, '=');
    cout << "\n" << sep << "\n";
    cout << "RESULT\n";
    cout << sep << "\n";

    cout << "Best Value (from root) = " << result << "\n";
    cout << "Optimal Path: " << buildOptimalPath(root->name) << "\n";

    cout << "Visited Nodes (in order): ";
    for (size_t i = 0; i < visitedNodes.size(); ++i) {
        if (i) cout << ", ";
        cout << visitedNodes[i];
    }
    cout << "\n";

    cout << "Pruned Nodes: ";
    for (size_t i = 0; i < prunedNodes.size(); ++i) {
        if (i) cout << ", ";
        cout << prunedNodes[i];
    }
    cout << "\n";

    cout << sep << "\n";

    freeTree(root);
    return 0;
}
