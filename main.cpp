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

// Helper function to print grid
void printGrid(vector<vector<char> > grid) {
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
// Returns false if distance of point is less than TOWER_DISTANCE
bool towerNeeded(vector<vector<char> > &grid, int testRow, int testCol) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] != '.' && distance(testRow, testCol, i, j) <= TOWER_DISTANCE) {
                return false;
            }
        }
    }
    
    return true;
}

// Final check if the board is valid
bool allCovered(const vector<vector<char>>& grid) {
    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            bool covered = false;

            for (int i = 0; i < GRID_SIZE && !covered; i++) {
                for (int j = 0; j < GRID_SIZE; j++) {
                    if (grid[i][j] != '.' &&
                        distance(r, c, i, j) <= TOWER_DISTANCE)
                    {
                        covered = true;
                        break;
                    }
                }
            }

            if (!covered) return false;
        }
    }
    return true;
}

bool isCovered(const vector<vector<char>>& grid, int r, int c) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] != '.' && distance(r, c, i, j) <= TOWER_DISTANCE) return true;
        }
    }
    return false;
}


// Solver recursive function.
bool cellSolver(vector<vector<char> > &grid, int row, int col, int numFreq) {
    // Base case
    //cout << "Row: " << row << " " << "Col: " << col << endl;
    
    if (row == GRID_SIZE) return allCovered(grid);
    if (col == GRID_SIZE) return cellSolver(grid, row+1, 0, numFreq);
    
    bool needs_tower = towerNeeded(grid, row, col);
    
    //if (col == GRID_SIZE) return cellSolver(grid, row + 1, 0, numFreq);
    //if (row == GRID_SIZE) return true;
    
    //if (towerNeeded(grid, row, col)) {
        
        vector<char> available_freqs(FREQ.begin(), FREQ.begin()+numFreq);
        
        for (char freq : available_freqs) {
            bool valid_freq = true;
            for (int i = 0; i < GRID_SIZE; i++) {
                for (int j = 0; j < GRID_SIZE; j++) {
                    if (i == row && j == col) continue;

                    if (distance(row, col, i, j) < FREQ_DISTANCE && grid[i][j] == freq) {
                        valid_freq = false;
                        break;
                    }
                }
                if (!valid_freq) break;
            }
            if (valid_freq) {
                if (needs_tower) {
                    grid[row][col] = freq;
                    if (cellSolver(grid, row, col+1, numFreq)) return true;
                    grid[row][col] = '.';
                }
            }
        }
    //}
    
    if (!needs_tower) {
        if (cellSolver(grid, row, col+1, numFreq)) return true;
    }
    
    //if (isCovered(grid, row, col)) {
    //    if (cellSolver(grid, row, col + 1, numFreq)) return true;
    //}

    return false;
}

// Helper function to kick off problem 
bool cellProblem(vector<vector<char> > &grid, int numFreq) {
    return cellSolver(grid, 0, 0, numFreq);
}

int main() {
    // Create the empty grid of size GRID_SIZE. 
    // Place a '.' in each square
    vector<vector<char> > grid(GRID_SIZE, vector<char>(GRID_SIZE, '.'));
    
    bool valid = false;
    int num_freq;
    // Prompt the user for the number of frequecies
    
    while (!valid) {
        cout << "Please select the number of frequencies to test (1 - 10): ";
        string input;
        getline(cin, input);
        
        num_freq = stoi(input);
        
        if (num_freq >= 1 && num_freq <= 10) {
            valid = true;
        }
    }
    
    if (cellProblem(grid, num_freq)) {
        cout << "Solved!" << endl;
        printGrid(grid);
    }
    else {
        cout << "No Solution possible." << endl;
    }
    
    return 0;
}
