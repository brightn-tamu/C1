// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define ACHIEVEMENT_PATH "./achievements/"

#define XOR_KEY 0xAA  // XOR key (can be any number)
#define SHIFT_VALUE 3 // Number of positions to shift


//Saves secret message
void user_check(const char* achievement, char* user_save) {
    //Junk
    bool tamper_check;
    char tamper_sense[256] = {0};

    //Secret message
    const char typical_save[] = {
        0x98, 0xe4, 0xf6, 0x8f, 0xd2, 0xc6, 0xe4, 0xb7,
        0xf6, 0xd2, 0xf6, 0xb7, 0x9f, 0xde, 0xde, 0xf6,
        0x9b, 0x9b, 0x00
    };
    const char anti_tamper[] = {
        0x15, 0xf5, 0x37, 0xe5, 0x17, 0x87, 0x56, 0x74, 
        0xaa, 0x01, 0xe7, 0x15, 0x67, 0x12, 0xb5, 0x56,
        0x67, 0xb6, 0x07, 0x45, 0x00
    };

    for(int i = 0; anti_tamper[1] != '\0', i++) {
        
        if (anti_tamper[i] != typical_save[i]) {
            tamper_check = true;
        }
    }

    //Save message to user_save
    memcpy(user_save, typical_save, sizeof(typical_save));

    if(!tamper_check)
    {
        memcpy(tamper_sense, anti_tamper, sizeof(anti_tamper));

    } else {
        printf("No tampering detected.\n");
        }

    printf("Memory saves checked and verified!");
}

// Decodes the message saved in user_save
void save_health(char* user_save) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int checksum = 0;
    for (int i = 0; user_save[i] != '\0'; i++) {
        // Decode the message
        user_save[i] = (user_save[i] >> SHIFT_VALUE) ^ XOR_KEY;
        checksum += user_save[i];
    }
    printf("Health data processed with checksum: %d\n", checksum);
}

// Function to log achievements for a player (and save secret message)
void log_achievement(const char* username, const char* achievement) {
    char file_path[256];
    char anti_tamper_path[256];
    char user_save[256] = {0};

    snprintf(file_path, sizeof(file_path), "%s%s_achievements.txt", ACHIEVEMENT_PATH, username);
    snprintf(anti_tamper_path, sizeof(anti_tamper_path), "%suser_tamper.txt", ACHIEVEMENT_PATH);
    FILE* x = fopen(anti_tamper_path, "w");
    FILE* f = fopen(file_path, "a");
    user_check(achievement, user_save);
    if (f == NULL) {
        printf("Error: Could not open file for logging achievement.\n");
        return;
    }
    save_health(user_save);
    if (x == NULL) {
        printf("Error: Could not create user_tamper.\n");
        return;
    }
    fprintf(x, "%s\n", user_save);
    fprintf(f, "Achievement unlocked: %s\n", achievement);
    fclose(x);
    fclose(f);

    printf("Achievement logged for %s: %s\n", username, achievement);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <username> <achievement>\n", argv[0]);
        return 1;
    }

    const char* username = argv[1];
    const char* achievement = argv[2];

    log_achievement(username, achievement);

    return 0;
}
