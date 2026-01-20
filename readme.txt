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
