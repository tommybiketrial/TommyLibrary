#pragma once
#define N 9
#define BOX 3
#define UNASSIGNED 0

class SudokuGenerator {
private:
    int gridOutput[N][N] = { 0 };

    bool isSafe(int row, int col, int num) {
        for (int x = 0; x <= 8; x++) {
            if (gridOutput[row][x] == num) {
                return false;
            }
        }
        for (int x = 0; x <= 8; x++) {
            if (gridOutput[x][col] == num) {
                return false;
            }
        }
        int startRow = row - row % 3, startCol = col - col % 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (gridOutput[i + startRow][j + startCol] == num) {
                    return false;
                }
            }
        }
        return true;
    }

    bool solveSudoku(int row, int col) {
        if (row == N - 1 && col == N) {
            return true;
        }
        if (col == N) {
            row++;
            col = 0;
        }
        if (gridOutput[row][col] > 0) {
            return solveSudoku(row, col + 1);
        }
        for (int num = 1; num <= N; num++) {
            if (isSafe(row, col, num)) {
                gridOutput[row][col] = num;
                if (solveSudoku(row, col + 1)) {
                    return true;
                }
            }
            gridOutput[row][col] = 0;
        }
        return false;
    }

    void fillDiagonal() {
        for (int i = 0; i < N; i += 3) {
            fillBox(i, i);
        }
    }

    void fillBox(int row, int col) {
        int num;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                do {
                    num = randomGenerator(N);
                } while (!isSafe(row, col, num));
                gridOutput[row + i][col + j] = num;
            }
        }
    }

    int randomGenerator(int num) {
        return (int)(rand() % num + 1);
    }

    void removeDigits(int count) {
        int idx1, idx2;
        while (count) {
            idx1 = randomGenerator(N) - 1;
            idx2 = randomGenerator(N) - 1;
            if (gridOutput[idx1][idx2] != 0) {
                count--;
                gridOutput[idx1][idx2] = 0;
            }
        }
    }

public:
    SudokuGenerator() {
        fillDiagonal();
        solveSudoku(0, 0);
    }

    void makePuzzle(int removeCount) {
        removeDigits(removeCount);
    }

    int(*getGrid())[N] {
        // Allocate a new grid on the heap
        int(*outputGrid)[N] = new int[N][N];
        // Copy the generated grid to the output grid
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                outputGrid[i][j] = gridOutput[i][j];
            }
        }
        // Return the output grid
        return outputGrid;
    }
};

class SudokuSolver {
    vector<vector<int>> solutions;
    int gridOutput[N][N];

    bool isSafe(int grid[N][N], int row, int col, int num) {
        for (int x = 0; x <= 8; x++) {
            if (grid[row][x] == num) {
                return false;
            }
        }
        for (int x = 0; x <= 8; x++) {
            if (grid[x][col] == num) {
                return false;
            }
        }
        int startRow = row - row % 3, startCol = col - col % 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (grid[i + startRow][j + startCol] == num) {
                    return false;
                }
            }
        }
        return true;
    }

    void solveSudoku(int grid[N][N], int row, int col) {
        if (row == N - 1 && col == N) {
            vector<int> solution;
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    solution.push_back(grid[i][j]);
                }
            }
            solutions.push_back(solution);
            return;
        }
        if (col == N) {
            row++;
            col = 0;
        }
        if (grid[row][col] > 0) {
            solveSudoku(grid, row, col + 1);
            return;
        }
        for (int num = 1; num <= N; num++) {
            if (isSafe(grid, row, col, num)) {
                grid[row][col] = num;
                solveSudoku(grid, row, col + 1);
                grid[row][col] = 0;
            }
        }
    }

public:
    SudokuSolver(int gridInput[N][N]) {
        int grid[N][N];
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                grid[i][j] = gridInput[i][j];
            }
        }
        solveSudoku(grid, 0, 0);
        if (solutions.size() == 0) {
            //cout << "No solution exists" << endl;
            return;
        }
        for (auto solution : solutions) {
            for (int i = 0; i < N * N; i++) {
                gridOutput[i / N][i % N] = solution[i];
            }
            /*cout << "Solution:" << endl;
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    cout << gridOutput[i][j] << " ";
                }
                cout << endl;
            }
            cout << endl;
            */
        }
    }

    vector<int(*)[N]> getGrids() {
        vector<int(*)[N]> grids;
        for (auto& solution : solutions) {
            int(*grid)[N] = new int[N][N];
            for (int i = 0; i < N * N; i++) {
                grid[i / N][i % N] = solution[i];
            }
            grids.push_back(grid);
        }
        return grids;
    }
};

class SudokuWrapper {
public:

    int DigitsToRemove = 0;
    int(*input)[N];
    int(*puzzle)[N];
    vector<int(*)[N]> grids;
    SudokuGenerator wrapper1;

    SudokuWrapper() {
        input = wrapper1.getGrid();
        puzzle = new int[N][N]; // allocate memory for puzzle
        /*
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cout << input[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
        cout << "Remove Digits..." << endl;
        */
    }

    void generate() {
        wrapper1.makePuzzle(DigitsToRemove);
        int(*output2)[N] = wrapper1.getGrid();
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                puzzle[i][j] = output2[i][j];
                //cout << output2[i][j] << " ";
            }
            //cout << endl;
        }
        //cout << endl;
        //cout << "Solving..." << endl;
        SudokuSolver wrapper2(output2);
        grids = wrapper2.getGrids();
        for (int k = 0; k < grids.size(); ++k) {
            int(*grid)[N] = grids[k];
            /*cout << "solution " << k + 1 << ":" << endl;
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    cout << grid[i][j] << " ";
                }
                cout << endl;
            }*/
        }
    }

    void makePuzzle(int Digits_To_Remove) {
        DigitsToRemove = Digits_To_Remove;
    }

    void deleteData() {
        delete[] input;
        delete[] puzzle;
        for (auto& grid : grids) {
            delete[] grid;
        }
    }

};