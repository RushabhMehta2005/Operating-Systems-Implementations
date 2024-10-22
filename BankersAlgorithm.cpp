#include <iostream>
#include <vector>
using namespace std;

class BankersAlgorithm {
private:
    vector<vector<int>> max;
    vector<vector<int>> alloc;
    vector<int> avail;
    int numProcesses;
    int numResources;

public:
    BankersAlgorithm(int np, int nr, vector<vector<int>> m, vector<vector<int>> a, vector<int> av) 
        : numProcesses(np), numResources(nr), max(m), alloc(a), avail(av) {}

    // Function to check if a process can request resources
    bool isSafe(int process) {
        vector<int> need(numResources);
        // Calculate need matrix
        for (int i = 0; i < numResources; i++) {
            need[i] = max[process][i] - alloc[process][i];
        }
        // Check if the process can safely request resources
        for (int i = 0; i < numResources; i++) {
            if (need[i] > avail[i]) {
                return false;
            }
        }
        return true;
    }

    // Banker's algorithm implementation
    bool execute() {
        vector<bool> finish(numProcesses, false);
        vector<int> safeSequence;
        int numFinished = 0;
        
        while (numFinished < numProcesses) {
            bool found = false;
            // Try to find an unfinished process that can request resources
            for (int i = 0; i < numProcesses; i++) {
                if (!finish[i] && isSafe(i)) {
                    // Grant resources to the process
                    for (int j = 0; j < numResources; j++) {
                        avail[j] += alloc[i][j];
                    }
                    safeSequence.push_back(i);
                    finish[i] = true;
                    numFinished++;
                    found = true;
                }
            }
            // If no such process is found, the system is in an unsafe state
            if (!found) {
                return false;
            }
        }

        // If all processes have been granted resources safely
        cout << "Safe sequence: ";
        for (int i = 0; i < safeSequence.size(); i++) {
            cout << "P" << safeSequence[i];
            if (i < safeSequence.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
        return true;
    }
};

int main() {
    int numProcesses = 5, numResources = 3;
    
    vector<vector<int>> max = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };
    
    vector<vector<int>> alloc = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };
    
    vector<int> avail = {3, 3, 2};

    BankersAlgorithm banker(numProcesses, numResources, max, alloc, avail);
    
    if (banker.execute()) {
        cout << "System is in a safe state." << endl;
    } else {
        cout << "System is in an unsafe state." << endl;
    }
    
    return 0;
}
