#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 100
#define MAX_ROWS 4
#define MAX_COLS 4
#define MAX_LINE_LENGTH 100

// Function prototypes
int dfs(char **grid, int row, int col, int rows, int cols, char *word, int index);
int search_word(char **grid, int rows, int cols, char *word);
void free_grid(char **grid, int rows);

int main() {
    int rows = MAX_ROWS, cols = MAX_COLS;
    char filename[] = "grid.txt"; // Specify your filename here
    FILE *arq = fopen(filename, "r");

    if (arq == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char **grid = (char **)malloc(rows * sizeof(char *));
    for (int i = 0; i < rows; i++) {
        grid[i] = (char *)malloc((cols + 1) * sizeof(char));
        if (fgets(grid[i], MAX_LINE_LENGTH, arq) == NULL) {
            perror("Error reading line");
            free_grid(grid, i); // Free already allocated rows
            fclose(arq);
            return EXIT_FAILURE;
        }

        // Remove newline character if present
        grid[i][strcspn(grid[i], "\n")] = 0;

        // Parse the string and fill the grid, skipping spaces
        int j = 0;
        for (const char *p = grid[i]; *p != '\0'; p++) {
            if (*p != ' ') {
                grid[i][j++] = *p;
            }
        }
        grid[i][j] = '\0'; // Null-terminate the string
    }

    fclose(arq); // Close the file after reading

    // Array of words to search
    char *words_searcher[] = {"ABC", "AFKP"};
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

// Function to check if the word exists in the grid
int dfs(char **grid, int row, int col, int rows, int cols, char *word, int index) {
    if (index == strlen(word)) {
        return 1;
    }

    if (row < 0 || row >= rows || col < 0 || col >= cols || grid[row][col] != word[index]) {
        return 0;
    }

    char temp = grid[row][col];
    grid[row][col] = '#'; // Mark as visited

    int found = dfs(grid, row + 1, col, rows, cols, word, index + 1) || // Down
                dfs(grid, row - 1, col, rows, cols, word, index + 1) || // Up
                dfs(grid, row, col + 1, rows, cols, word, index + 1) || // Right
                dfs(grid, row, col - 1, rows, cols, word, index + 1) || // Left
                dfs(grid, row + 1, col + 1, rows, cols, word, index + 1) || // Down-Right
                dfs(grid, row - 1, col - 1, rows, cols, word, index + 1) || // Up-Left
                dfs(grid, row + 1, col - 1, rows, cols, word, index + 1) || // Down-Left
                dfs(grid, row - 1, col + 1, rows, cols, word, index + 1);   // Up-Right

    grid[row][col] = temp; // Restore the original character

    return found;
}

// Function to search for a word in the grid
int search_word(char **grid, int rows, int cols, char *word) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
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