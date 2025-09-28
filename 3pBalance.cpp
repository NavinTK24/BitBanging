#include <bits/stdc++.h>
using namespace std;

struct Move {
    int load;
    string fromPhase;
    string toPhase;
    int houseIdx;
};

int main() {
    // Example loads
    vector<int> R = {0,4,3,4,2,1,2};
    vector<int> Y = {3,2,2,1,3,4,2};
    vector<int> B = {0,0,2,2,1,3,1};

    map<string, vector<int>> phases = {{"R", R}, {"Y", Y}, {"B", B}};
    vector<Move> moves;

    // Compute total load
    int totalLoad = 0;
    for (auto &p : phases) totalLoad += accumulate(p.second.begin(), p.second.end(), 0);
    int ideal = totalLoad / 3;

    while (true) {
        // Compute phase totals
        map<string,int> totals;
        for (auto &p : phases) totals[p.first] = accumulate(p.second.begin(), p.second.end(), 0);

        // Find most overloaded and underloaded phases
        string maxPhase, minPhase;
        int maxTotal = INT_MIN, minTotal = INT_MAX;
        for (auto &p : totals) {
            if (p.second > maxTotal) { maxTotal = p.second; maxPhase = p.first; }
            if (p.second < minTotal) { minTotal = p.second; minPhase = p.first; }
        }

        if (maxTotal - minTotal <= 1) break; // as balanced as possible

        // Find the house in maxPhase to move to minPhase
        int bestHouse = -1, bestLoad = 0;
        for (int i = 0; i < phases[maxPhase].size(); i++) {
            int load = phases[maxPhase][i];
            if (load > 0 && load <= (ideal - totals[minPhase])) {
                if (load > bestLoad) { bestLoad = load; bestHouse = i; }
            }
        }

        if (bestHouse == -1) {
            // If no perfect fit, move the largest possible
            for (int i = 0; i < phases[maxPhase].size(); i++) {
                int load = phases[maxPhase][i];
                if (load > 0 && load > bestLoad) { bestLoad = load; bestHouse = i; }
            }
        }

        // Perform the move
        phases[maxPhase][bestHouse] -= bestLoad;
        phases[minPhase][bestHouse] += bestLoad;
        moves.push_back({bestLoad, maxPhase, minPhase, bestHouse});
    }

    // Print moves
    cout << "Optimized Moves to balance phases:\n";
    for (auto &m : moves)
        cout << "Move " << m.load << "A from " << m.fromPhase
             << " (house " << m.houseIdx << ") to " << m.toPhase << endl;

    // Print final totals
    cout << "\nFinal phase totals:\n";
    for (auto &p : phases) {
        int total = accumulate(p.second.begin(), p.second.end(), 0);
        cout << p.first << " = " << total << "A\n";
    }

    return 0;
}
