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

