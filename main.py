import math

# Cell tower struct (represented as a tuple of (x, y))

GRID_SIZE = 10
FREQ_DISTANCE = 5.6
TOWER_DISTANCE = 2.1
FREQ = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j']

# Precomputed list of tower positions that cover each cell.
g_cover_positions = []


def printGrid(grid):
    for row in range(GRID_SIZE):
        for col in range(GRID_SIZE):
            print(grid[row][col], end=' ')
        print()


def distance(x1, y1, x2, y2):
    a = x2 - x1
    b = y2 - y1
    return math.sqrt(a * a + b * b)


def towerNeeded(grid, testRow, testCol):
    for row in range(GRID_SIZE):
        for col in range(GRID_SIZE):
            if grid[row][col] != '.' and distance(testRow, testCol, row, col) <= TOWER_DISTANCE:
                return False
    return True


def freqAllowed(grid, testRow, testCol, freq):
    for row in range(GRID_SIZE):
        for col in range(GRID_SIZE):
            if row == testRow and col == testCol:
                continue
            if grid[row][col] == freq and distance(testRow, testCol, row, col) < FREQ_DISTANCE:
                return False
    return True


def canPlaceAnyFreq(grid, testRow, testCol, numFreq):
    # Check if at least one frequency can be placed at this position.
    for i in range(numFreq):
        if freqAllowed(grid, testRow, testCol, FREQ[i]):
            return True
    return False


def buildCoverPositions():
    global g_cover_positions
    if g_cover_positions:
        return
    # Precompute coverage neighborhoods to speed up forward checks.
    g_cover_positions = [[[] for _ in range(GRID_SIZE)] for _ in range(GRID_SIZE)]
    for targetRow in range(GRID_SIZE):
        for targetCol in range(GRID_SIZE):
            for row in range(GRID_SIZE):
                for col in range(GRID_SIZE):
                    if distance(targetRow, targetCol, row, col) <= TOWER_DISTANCE:
                        g_cover_positions[targetRow][targetCol].append((row, col))


def canBeCoveredLater(grid, targetRow, targetCol, startIndex, numFreq):
    # See if any unassigned future position could cover this cell.
    for (row, col) in g_cover_positions[targetRow][targetCol]:
        idx = row * GRID_SIZE + col
        if idx < startIndex or grid[row][col] != '.':
            continue
        if canPlaceAnyFreq(grid, row, col, numFreq):
            return True
    return False


def allCovered(grid):
    # Validate final grid coverage for all cells.
    for row in range(GRID_SIZE):
        for col in range(GRID_SIZE):
            if towerNeeded(grid, row, col):
                return False
    return True


def forwardCheckCoverage(grid, startIndex, numFreq):
    # Prune if any uncovered cell cannot be covered by a future placement.
    for row in range(GRID_SIZE):
        for col in range(GRID_SIZE):
            if not towerNeeded(grid, row, col):
                continue
            if not canBeCoveredLater(grid, row, col, startIndex, numFreq):
                return False
    return True


def validFreq(grid, testRow, testCol, numFreq):
    startIndex = 0
    if grid[testRow][testCol] != '.':
        for i in range(numFreq):
            if FREQ[i] == grid[testRow][testCol]:
                startIndex = i + 1
                break

    for i in range(startIndex, numFreq):
        if freqAllowed(grid, testRow, testCol, FREQ[i]):
            return FREQ[i]
    return '.'


def cellSolver(grid, row, col, numFreq):
    if row == GRID_SIZE:
        return allCovered(grid)

    nextRow = row
    nextCol = col + 1
    if nextCol == GRID_SIZE:
        nextCol = 0
        nextRow += 1

    curIndex = row * GRID_SIZE + col
    covered = not towerNeeded(grid, row, col)
    canDefer = covered or canBeCoveredLater(grid, row, col, curIndex + 1, numFreq)

    if not forwardCheckCoverage(grid, curIndex, numFreq):
        return False

    grid[row][col] = '.'
    freq = validFreq(grid, row, col, numFreq)
    while freq != '.':
        grid[row][col] = freq
        if cellSolver(grid, nextRow, nextCol, numFreq):
            return True
        freq = validFreq(grid, row, col, numFreq)

    grid[row][col] = '.'
    if canDefer:
        if cellSolver(grid, nextRow, nextCol, numFreq):
            return True
    return False


def cellProblem(grid, numFreq):
    buildCoverPositions()
    return cellSolver(grid, 0, 0, numFreq)


def readInt(low, high, prompt, reprompt):
    if reprompt == "":
        reprompt = prompt
    while True:
        try:
            value = int(input(prompt))
        except ValueError:
            prompt = reprompt
            continue
        if low <= value <= high:
            return value
        prompt = reprompt


def main():
    # Create the empty grid of size GRID_SIZE.
    # Place a '.' in each square
    grid = [['.' for _ in range(GRID_SIZE)] for _ in range(GRID_SIZE)]

    # Prompt the user for the number of frequecies
    numFreq = readInt(1, 10,
                      "Please select the number of frequencies to test (1 - 10): ",
                      "Please select the number of frequencies to test (1 - 10): ")

    # Print the results
    if cellProblem(grid, numFreq):
        print("Solved!")
        printGrid(grid)
    else:
        print("No Solution possible.")


if __name__ == "__main__":
    main()
