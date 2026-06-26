#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define MAZE_SIZE 21
#define MAX_INPUT_LEN 32

typedef struct {
    char grid[MAZE_SIZE][MAZE_SIZE];
    unsigned int seed;
    bool debug_mode;
} Maze;

void generate_random_maze(Maze *m) {
    srand(m->seed);
    
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            if (i == 0 || i == MAZE_SIZE - 1 || j == 0 || j == MAZE_SIZE - 1) {
                m->grid[i][j] = '#';
            } else {
                if (rand() % 4 == 0) {
                    m->grid[i][j] = '#';
                } else {
                    m->grid[i][j] = '.';
                }
            }
        }
    }
}

void print_maze(const Maze *m) {
    printf("\n--- LUNA HACKLEY MAZE GENERATOR ---\n");
    printf("Current Seed: %u\n", m->seed);
    
    if (m->debug_mode) {
        printf("[DEBUG] Grid dimensions: %dx%d\n", MAZE_SIZE, MAZE_SIZE);
        printf("[DEBUG] Memory address of grid: %p\n", (void*)m->grid);
    }

    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            printf("%c ", m->grid[i][j]);
        }
        printf("\n");
    }
    printf("------------------------------------\n");
}

void export_to_file(const Maze *m) {
    FILE *fp = fopen("maze_export.txt", "w");
    if (fp == NULL) {
        perror("Failed to export canvas");
        return;
    }
    
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            fprintf(fp, "%c ", m->grid[i][j]);
        }
        fprintf(fp, "\n");
    }
    
    fclose(fp);
    printf("Successfully exported maze to maze_export.txt\n");
}

int main(int argc, char *argv[]) {
    Maze myMaze;
    myMaze.debug_mode = false;
    char input_buffer[MAX_INPUT_LEN];
    char command[MAX_INPUT_LEN];

    printf("Initialize system... Ready.\n");
    printf("Commands: [seed <val>] [run] [debug] [export] [exit]\n");

    while (1) {
        printf("> ");
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) break;
        
        input_buffer[strcspn(input_buffer, "\n")] = 0;
        
        if (strcmp(input_buffer, "exit") == 0) {
            break;
        } else if (strncmp(input_buffer, "seed", 4) == 0) {
            myMaze.seed = (unsigned int)atoi(input_buffer + 5);
            printf("Seed set to %u\n", myMaze.seed);
        } else if (strcmp(input_buffer, "run") == 0) {
            generate_random_maze(&myMaze);
            print_maze(&myMaze);
        } else if (strcmp(input_buffer, "debug") == 0) {
            myMaze.debug_mode = !myMaze.debug_mode;
            printf("Debug mode: %s\n", myMaze.debug_mode ? "ON" : "OFF");
        } else if (strcmp(input_buffer, "export") == 0) {
            export_to_file(&myMaze);
        } else {
            printf("Unknown command. Try: seed <n>, run, debug, export, exit\n");
        }
    }

    printf("System shutting down.\n");
    return 0;
}
