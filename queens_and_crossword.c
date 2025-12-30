#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// =============================================================
// Constants & Defines
// =============================================================

// Queens Battle Constants
#define BOARD_MAX_ROWS_QUEENS 20

// Crossword Generator Constants
#define BOARD_MAX_ROWS_CROSSWORD 30
#define MAX_WORDS 100
#define MAX_SLOTS 100
#define MAX_WORD_SIZE 15
#define HORIZONTAL 0
#define VERTICAL 1

// =============================================================
// Function Prototypes
// =============================================================

// --- Drivers (Menu Functions) ---
void runQueensBattle();
void runCrosswordGenerator();

// --- Queens Battle Helpers ---
int task4_queens_battle_rec(char board[BOARD_MAX_ROWS_QUEENS][BOARD_MAX_ROWS_QUEENS],
                            int boardSize,
                            int queensFlags[BOARD_MAX_ROWS_QUEENS][BOARD_MAX_ROWS_QUEENS],
                            int i,
                            int j,
                            int queensPlaced);

int doesRegionContainQueen(char board[BOARD_MAX_ROWS_QUEENS][BOARD_MAX_ROWS_QUEENS],
                           int boardSize,
                           int queensFlags[BOARD_MAX_ROWS_QUEENS][BOARD_MAX_ROWS_QUEENS],
                           int i,
                           int j,
                           char regionLetter);

int doesAdjacentCellsContainQueen(int boardSize,
                                  int queensFlags[BOARD_MAX_ROWS_QUEENS][BOARD_MAX_ROWS_QUEENS],
                                  int i,
                                  int j);

int ColUpContainQueen(int queensFlags[BOARD_MAX_ROWS_QUEENS][BOARD_MAX_ROWS_QUEENS], int i, int j);

// --- Crossword Generator Helpers ---
int task5_crossword_generator_rec(char board[BOARD_MAX_ROWS_CROSSWORD][BOARD_MAX_ROWS_CROSSWORD],
                                  int slots[MAX_SLOTS][4],
                                  int slotsNum,
                                  int curSlotInd,
                                  char words[MAX_WORDS][MAX_WORD_SIZE + 1],
                                  int curWordInd,
                                  int wordsNum,
                                  int slotsFilled,
                                  int wordsUsed[MAX_WORDS],
                                  int numWordsOnCell[BOARD_MAX_ROWS_CROSSWORD][BOARD_MAX_ROWS_CROSSWORD]);

int canWordBePlacedOnSlot(char board[BOARD_MAX_ROWS_CROSSWORD][BOARD_MAX_ROWS_CROSSWORD],
                          char word[MAX_WORD_SIZE + 1],
                          int wordCurChInd,
                          int slotStartRow,
                          int slotStartCol,
                          int slotLen,
                          int slotDir);

void placeOrUnplaceWordOnSlot(char board[BOARD_MAX_ROWS_CROSSWORD][BOARD_MAX_ROWS_CROSSWORD],
                              char word[MAX_WORD_SIZE + 1],
                              int wordCurChInd,
                              int slotStartRow,
                              int slotStartCol,
                              int slotLen,
                              int slotDir,
                              int isPlacing,
                              int numWordsOnCell[BOARD_MAX_ROWS_CROSSWORD][BOARD_MAX_ROWS_CROSSWORD]);

// =============================================================
// Main Application Loop
// =============================================================

int main()
{
    int task = -1;
    do
    {
        printf("\n=== Constraint Satisfaction Solver Suite ===\n");
        printf("Choose an option:\n"
               "1. Queens Battle Solver (Backtracking with Region Constraints)\n"
               "2. Crossword Generator (CSP Solver)\n"
               "3. Exit\n");
        printf("Your choice: ");

        if (scanf("%d", &task) == 1)
        {
            switch (task)
            {
            case 1:
                runQueensBattle();
                break;
            case 2:
                runCrosswordGenerator();
                break;
            case 3:
                printf("Goodbye!\n");
                break;
            default:
                printf("Please choose a task number from the list.\n");
                break;
            }
        }
        else
        {
            // Clear buffer if input is invalid
            scanf("%*s");
        }

    } while (task != 3);

    return 0;
}

// =============================================================
// PART 1: Queens Battle Implementation
// =============================================================

