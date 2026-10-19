/*
 * ============================================================
 *   LOCAL BEAM SEARCH — Traveling Salesman Problem (TSP)
 * ============================================================
 *
 *  Algorithm  : Local Beam Search (width k)
 *  Neighbor   : 2-opt swap (reverse a sub-segment of the route)
 *  Objective  : Minimise total tour distance
 *
 *  Hardcoded input: 8 cities with (x, y) coordinates.
 *  Beam width (k) and max iterations are defined as constants.
 * ============================================================
 */

#include <bits/stdc++.h>
using namespace std;

// ──────────────────────────────────────────────
//  CONSTANTS
// ──────────────────────────────────────────────
const int    BEAM_WIDTH    = 4;    // number of states kept per iteration
const int    MAX_ITER      = 200;  // safety cap on iterations
const double INF           = 1e18;

// ──────────────────────────────────────────────
//  CITY DATA  (hardcoded)
// ──────────────────────────────────────────────
struct City {
    string name;
    double x, y;
};

const vector<City> CITIES = {
    {"A", 0.0,  0.0},
    {"B", 3.0,  4.0},
    {"C", 7.0,  1.0},
    {"D", 5.0,  7.0},
    {"E", 1.0,  6.0},
    {"F", 8.0,  5.0},
    {"G", 4.0,  3.0},
    {"H", 6.0,  9.0}
};

const int N = (int)CITIES.size();   // number of cities

// ──────────────────────────────────────────────
//  DISTANCE MATRIX
// ──────────────────────────────────────────────
double dist[8][8];

void buildDistMatrix() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            double dx = CITIES[i].x - CITIES[j].x;
            double dy = CITIES[i].y - CITIES[j].y;
            dist[i][j] = sqrt(dx*dx + dy*dy);
        }
}

// ──────────────────────────────────────────────
//  TOUR UTILITIES
// ──────────────────────────────────────────────
using Tour = vector<int>;

double tourCost(const Tour& t) {
    double cost = 0.0;
    for (int i = 0; i < N; i++)
        cost += dist[t[i]][t[(i + 1) % N]];
    return cost;
}

// Print a tour as:  A -> B -> C -> ... -> A  (cost)
void printTour(const Tour& t, double cost, const string& label = "") {
    if (!label.empty()) cout << label << "\n";
    cout << "   Route : ";
    for (int i = 0; i < N; i++)
        cout << CITIES[t[i]].name << (i < N-1 ? " -> " : "");
    cout << " -> " << CITIES[t[0]].name << "\n";
    cout << fixed << setprecision(4);
    cout << "   Cost  : " << cost << "\n";
}

// ──────────────────────────────────────────────
//  2-OPT NEIGHBORHOOD
//  Returns all tours reachable by one 2-opt swap.
// ──────────────────────────────────────────────
vector<Tour> generateNeighbors(const Tour& t) {
    vector<Tour> neighbors;
    for (int i = 1; i < N - 1; i++) {
        for (int j = i + 1; j < N; j++) {
            Tour nb = t;
            reverse(nb.begin() + i, nb.begin() + j + 1);
            neighbors.push_back(nb);
        }
    }
    return neighbors;
}

// ──────────────────────────────────────────────
//  RANDOM TOUR (used for beam initialisation)
// ──────────────────────────────────────────────
Tour randomTour(mt19937& rng) {
    Tour t(N);
    iota(t.begin(), t.end(), 0);
    shuffle(t.begin() + 1, t.end(), rng);   // keep city 0 fixed at start
    return t;
}

// ──────────────────────────────────────────────
//  SEPARATOR HELPERS
// ──────────────────────────────────────────────
void printLine(char c = '-', int w = 62) {
    cout << string(w, c) << "\n";
}

void printHeader(const string& s) {
    printLine('=');
    int pad = (62 - (int)s.size()) / 2;
    cout << string(max(0, pad), ' ') << s << "\n";
    printLine('=');
}

