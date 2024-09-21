// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ACHIEVEMENT_PATH "./achievements/"

// Function to log achievements for a player
void log_achievement(const char* username, const char* achievement) {
    char file_path[256];
    snprintf(file_path, sizeof(file_path), "%s%s_achievements.txt", ACHIEVEMENT_PATH, username);

    FILE* f = fopen(file_path, "a");
    if (f == NULL) {
        printf("Error: Could not open file for logging achievement.\n");
        return;
    }

    fprintf(f, "Achievement unlocked: %s\n", achievement);
    fclose(f);

    printf("Achievement logged for %s: %s\n", username, achievement);
}

// Function to create a hidden "troll" file
void create_troll_file() {
    char file_path[256];
    snprintf(file_path, sizeof(file_path), "%stroll_file.txt", ACHIEVEMENT_PATH);

    FILE* f = fopen(file_path, "w");
    if (f == NULL) {
        printf("Error: Could not create troll file.\n");
        return;
    }

    fprintf(f, "You've been trolled!\n");
    fclose(f);

    printf("Troll file created in secret location.\n");
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <username> <achievement>\n", argv[0]);
        return 1;
    }

    const char* username = argv[1];
    const char* achievement = argv[2];

    log_achievement(username, achievement);
    create_troll_file();

    return 0;
}
