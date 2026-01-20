You are placing cell phone towers on a 10x10 grid. Cell phone frequencies are represented with a lowercase letter. There are two rules:
The constant TOWER_DISTANCE represents the maximum distance from a point on the grid to a tower.
Two towers sharing the same frequency must be no closer than FREQ_DISTANCE.
TOWER_DISTANCE = 2.1
FREQ_DISTANCE = 5.6
Hint: This is similar to the N-Queen problem.
Sample Output:
Sample Output 1:
Please select the number of frequencies to test (1 - 10): 3
No Solution possible.
Sample Output 2:
Please select the number of frequencies to test (1 - 10): 7
Solved!
a . . b . . a . . b 
. . . . . . . . . . 
. c . . d . . c . . 
. . . . . . . . . e 
e . . f . . h . . . 
. . . a . . b . . . 
b . . . . . . . d . 
. d . . c . . . . . 
. . . . . . e . . a 
f . . h . . . . . . 

Solution Approach:
The program uses backtracking to place towers on the 10x10 grid. Each cell is
visited in row-major order, and the solver tries all valid frequencies at that
cell or leaves it empty when it can still be covered by a tower placed later.
Two constraints are enforced:
1) Coverage: every grid cell must be within TOWER_DISTANCE of at least one tower.
2) Frequency spacing: towers with the same letter must be at least FREQ_DISTANCE
   apart.

To reduce the search space, the solver precomputes which tower positions can
cover each cell. During the search it performs forward checking: if any
uncovered cell cannot be covered by any future placement, that branch is
pruned. When all cells are assigned, the solver verifies full coverage before
accepting a solution.

Function Purpose and Implementation:
printGrid:
- Purpose: Display the grid in the required format.
- How: Iterates rows and columns, printing each cell followed by a space.

distance:
- Purpose: Compute Euclidean distance between two grid coordinates.
- How: Uses the Pythagorean formula with sqrt on integer deltas.

towerNeeded:
- Purpose: Determine whether a cell is currently uncovered.
- How: Scans the grid for any tower within TOWER_DISTANCE; returns true if none.

freqAllowed:
- Purpose: Check if a specific frequency can be placed at a cell.
- How: Scans for same-letter towers closer than FREQ_DISTANCE; rejects if found.

canPlaceAnyFreq:
- Purpose: Determine whether any frequency is legal at a cell.
- How: Tries all available frequencies and returns true if any are allowed.

buildCoverPositions:
- Purpose: Precompute coverage neighborhoods for forward checking.
- How: For each target cell, stores all positions within TOWER_DISTANCE.

canBeCoveredLater:
- Purpose: Decide if a cell can still be covered by a future placement.
- How: Looks through the precomputed coverage list for unassigned cells at or
  after startIndex that can accept at least one frequency.

allCovered:
- Purpose: Validate that every grid cell is covered at the end of the search.
- How: Calls towerNeeded on each cell and fails if any remain uncovered.

forwardCheckCoverage:
- Purpose: Prune branches that cannot possibly achieve full coverage.
- How: For each currently uncovered cell, ensures canBeCoveredLater is true.

validFreq:
- Purpose: Find the next valid frequency to try at a cell.
- How: Starts from the current frequency (if any) and returns the next allowed
  letter; returns '.' when none remain.

cellSolver:
- Purpose: Recursively search for a valid tower placement.
- How: Visits cells in row-major order, tries all valid frequencies, optionally
  leaves a cell empty if it can be covered later, and uses forward checking to
  prune impossible branches.

cellProblem:
- Purpose: Initialize state and start the solver.
- How: Builds coverage neighborhoods and calls cellSolver at (0, 0).

main:
- Purpose: Program entry point and user I/O.
- How: Initializes the grid, reads the number of frequencies, runs the solver,
  and prints either the solution grid or a no-solution message.