// ──────────────────────────────────────────────
//  LOCAL BEAM SEARCH
// ──────────────────────────────────────────────
Tour localBeamSearch() {
    mt19937 rng(42);   // fixed seed for reproducibility

    // ── Initialisation ──────────────────────────
    printHeader("LOCAL BEAM SEARCH  --  TSP");
    cout << "\n";
    cout << "  Cities      : " << N << "\n";
    cout << "  Beam Width  : " << BEAM_WIDTH << "\n";
    cout << "  Max Iter    : " << MAX_ITER << "\n";
    cout << "  Neighbor    : 2-opt swap\n";
    cout << "\n";

    // Display city table
    printLine();
    cout << "  CITY TABLE\n";
    printLine();
    cout << left << setw(8) << "  City" << setw(10) << "X" << "Y\n";
    printLine();
    for (const auto& c : CITIES)
        cout << "  " << left << setw(6) << c.name
             << setw(10) << c.x << c.y << "\n";
    printLine();
    cout << "\n";

    // Generate initial beam
    vector<Tour> beam;
    while ((int)beam.size() < BEAM_WIDTH)
        beam.push_back(randomTour(rng));

    // Sort beam by cost
    sort(beam.begin(), beam.end(),
         [](const Tour& a, const Tour& b){ return tourCost(a) < tourCost(b); });

    printLine('=');
    cout << "  INITIAL BEAM  (k = " << BEAM_WIDTH << " random tours)\n";
    printLine('=');
    for (int b = 0; b < BEAM_WIDTH; b++) {
        cout << "\n  State " << (b+1) << ":\n";
        printTour(beam[b], tourCost(beam[b]));
    }
    cout << "\n";

    double bestCost   = tourCost(beam[0]);
    Tour   bestTour   = beam[0];
    int    iter       = 0;
    int    noImproveC = 0;

    // ── Main Loop ──────────────────────────────
    printLine('=');
    cout << "  SEARCH PROGRESS\n";
    printLine('=');
    cout << "\n";

    for (iter = 1; iter <= MAX_ITER; iter++) {

        // Collect ALL neighbors of ALL beam states
        vector<pair<double, Tour>> pool;
        for (const Tour& t : beam) {
            for (const Tour& nb : generateNeighbors(t)) {
                pool.push_back({tourCost(nb), nb});
            }
        }

        // Sort and pick best BEAM_WIDTH unique tours
        sort(pool.begin(), pool.end(),
             [](const auto& a, const auto& b){ return a.first < b.first; });

        vector<Tour> newBeam;
        set<vector<int>> seen;
        for (auto& [c, t] : pool) {
            if ((int)newBeam.size() == BEAM_WIDTH) break;
            if (!seen.count(t)) {
                seen.insert(t);
                newBeam.push_back(t);
            }
        }
        beam = newBeam;

        double iterBest = tourCost(beam[0]);

        // Convergence check
        if (iterBest < bestCost - 1e-9) {
            bestCost   = iterBest;
            bestTour   = beam[0];
            noImproveC = 0;
        } else {
            noImproveC++;
        }

        // Print progress every 10 iterations (and first 3)
        bool printIt = (iter <= 3) || (iter % 10 == 0);
        if (printIt) {
            cout << "  Iter " << setw(3) << iter
                 << "  |  Best in beam: "
                 << fixed << setprecision(4) << iterBest
                 << "  |  Global best: " << bestCost << "\n";
        }

        // Termination: no improvement for 20 consecutive iterations
        if (noImproveC >= 20) {
            cout << "\n  [Converged] No improvement for 20 iterations."
                 << " Stopping at iter " << iter << ".\n";
            break;
        }
    }

    if (iter > MAX_ITER)
        cout << "\n  [Stopped] Maximum iterations reached.\n";

    return bestTour;
}

// ──────────────────────────────────────────────
//  MAIN
// ──────────────────────────────────────────────
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    buildDistMatrix();

    Tour best = localBeamSearch();
    double bestCost = tourCost(best);

    // ── Final Result ────────────────────────────
    cout << "\n";
    printLine('=');
    cout << "  FINAL RESULT\n";
    printLine('=');
    printTour(best, bestCost);

    // Leg-by-leg breakdown
    cout << "\n  LEG BREAKDOWN:\n";
    printLine();
    cout << left << setw(20) << "  Segment"
         << right << setw(10) << "Distance\n";
    printLine();
    double runSum = 0.0;
    for (int i = 0; i < N; i++) {
        int a = best[i], b = best[(i+1) % N];
        double d = dist[a][b];
        runSum += d;
        cout << "  " << left << setw(4) << CITIES[a].name
             << " -> " << setw(4) << CITIES[b].name
             << right << setw(10) << fixed << setprecision(4) << d << "\n";
    }
    printLine();
    cout << "  " << left << setw(16) << "TOTAL"
         << right << setw(10) << fixed << setprecision(4) << runSum << "\n";
    printLine('=');

    cout << "\n  Algorithm : Local Beam Search\n";
    cout << "  Beam Width: " << BEAM_WIDTH << "\n";
    cout << "  Neighbors : 2-opt swap\n";
    printLine('=');
    cout << "\n";

    return 0;
}