void runQueensBattle()
{
    char board[BOARD_MAX_ROWS_QUEENS][BOARD_MAX_ROWS_QUEENS];
    int queensFlags[BOARD_MAX_ROWS_QUEENS][BOARD_MAX_ROWS_QUEENS] = {0};
    int boardSize;

    // Input dimensions and board layout
    printf("Please enter the board dimensions:\n");
    scanf("%d", &boardSize);

    printf("Please enter the %d*%d puzzle board (use letters for regions):\n", boardSize, boardSize);
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            scanf(" %c", &board[i][j]);
        }
    }

    // Start Recursion
    if (task4_queens_battle_rec(board, boardSize, queensFlags, 0, 0, 0))
    {
        printf("\nSolution:\n");
        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                printf("%c ", queensFlags[i][j] ? 'X' : '*');
            }
            printf("\n");
        }
    }
    else
    {
        printf("This puzzle cannot be solved.\n");
    }
}

/**
 * Checks constraints: Is there already a queen in this specific color region?
 * Uses recursive flood-fill to check connected region cells.
 */
int doesRegionContainQueen(char board[BOARD_MAX_ROWS_QUEENS][BOARD_MAX_ROWS_QUEENS],
                           int boardSize,
                           int queensFlags[BOARD_MAX_ROWS_QUEENS][BOARD_MAX_ROWS_QUEENS],
                           int i,
                           int j,
                           char regionLetter)
{
    // Boundary and validity checks
    if (i < 0 || j < 0 || i >= boardSize || j >= boardSize ||
        board[i][j] == '#' || board[i][j] != regionLetter)
        return 0;

    // Found a queen in the region
    if (queensFlags[i][j] == 1)
        return 1;

    // Mark visited
    board[i][j] = '#';

    int foundQueen = doesRegionContainQueen(board, boardSize, queensFlags, i, j + 1, regionLetter) ||
                     doesRegionContainQueen(board, boardSize, queensFlags, i, j - 1, regionLetter) ||
                     doesRegionContainQueen(board, boardSize, queensFlags, i + 1, j, regionLetter) ||
                     doesRegionContainQueen(board, boardSize, queensFlags, i - 1, j, regionLetter);

    // Backtrack (Unmark)
    board[i][j] = regionLetter;
    return foundQueen;
}

/**
 * Checks adjacency constraints (Up, Up-Left, Up-Right).
 * Since we fill row by row, we only need to check the rows above.
 */
int doesAdjacentCellsContainQueen(int boardSize,
                                  int queensFlags[BOARD_MAX_ROWS_QUEENS][BOARD_MAX_ROWS_QUEENS],
                                  int i,
                                  int j)
{
    if ((i > 0 && j > 0 && queensFlags[i - 1][j - 1] == 1) ||            // Top-Left
        (i > 0 && queensFlags[i - 1][j] == 1) ||                         // Top
        (i > 0 && (j < boardSize - 1) && queensFlags[i - 1][j + 1] == 1)) // Top-Right
        return 1;

    return 0;
}

/**
 * Checks if a Queen exists in the column above.
 */
int ColUpContainQueen(int queensFlags[BOARD_MAX_ROWS_QUEENS][BOARD_MAX_ROWS_QUEENS], int i, int j)
{
    if (i < 0)
        return 0;

    if (queensFlags[i][j] == 1)
        return 1;

    return ColUpContainQueen(queensFlags, i - 1, j);
}

/**
 * Core Backtracking Algorithm for N-Queens with Constraints.
 */
int task4_queens_battle_rec(char board[BOARD_MAX_ROWS_QUEENS][BOARD_MAX_ROWS_QUEENS],
                            int boardSize,
                            int queensFlags[BOARD_MAX_ROWS_QUEENS][BOARD_MAX_ROWS_QUEENS],
                            int i,
                            int j,
                            int queensPlaced)
{
    // Base Case: All queens placed
    if (queensPlaced == boardSize)
        return 1;

    // Base Case: Out of bounds
    if (i >= boardSize)
        return 0;

    int canQBePlacedOnCurCell = 1;

    // Check all constraints
    int doesRegionContainQueenRes = doesRegionContainQueen(board, boardSize, queensFlags, i, j, board[i][j]);
    int doesAdjacentCellsContainQueenRes = doesAdjacentCellsContainQueen(boardSize, queensFlags, i, j);
    int ColUpContainQueenRes = ColUpContainQueen(queensFlags, i - 1, j);

    if (doesRegionContainQueenRes || doesAdjacentCellsContainQueenRes || ColUpContainQueenRes)
    {
        canQBePlacedOnCurCell = 0;
    }

    // Try placing a queen
    if (canQBePlacedOnCurCell)
    {
        queensFlags[i][j] = 1;

        // Recurse to next row (i+1)
        if (task4_queens_battle_rec(board, boardSize, queensFlags, i + 1, 0, queensPlaced + 1))
            return 1;

        // Backtrack
        queensFlags[i][j] = 0;
    }

    // Try next column in current row
    if (j < boardSize - 1)
        return task4_queens_battle_rec(board, boardSize, queensFlags, i, j + 1, queensPlaced);

    return 0;
}

