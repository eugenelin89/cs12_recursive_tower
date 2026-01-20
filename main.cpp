#include "util.h"
#include <vector>
#include <map>
#include <cmath> // used for the sqrt function

// Cell tower struct
struct cellTower {
    int x, y;
};

const int GRID_SIZE = 10;
const double FREQ_DISTANCE = 5.6;
const double TOWER_DISTANCE = 2.1;
//  The FREQ vector  may have more frequencies than needed
const vector<char> FREQ {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
static vector<vector<vector<cellTower> > > g_coverPositions;

// Helper function to print grid
void printGrid(const vector<vector<char> > &grid) {
    for (int row = 0; row < GRID_SIZE; row ++) {
        for (int col = 0; col < GRID_SIZE; col ++) {
            cout << grid[row][col] << ' ';
        }
        cout << endl;
    }
}

// Helper function to determine the distance between points.
// x and y should represent the grid coordinates.
double distance(int x1, int y1, int x2, int y2) {
    int a = x2 - x1;
    int b = y2 - y1;
    return sqrt(a * a + b * b);
}
// Helper function to determine if a tower is needed
// Returns true if distance of point is less than TOWER_DISTANCE
bool towerNeeded(const vector<vector<char> > &grid, int testRow, int testCol) {
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            if (grid[row][col] != '.' &&
                distance(testRow, testCol, row, col) <= TOWER_DISTANCE) {
                return false;
            }
        }
    }
    return true;
}

// Helper function to determine if a frequency can be placed at a position
bool freqAllowed(const vector<vector<char> > &grid, int testRow, int testCol, char freq) {
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            if (row == testRow && col == testCol) continue;
            if (grid[row][col] == freq &&
                distance(testRow, testCol, row, col) < FREQ_DISTANCE) {
                return false;
            }
        }
    }
    return true;
}

// Helper function to find the next valid frequency
// Returns the next frequency to use
char validFreq(const vector<vector<char> > &grid, int testRow, int testCol, int numFreq) {
    int startIndex = 0;
    if (grid[testRow][testCol] != '.') {
        for (int i = 0; i < numFreq; i++) {
            if (FREQ[i] == grid[testRow][testCol]) {
                startIndex = i + 1;
                break;
            }
        }
    }

    for (int i = startIndex; i < numFreq; i++) {
        if (freqAllowed(grid, testRow, testCol, FREQ[i])) {
            return FREQ[i];
        }
    }
    return '.';
}

bool canPlaceAnyFreq(const vector<vector<char> > &grid, int testRow, int testCol, int numFreq) {
    for (int i = 0; i < numFreq; i++) {
        if (freqAllowed(grid, testRow, testCol, FREQ[i])) {
            return true;
        }
    }
    return false;
}

bool canBeCoveredLater(const vector<vector<char> > &grid, int targetRow, int targetCol,
                       int startIndex, int numFreq) {
    for (size_t i = 0; i < g_coverPositions[targetRow][targetCol].size(); i++) {
        int row = g_coverPositions[targetRow][targetCol][i].x;
        int col = g_coverPositions[targetRow][targetCol][i].y;
        int idx = row * GRID_SIZE + col;
        if (idx < startIndex || grid[row][col] != '.') continue;
        if (canPlaceAnyFreq(grid, row, col, numFreq)) {
            return true;
        }
    }
    return false;
}

bool allCovered(const vector<vector<char> > &grid) {
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            if (towerNeeded(grid, row, col)) return false;
        }
    }
    return true;
}

bool forwardCheckCoverage(const vector<vector<char> > &grid, int startIndex, int numFreq) {
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            if (!towerNeeded(grid, row, col)) continue;
            if (!canBeCoveredLater(grid, row, col, startIndex, numFreq)) {
                return false;
            }
        }
    }
    return true;
}

void buildCoverPositions() {
    if (!g_coverPositions.empty()) return;
    g_coverPositions.assign(GRID_SIZE,
                            vector<vector<cellTower> >(GRID_SIZE, vector<cellTower>()));
    for (int targetRow = 0; targetRow < GRID_SIZE; targetRow++) {
        for (int targetCol = 0; targetCol < GRID_SIZE; targetCol++) {
            for (int row = 0; row < GRID_SIZE; row++) {
                for (int col = 0; col < GRID_SIZE; col++) {
                    if (distance(targetRow, targetCol, row, col) <= TOWER_DISTANCE) {
                        g_coverPositions[targetRow][targetCol].push_back(cellTower{row, col});
                    }
                }
            }
        }
    }
}

// Solver recursive function.
bool cellSolver(vector<vector<char> > &grid, int row, int col, int numFreq) {
    if (row == GRID_SIZE) {
        return allCovered(grid);
    }

    int nextRow = row;
    int nextCol = col + 1;
    if (nextCol == GRID_SIZE) {
        nextCol = 0;
        nextRow++;
    }

    int curIndex = row * GRID_SIZE + col;
    bool covered = !towerNeeded(grid, row, col);
    bool canDefer = covered ||
                    canBeCoveredLater(grid, row, col, curIndex + 1, numFreq);

    if (!forwardCheckCoverage(grid, curIndex, numFreq)) {
        return false;
    }

    grid[row][col] = '.';
    char freq = validFreq(grid, row, col, numFreq);
    while (freq != '.') {
        grid[row][col] = freq;
        if (cellSolver(grid, nextRow, nextCol, numFreq)) {
            return true;
        }
        freq = validFreq(grid, row, col, numFreq);
    }

    grid[row][col] = '.';
    if (canDefer) {
        if (cellSolver(grid, nextRow, nextCol, numFreq)) {
            return true;
        }
    }
    return false;
}

// Helper function to kick off problem 
bool cellProblem(vector<vector<char> > &grid, int numFreq) {
    buildCoverPositions();
    return cellSolver(grid, 0, 0, numFreq);
}

int main() {
    // Create the empty grid of size GRID_SIZE. 
    // Place a '.' in each square
    vector<vector<char> > grid(GRID_SIZE, vector<char>(GRID_SIZE, '.'));
   
    // Prompt the user for the number of frequecies
    int numFreq = readInt(1, 10,
                          "Please select the number of frequencies to test (1 - 10): ",
                          "Please select the number of frequencies to test (1 - 10): ");
    
    // Print the results
    if (cellProblem(grid, numFreq)) {
        cout << "Solved!" << endl;
        printGrid(grid);
    } else {
        cout << "No Solution possible." << endl;
    }
    return 0;
}
