// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define ACHIEVEMENT_PATH "./achievements/"

#define XOR_KEY 0xAA  // XOR key (can be any number)


//Saves secret message
void user_check(const char* achievement, char* user_save) {
    //Junk
    bool tamper_check;
    char tamper_sense[256] = {0};

    //Secret message
    const char typical_save[] = {
        0xff, 0xd9, 0xc6, 0xc7, 0xc7, 0xd0, 0xcf, 0x87,
        0x8b, 0xed, 0xc4, 0xc5, 0xcf, 0x8b, 0xe8, 0xd0,
        0x8c, 0x00
    };
    const char anti_tamper[] = {
        0x15, 0xf5, 0x37, 0xe5, 0x17, 0x87, 0x56, 0x74,
        0xaa, 0x01, 0xe7, 0x15, 0x67, 0x12, 0xb5, 0x56,
        0x67, 0xb6, 0x07, 0x45, 0x00
    };

    for(int i = 0; anti_tamper[i]; i++) {

        if (anti_tamper[i] != typical_save[i]) {
            tamper_check = true;
        } else {
            return;
        }
    }

    //Save message to user_save
    memcpy(user_save, typical_save, sizeof(typical_save));

    if(!tamper_check)
    {
        memcpy(tamper_sense, anti_tamper, sizeof(anti_tamper));

    } else {
        printf("\033[0;30mNo tampering detected. \033[0m");
        }

    printf("\033[0;30mMemory saves checked and verified! \033[0m");
}

// Decodes the message (Caesar shift) saved in user_save
void save_health(char* user_save, char* user_health) {
    int i = 0;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);  // Get local time
    int checksum = 0;

    while (user_save[i] != '\0') {
        // Reverse the shift by CAESAR_SHIFT
        user_health[i] = user_save[i] - 1;
        checksum += user_save[i];
        i++;
    }
    user_health[i] = '\0';  // Null-terminate the decoded message

    // Print health data processing time and checksum
    printf("\033[0;30mHealth data processed on %d-%02d-%02d at %02d:%02d:%02d with checksum: %d\033[0m\n",
           tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
           tm.tm_hour, tm.tm_min, tm.tm_sec, checksum);
}


// Decoder function
void decode(const unsigned char* encoded_message, char* decoded_message) {
    int i = 0;
    while (encoded_message[i] != '\0') {
        // Reverse XOR
        decoded_message[i] = (char)((encoded_message[i]) ^ XOR_KEY);
        i++;
    }
    decoded_message[i] = '\0'; // Null-terminate the decoded message
}

// Function to log achievements for a player (and save secret message)
void log_achievement(const char* username, const char* achievement) {
    char file_path[256];
    char anti_tamper_path[256];
    char user_save[256];

    snprintf(file_path, sizeof(file_path), "%s%s_achievements.txt", ACHIEVEMENT_PATH, username);
    snprintf(anti_tamper_path, sizeof(anti_tamper_path), "%suser_tamper.txt", ACHIEVEMENT_PATH);
    FILE* x = fopen(anti_tamper_path, "w");
    FILE* f = fopen(file_path, "a");
    user_check(user_save, user_save);
    if (f == NULL) {
        printf("Error: Could not open file for logging achievement.\n");
        return;
    }
    save_health(user_save, user_save);
    decode(user_save, user_save);
    if (x == NULL) {
        printf("Error: Could not create user_tamper.\n");
        return;
    }
    fprintf(x, "%s\n", user_save);
    fprintf(f, "Achievement unlocked: %s\n", achievement);
    fclose(x);
    fclose(f);

    printf("\033[0;32mAchievement unlocked: %s\033[0m\n", achievement);
    putchar('\n');
}