// =============================================================
// PART 2: Crossword Generator Implementation
// =============================================================

void runCrosswordGenerator()
{
    char board[BOARD_MAX_ROWS_CROSSWORD][BOARD_MAX_ROWS_CROSSWORD];
    char words[MAX_WORDS][MAX_WORD_SIZE + 1];
    int slots[MAX_SLOTS][4]; // [Row, Col, Length, Direction]
    int boardSize, slotsNum, wordsNum;
    int wordsUsed[MAX_WORDS] = {0};
    int numWordsOnCell[BOARD_MAX_ROWS_CROSSWORD][BOARD_MAX_ROWS_CROSSWORD] = {0};

    // Initialize board
    for (int i = 0; i < BOARD_MAX_ROWS_CROSSWORD; i++)
    {
        for (int j = 0; j < BOARD_MAX_ROWS_CROSSWORD; j++)
        {
            board[i][j] = '#';
        }
    }

    printf("Please enter the dimensions of the crossword grid:\n");
    scanf("%d", &boardSize);

    printf("Please enter the number of slots in the crossword:\n");
    scanf("%d", &slotsNum);

    printf("Please enter the details for each slot (Row Column Length Direction[H/V]):\n");
    for (int i = 0; i < slotsNum; i++)
    {
        char curSlotDir;
        scanf("%d %d %d %c", &slots[i][0], &slots[i][1], &slots[i][2], &curSlotDir);
        slots[i][3] = curSlotDir == 'H' ? HORIZONTAL : VERTICAL;
    }

    printf("Please enter the number of words in the dictionary:\n");
    do
    {
        scanf("%d", &wordsNum);
        if (wordsNum < slotsNum)
        {
            printf("The dictionary must contain at least %d words. Please enter a valid dictionary size:\n", slotsNum);
        }

    } while (wordsNum < slotsNum);

    printf("Please enter the words for the dictionary:\n");
    for (int i = 0; i < wordsNum; i++)
    {
        scanf("%s", words[i]);
    }

    // Start Recursion
    if (task5_crossword_generator_rec(
            board,
            slots,
            slotsNum,
            0,
            words,
            0,
            wordsNum,
            0,
            wordsUsed,
            numWordsOnCell))
    {
        // Print Result
        printf("\nSolution:\n");
        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                printf("| %c ", board[i][j]);
            }
            printf("|\n");
        }
    }
    else
    {
        printf("This crossword cannot be solved.\n");
    }
}

/**
 * Checks if a word fits in a slot (length matches, and no character conflicts).
 */
int canWordBePlacedOnSlot(char board[BOARD_MAX_ROWS_CROSSWORD][BOARD_MAX_ROWS_CROSSWORD],
                          char word[MAX_WORD_SIZE + 1],
                          int wordCurChInd,
                          int slotStartRow,
                          int slotStartCol,
                          int slotLen,
                          int slotDir)
{
    if (wordCurChInd == slotLen)
        return 1;

    int curCellRowInd = slotStartRow + (slotDir == VERTICAL ? wordCurChInd : 0);
    int curCellColInd = slotStartCol + (slotDir == HORIZONTAL ? wordCurChInd : 0);

    char curCellCh = board[curCellRowInd][curCellColInd];

    // Conflict check: if cell is not empty ('#') and doesn't match current char
    if (curCellCh != '#' && curCellCh != word[wordCurChInd])
        return 0;

    return canWordBePlacedOnSlot(board, word, wordCurChInd + 1, slotStartRow, slotStartCol, slotLen, slotDir);
}

