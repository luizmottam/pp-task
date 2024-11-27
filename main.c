#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 100

// Function to check if the word exists in the grid
int dfs(char **grid, int row, int col, int rows, int cols, char *word, int index) {
    // If the word is completely found
    if (index == strlen(word)) {
        return 1;
    }

    // Check bounds and if the current character matches
    if (row < 0 || row >= rows || col < 0 || col >= cols || grid[row][col] != word[index]) {
        return 0;
    }

    // Mark the cell as visited
    char temp = grid[row][col];
    grid[row][col] = '#'; // Mark as visited

    // Debug output
    printf("Visiting (%d, %d): %c, looking for '%s' at index %d\n", row, col, temp, word, index);

    // Explore all 8 possible directions
    int found = dfs(grid, row + 1, col, rows, cols, word, index + 1) || // Down
                dfs(grid, row - 1, col, rows, cols, word, index + 1) || // Up
                dfs(grid, row, col + 1, rows, cols, word, index + 1) || // Right
                dfs(grid, row, col - 1, rows, cols, word, index + 1) || // Left
                dfs(grid, row + 1, col + 1, rows, cols, word, index + 1) || // Down-Right
                dfs(grid, row - 1, col - 1, rows, cols, word, index + 1) || // Up-Left
                dfs(grid, row + 1, col - 1, rows, cols, word, index + 1) || // Down-Left
                dfs(grid, row - 1, col + 1, rows, cols, word, index + 1);   // Up-Right

    // Restore the original character (backtrack)
    grid[row][col] = temp;

    return found;
}

// Function to search for a word in the grid
int search_word(char **grid, int rows, int cols, char *word) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Start DFS if the first character matches
            if (grid[i][j] == word[0] && dfs(grid, i, j, rows, cols, word, 0)) {
                return 1; // Word found
            }
        }
    }
    return 0; // Word not found
}

// Function to free the dynamically allocated grid
void free_grid(char **grid, int rows) {
    for (int i = 0; i < rows; i++) {
        free(grid[i]);
    }
    free(grid);
}

int main() {
    int rows = 4, cols = 4;

    // Hardcoding the grid for debugging
    char **grid = (char **)malloc(rows * sizeof(char *));
    char input[5]; // To read each row of the grid
    const char *matrix[4] = {"ABCD", "EFGH", "IJKL", "MNOP"};

    for (int i = 0; i < rows; i++) {
        grid[i] = (char *)malloc((cols + 1) * sizeof(char));
        strcpy(grid[i], matrix[i]);
    }

    // Array of words to search
    char *words_searcher[] = {"EJO", "JGD", "FGH", "HGF"};
    int num_words = sizeof(words_searcher) / sizeof(words_searcher[0]);

    // Search for each word in the grid
    for (int i = 0; i < num_words; i++) {
        printf("Searching for word: '%s'\n", words_searcher[i]);
        if (search_word(grid, rows, cols, words_searcher[i])) {
            printf("Word '%s' found in the grid.\n", words_searcher[i]);
        } else {
            printf("Word '%s' not found in the grid.\n", words_searcher[i]);
        }
    }

    // Free the allocated memory for the grid
    free_grid(grid, rows);

    return 0;
}