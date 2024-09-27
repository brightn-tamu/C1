#ifndef SERVER_H
#define SERVER_H

#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#define ACHIEVEMENT_PATH "./achievements/"
#define KEY 0xAA  // XOR key (can be any number)

// Type definition for convenience
typedef unsigned short uint16;
typedef unsigned long uint64;

// Structure for maintaining scene state
typedef struct {
    uint64 current_scene;
    uint64 previous_scene;
} SceneState;

// Function prototypes
void user_check(const char* achievement, char* user_save);
void save_health(char* user_save, char* user_health);
void decode(const unsigned char* encoded_message, char* decoded_message);
void log_achievement(const char* username, const char* achievement);

#endif // SERVER_H
