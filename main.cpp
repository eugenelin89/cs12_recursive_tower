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
// Returns true if distance of point is less than TOWER_DISTANCE
bool towerNeeded(vector<vector<char> > grid, int testRow, int testCol) {
    // Base case
    return true;
}

// Helper function to find the next valid frequency
// Returns the next frequency to use
char validFreq(vector<vector<char> > grid, int testRow, int testCol, int numFreq) {
    // Enter your code here
    return '.';
}

bool validGrid(vector<vector<char> > &grid) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            bool enforce_freq = false;
            bool enforce_tower_distance = false;
            bool valid_distance = false;
            if (grid[i][j] != '.') {
                enforce_freq = true;
            }
            else {
                enforce_tower_distance = true;
            }
            for (int x = 0; x < GRID_SIZE; x++) {
                for (int y = 0; y < GRID_SIZE; y++) {
                    if (enforce_freq) {
                        if (i==x && j==y) continue;
                        if (distance(i, j, x, y) < FREQ_DISTANCE && grid[i][j] == grid[x][y]) {
                            return false;
                        }
                    }
                    else if (enforce_tower_distance) {
                        if (grid[x][y] != '.' && distance(i, j, x, y) <= TOWER_DISTANCE) {
                            valid_distance = true;
                            break;
                        }
                    }
                }
                if (valid_distance) break;
            }
            
            if (enforce_tower_distance && !valid_distance) return false;
        }
    }
    return true;
}

bool checkDistanceRule(vector<vector<char> > &grid) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            bool valid_distance = false;
            for (int x = 0; x < GRID_SIZE; x++) {
                for (int y = 0; y < GRID_SIZE; y++) {
                    if (grid[x][y] != '.' && distance(i, j, x, y) <= TOWER_DISTANCE) {
                        valid_distance = true;
                        break;
                    }
                }
                if (valid_distance) break;
            }
            
            if (!valid_distance) return false;
        }
    }
    return true;
}

bool checkFreqRule(vector<vector<char> > &grid) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] == '.') continue;
            for (int x = 0; x < GRID_SIZE; x++) {
                for (int y = 0; y < GRID_SIZE; y++) {
                    if (i==x && j==y) continue;
                    if (distance(i, j, x, y) < FREQ_DISTANCE && grid[i][j] == grid[x][y]) {
                        return false;
                    }
                }    
            }
        }
    }
    return true;
}

// Solver recursive function.
bool cellSolver(vector<vector<char> > &grid, int row, int col, int numFreq) {
    // Base case
    if (row >= GRID_SIZE) return checkDistanceRule(grid);
    
    // Go to the next row
    if (col >= GRID_SIZE) return cellSolver(grid, row+1, 0, numFreq);
    
    // Option 1: place a tower with each available frequency
    vector<char> available_freqs(FREQ.begin(), FREQ.begin()+numFreq);
    for (char freq : available_freqs) {
        grid[row][col] = freq;
        // Prune
        if (!checkFreqRule(grid)) {
            grid[row][col] = '.';
            continue;
        }
        if (cellSolver(grid, row, col + 1, numFreq)) return true;
    }
    
    // Option 2: leave cell empty
    grid[row][col] = '.';
    if (cellSolver(grid, row, col+1, numFreq)) return true;

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
