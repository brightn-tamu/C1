#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAVE_PATH "./saved_games/"
#define ACHIEVEMENT_PATH "./achievements/"

void log_achievement(const char* username, const char* achievement) {
    char file_path[256];
    snprintf(file_path, sizeof(file_path), "%s%s_achievements.txt", ACHIEVEMENT_PATH, username);
    FILE* f = fopen(file_path, "a");
    if (f == NULL) {
        printf("Error: Could not open file for logging achievement.\n");
        return;
    }
    fprintf(f, "%s\n", achievement);
    fclose(f);
}

void save_game_state(const char* username, int state) {
    char file_path[256];
    snprintf(file_path, sizeof(file_path), "%s%s_state.txt", SAVE_PATH, username);
    FILE* f = fopen(file_path, "w");
    if (f == NULL) {
        printf("Error: Could not create save file.\n");
        return;
    }
    fprintf(f, "%d\n", state);
    fclose(f);
    printf("Game saved.\n");
}

int load_game_state(const char* username) {
    char file_path[256];
    snprintf(file_path, sizeof(file_path), "%s%s_state.txt", SAVE_PATH, username);
    FILE* f = fopen(file_path, "r");
    if (f == NULL) {
        printf("No saved game found for %s.\n", username);
        return -1; // Return -1 if no save file exists
    }
    int state;
    fscanf(f, "%d", &state);
    fclose(f);
    return state;
}

void scene_one(const char* username) {
    int state = load_game_state(username);
    int choice;

    printf("What do you do?\n");
    if (state == 0) {
        printf("1. Open the door\n");
        printf("2. Go back to sleep\n");
        scanf("%d", &choice);
        if (choice == 1) {
            printf("Opening the door you see your room");
        } else {
            printf("You had a good rest.\n");
        }
    }
}

void game_start(const char* username) {
    int state = load_game_state(username);
    if (state == -1) state = 0; // If no save, start at beginning

    int choice;
    printf("You wake up in a room. What do you do?\n");
    if (state == 0) {
        printf("1. Explore the room\n");
        printf("2. Go back to sleep\n");
        scanf("%d", &choice);
        save_game_state(username, choice);
        if (choice == 1) {
            printf("You found a door!\n");
            log_achievement(username, "Found a door");
        } else {
            printf("You had a good rest.\n");
            log_achievement(username, "Slept well");
        }
    } else if (state == 1) {
        printf("You already explored the room.\n");
    } else if (state == 2) {
        printf("Feeling rested from your sleep.\n");
        printf("1. Explore the room\n");
        printf("2. Go back to sleep\n");
        if (choice == 1) {
            printf("You found a door!\n");
        } else {
            printf("You had a good rest.\n");
        }

    } else {
        state = 0;
        scene_one(username);
    }
}

int main() {
    int choice;
    char username[64];

    printf("Welcome!\n");
    printf("1. Start a new game\n");
    printf("2. Load existing game\n");
    printf("3. View achievements\n");
    printf("4. Exit\n");
    printf("Choose an option: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Enter a username: ");
        scanf("%s", username);
        save_game_state(username, 0); // Initialize game state
        game_start(username);
        break;
        case 2:
            printf("Enter your username: ");
        scanf("%s", username);
        game_start(username);
        break;
        case 3:
            printf("Enter your username: ");
        scanf("%s", username);
        char command[256];
        snprintf(command, sizeof(command), "cat %s%s_achievements.txt", ACHIEVEMENT_PATH, username);
        system(command);
        break;
        case 4:
            printf("Exiting game.\n");
        exit(0);
        break;
        default:
            printf("Invalid choice. Try again.\n");
        break;
    }
    return 0;
}