/**
 * Updates the board to place or remove a word.
 * Maintains a reference count (numWordsOnCell) to handle intersecting words correctly.
 */
void placeOrUnplaceWordOnSlot(char board[BOARD_MAX_ROWS_CROSSWORD][BOARD_MAX_ROWS_CROSSWORD],
                              char word[MAX_WORD_SIZE + 1],
                              int wordCurChInd,
                              int slotStartRow,
                              int slotStartCol,
                              int slotLen,
                              int slotDir,
                              int isPlacing,
                              int numWordsOnCell[BOARD_MAX_ROWS_CROSSWORD][BOARD_MAX_ROWS_CROSSWORD])
{
    if (wordCurChInd == slotLen)
        return;

    int curCellRowInd = slotStartRow + (slotDir == VERTICAL ? wordCurChInd : 0);
    int curCellColInd = slotStartCol + (slotDir == HORIZONTAL ? wordCurChInd : 0);

    if (isPlacing)
    {
        board[curCellRowInd][curCellColInd] = word[wordCurChInd];
        numWordsOnCell[curCellRowInd][curCellColInd]++;
    }
    else
    {
        // Decrement usage count. Only reset to '#' if no other word is using this cell.
        numWordsOnCell[curCellRowInd][curCellColInd]--;
        if (numWordsOnCell[curCellRowInd][curCellColInd] <= 0)
            board[curCellRowInd][curCellColInd] = '#';
    }

    placeOrUnplaceWordOnSlot(board, word, wordCurChInd + 1, slotStartRow,
                             slotStartCol, slotLen, slotDir, isPlacing, numWordsOnCell);
}

/**
 * Core Recursive Function for Crossword Generation.
 * Iterates through slots and tries to fit available words.
 */
int task5_crossword_generator_rec(char board[BOARD_MAX_ROWS_CROSSWORD][BOARD_MAX_ROWS_CROSSWORD],
                                  int slots[MAX_SLOTS][4],
                                  int slotsNum,
                                  int curSlotInd,
                                  char words[MAX_WORDS][MAX_WORD_SIZE + 1],
                                  int curWordInd,
                                  int wordsNum,
                                  int slotsFilled,
                                  int wordsUsed[MAX_WORDS],
                                  int numWordsOnCell[BOARD_MAX_ROWS_CROSSWORD][BOARD_MAX_ROWS_CROSSWORD])
{
    // Base Case: All slots filled successfully
    if (slotsFilled == slotsNum)
        return 1;

    // Fail Case: Out of words to check for this slot
    if (curWordInd >= wordsNum)
        return 0;

    int curSlotStartRow = slots[curSlotInd][0];
    int curSlotStartCol = slots[curSlotInd][1];
    int curSlotLen = slots[curSlotInd][2];
    int curSlotDir = slots[curSlotInd][3];

    // Option 1: Try to place the current word in the current slot
    if (wordsUsed[curWordInd] == 0 &&
        ((size_t)curSlotLen == strlen(words[curWordInd])) &&
        canWordBePlacedOnSlot(board, words[curWordInd], 0, curSlotStartRow, curSlotStartCol, curSlotLen, curSlotDir))
    {

        // Place word
        placeOrUnplaceWordOnSlot(board, words[curWordInd], 0, curSlotStartRow, curSlotStartCol, curSlotLen, curSlotDir, 1, numWordsOnCell);
        wordsUsed[curWordInd] = 1;

        // Recurse to next slot (curSlotInd + 1), resetting word index to 0
        if (task5_crossword_generator_rec(board, slots, slotsNum, curSlotInd + 1, words, 0, wordsNum,
                                          slotsFilled + 1, wordsUsed, numWordsOnCell))
            return 1;

        // Backtrack: Remove word
        placeOrUnplaceWordOnSlot(board, words[curWordInd], 0, curSlotStartRow, curSlotStartCol, curSlotLen, curSlotDir, 0, numWordsOnCell);
        wordsUsed[curWordInd] = 0;
    }

    // Option 2: Current word didn't fit (or led to dead end), try next word for the SAME slot
    return task5_crossword_generator_rec(board, slots, slotsNum, curSlotInd, words, curWordInd + 1, wordsNum,
                                         slotsFilled, wordsUsed, numWordsOnCell);
}
