#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scenes.h"
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <inttypes.h>


#define SAVE_PATH "./saved_games/"
#define ACHIEVEMENT_PATH "./achievements/"
#define MAX_ACHIEVEMENT_LENGTH 50

typedef unsigned short uint16;
typedef unsigned long uint64;
typedef struct {
    uint64 current_scene;
    uint64 previous_scene;
} SceneState;

/* std in/out helper functions */
void prompt(const char *s, const char *format, void *dest) {
    printf("%s", s);
    scanf(format, dest);
}

void clear_console() {
    system("clear");
}

void update_scene(SceneState *ss, uint64 next_scene, int clear) {
    ss->previous_scene = ss->current_scene;
    ss->current_scene = next_scene;
    if (clear) clear_console();
}

const unsigned char all_achievements[][MAX_ACHIEVEMENT_LENGTH] = {
    {0xeb, 0xd8, 0xde, 0xc3, 0xcc, 0xcb, 0xc9, 0xde, 0x00},
    {0xec, 0xcf, 0xc6, 0xc6, 0xcf, 0xce, 0x8a, 0xed, 0xc5, 0xc8, 0xc6, 0xc3, 0xc4, 0xd9, 0x00},
    {0xec, 0xc5, 0xd8, 0xcf, 0xd9, 0xde, 0xcf, 0xd8, 0x00},
    {0xed, 0xc5, 0xc8, 0xc6, 0xc3, 0xc4, 0x8d, 0xd9, 0x8a, 0xf8, 0xcf, 0xd9, 0xda, 0xcf, 0xc9, 0xde, 0x00},
    {0xed, 0xdf, 0xcb, 0xd8, 0xce, 0xc3, 0xcb, 0xc4, 0x8a, 0xe1, 0xc3, 0xc6, 0xc6, 0xcf, 0xd8, 0x00},
    {0xed, 0xdf, 0xcb, 0xd8, 0xce, 0xc3, 0xcb, 0xc4, 0x8d, 0xd9, 0x8a, 0xf8, 0xcf, 0xd9, 0xda, 0xcf, 0xc9, 0xde, 0x00},
    {0xe7, 0xcb, 0xcd, 0xc3, 0xc9, 0xcb, 0xc6, 0x8a, 0xfd, 0xcf, 0xcb, 0xda, 0xc5, 0xc4, 0x00},
    {0xe7, 0xc5, 0xdf, 0xc4, 0xde, 0xcb, 0xc3, 0xc4, 0xcf, 0xcf, 0xd8, 0x00},
    {0xe4, 0xcf, 0xdd, 0x8a, 0xeb, 0xce, 0xdc, 0xcf, 0xc4, 0xde, 0xdf, 0xd8, 0xcf, 0x00},
    {0xe4, 0xcf, 0xc9, 0xd8, 0xc5, 0xc7, 0xcb, 0xc4, 0xc9, 0xcf, 0xd8, 0x95, 0x00},
    {0xfa, 0xc5, 0xdd, 0xcf, 0xd8, 0x8a, 0xcb, 0xde, 0x8a, 0xed, 0xd8, 0xcf, 0xcb, 0xde, 0x8a, 0xe9, 0xc5, 0xd9, 0xde, 0x00},
    {0xfa, 0xc5, 0xdd, 0xcf, 0xd8, 0x8a, 0xcb, 0xde, 0x8a, 0xed, 0xd8, 0xcf, 0xcb, 0xde, 0xcf, 0xd9, 0xde, 0x8a, 0xe9, 0xc5, 0xd9, 0xde, 0x00},
    {0xf9, 0xde, 0xcf, 0xcb, 0xc6, 0xde, 0xc2, 0xd3, 0x8a, 0xed, 0xcf, 0xde, 0x8a, 0xeb, 0xdd, 0xcb, 0xd3, 0x00},
    {0xfe, 0xd8, 0xcb, 0xda, 0x8a, 0xef, 0xd2, 0xda, 0xcf, 0xd8, 0xde, 0x00},
    {0xfd, 0xcb, 0xc6, 0xc1, 0x8a, 0xfe, 0xc2, 0xc3, 0xd9, 0x8a, 0xfd, 0xcb, 0xd3, 0x00},
    {0xfd, 0xc3, 0xde, 0xde, 0xd3, 0x8a, 0xe8, 0xd3, 0xda, 0xcb, 0xd9, 0xd9, 0x00}
};

/*
List of achievments above
    "Artifact",
    "Felled Goblins",
    "Forester",
    "Goblin's Respect",
    "Guardian Killer",
    "Guardian's Respect",
    "Magical Weapon",
    "Mountaineer",
    "New Adventure",
    "Necromancer?",
    "Power at Great Cost",
    "Power at Greatest Cost",
    "Stealthy Get Away",
    "Trap Expert",
    "Walk This Way",
    "Witty Bypass"
*/


int total_achievements = sizeof(all_achievements) / sizeof(all_achievements[0]);


// forward declarations of helper functions
uint64 load_game_scene(const char *);
void display_achievements(const char *username);
void if_log_achievement(const char *, const char *);
void log_achievement(const char *, const char *);
void save_game_state(const char *, uint64);
void decode(const unsigned char* encoded_message, char* decoded_message);

// forward declaration for scenes array
void scene_exit(char *, SceneState *);
void scene_menu(char *, SceneState *);
void scene_tavern(char *, SceneState *);
void scene_forest(char *, SceneState *);
void scene_mountains(char *, SceneState *);
void scene_goblin_lair(char *, SceneState *);
void scene_dungeon(char *, SceneState *);
void scene_goblin_favor(char *, SceneState *);
void scene_guardian(char *, SceneState *);
void scene_cave(char *, SceneState *);
void scene_return(char *, SceneState *);
void scene_death(char *, SceneState *);

static void  (*scenes[])(char *, SceneState *) = {
[SCENE_exit] =           scene_exit,
[SCENE_menu]  =          scene_menu,
[SCENE_tavern] =         scene_tavern,
[SCENE_forest]  =        scene_forest,
[SCENE_mountains] =      scene_mountains,
[SCENE_goblin_lair] =    scene_goblin_lair,
[SCENE_dungeon] =        scene_dungeon,
[SCENE_goblin_favor] =   scene_goblin_favor,
[SCENE_guardian] =       scene_guardian,
[SCENE_cave] =           scene_cave,
[SCENE_return] =         scene_return,
[SCENE_death] =          scene_death,
};

/* This is always the final scene - the program is terminated from here */
void scene_exit(char *username, SceneState *ss) {
    save_game_state(username, ss->previous_scene);
    exit(0);
}


void scene_menu(char *username, SceneState *ss) {
    uint16 choice = 0;

    prompt("Welcome!\n"\
            "1. Start a new game\n"\
            "2. Load an existing game\n"\
            "3. View achievements\n"\
            "9. Exit\n"\
            "Choose an option: ",
            "%hu",
            &choice);

    switch (choice) {
        case 1:
            prompt("Enter a username: ", "%s", username);
            update_scene(ss, SCENE_tavern, 1); // tavern is the first scene
            break;
        case 2:
            prompt("Enter a username: ", "%s", username);
            clear_console();
            if (( (ss->current_scene = load_game_scene(username)) == SCENE_exit)) {
                printf("Could not find an existing game for user '%s'.\n\n", username);
                ss->current_scene = SCENE_menu; // reset the scene to menu
            }
            break;
        case 3:
            prompt("Enter a username: ", "%s", username);
            clear_console();
            display_achievements(username);
            /* scene is not updated so that the next scene remains the menu */
            break;
        case 9:
            printf("Exiting game.\n");
            exit(0);
        default:
            clear_console();
            /* again, scene not updated to keep the menu showing */
            printf("'%hu' is an invalid choice. Try again.\n\n", choice);
            break;
    }
}


void scene_tavern(char *username, SceneState *ss) {
    uint16 choice = 0;

    prompt("You find yourself in a dimly lit tavern, filled with rowdy patrons. A hooded figure sits in the corner, gesturing for you to approach.\n"\
           "What do you do?\n"\
           "1. Sit down with the hooded figure.\n"\
           "2. Ignore the figure and stay at the bar.\n"\
           "9. Save and exit.\n"\
           "Choose an option: ",
           "%hu",
           &choice);

    switch(choice) {
        case 1:
            update_scene(ss, SCENE_forest, 1);
            if_log_achievement(username, "Forester");
            printf("The figure offers a dangerous quest to retrieve an ancient artifact from a cursed dungeon.\n");
            break;
        case 2:
            update_scene(ss, SCENE_mountains, 1);
            if_log_achievement(username, "Mountaineer");
            printf("You ignore the man but instead hear the bartender's whispers of a rumor, a lost treasure hidden deep in the mountains.\n");
            printf("You leave to find this treasure.\n");
            break;
        case 9:
            printf("Exiting game.\n");
            update_scene(ss, SCENE_exit, 0);
            break;
        default:
            clear_console();
            printf("'%hu' is an invalid choice. Try again.\n\n", choice);
            break;
    }
}

void scene_forest(char *username, SceneState *ss) {
    uint16 choice = 0;

    prompt("Deep in the eerie, dark woods, you are alone on your way to the cursed dungeon when you hear rustling in the bushes.\n"\
           "What do you do?\n"\
           "1. Investigate the noise.\n"\
           "2. Ignore the noise and keep moving.\n"\
           "9. Save and exit.\n"\
           "Choose an option: ",
           "%hu",
           &choice);

    switch(choice) {
        case 1:
            invalid_ambush_choice:
            clear_console();
            prompt("You are ambushed by a group of goblins!\n"\
                   "PRESS *1* TO TRY TO FIGHT BACK\n"\
                   "PRESS *2* TO SUBMIT QUIETLY\n",
                   "%hu",
                   &choice);

            switch(choice) {
                case 1:
                    if (((rand() % 20) + 1) >= 7) {
                        update_scene(ss, SCENE_dungeon, 1);
                        printf("You have defeated the goblins and can now claim their loot as you continue on your quest!\n");
                        if_log_achievement(username, "Felled Goblins");
                    } else {
                        update_scene(ss, SCENE_goblin_lair, 1);
                        printf("The goblins defeated you and you are now their prisoner!\n");
                    }
                    break;
                case 2:
                    update_scene(ss, SCENE_goblin_lair, 1);
                    printf("You are captured by the goblins.\n");
                    break;
                default:
                    printf("'%hu' is an invalid choice. Try again.\n\n", choice);
                    goto invalid_ambush_choice;
            }
            break;
        case 2:
            update_scene(ss, SCENE_dungeon, 1);
            printf("You ignore the noise and continue on your quest with a strange feeling you are being watched...\n");
            break;
        case 9:
            printf("Exiting game.\n");
            update_scene(ss, SCENE_exit, 0);
            break;
        default:
            clear_console();
            printf("'%hu' is an invalid choice. Try again.\n\n", choice);
            break;
    }
}

void scene_mountains(char *username, SceneState *ss){
    uint16 choice = 0;

    prompt("You traverse treacherous mountain paths, the wind howling around you, when you come across an injured traveler lying on the ground.\n"\
           "What do you do?\n"\
           "1. Help the traveler.\n"\
           "2. Leave the traveler.\n"\
           "9. Save and exit.\n"\
           "Choose an option: ",
           "%hu",
           &choice);

    switch(choice) {
        case 1:
            update_scene(ss, SCENE_cave, 1);
            if_log_achievement(username, "Magical Weapon");
            printf("The traveler reveals themselves to be a powerful wizard.\n");
            printf("He grants you his magical staff for your journey.\n");
            break;
        case 2:
            update_scene(ss, SCENE_cave, 1);
            printf("You reach the treasure cave but encounter a deadly trap... \n");
            break;
        case 9:
            printf("Exiting game.\n");
            update_scene(ss, SCENE_exit, 0);
            break;
        default:
            clear_console();
            printf("'%hu' is an invalid choice. Try again.\n\n", choice);
            break;
    }
}

void scene_goblin_lair(char *username, SceneState *ss){
    uint16 choice = 0;

    prompt("You are tied up in a grimy goblin cave, surrounded by laughing goblins.\n"\
           "What do you do?\n"\
           "1. Attempt to break free.\n"\
           "2. Bargain with the goblin chief.\n"\
           "9. Save and exit.\n"\
           "Choose an option: ",
           "%hu",
           &choice);

    switch(choice) {
        case 1:
            update_scene(ss, SCENE_dungeon, 1);
            if_log_achievement(username, "Stealthy Get Away");
            printf("You manage to slip away when the goblins aren't looking but are forced to leave some of your valuables behind.\n");
            break;
        case 2:
            update_scene(ss, SCENE_goblin_favor, 1);
            printf("The goblin chief offers you a deal: perform a task for them, and they will let the you go.\n");
            break;
        case 9:
            printf("Exiting game.\n");
            update_scene(ss, SCENE_exit, 0);
            break;
        default:
            clear_console();
            printf("'%hu' is an invalid choice. Try again.\n\n", choice);
            break;
    }
}

void scene_dungeon(char *username, SceneState *ss){
    uint16 choice = 0;

    prompt("The cursed dungeon looms before you, filled with traps, monsters, and treasure.\n"\
           "What do you do?\n"\
           "1. Enter cautiously.\n"\
           "2. Charge in boldly.\n"\
           "9. Save and exit.\n"\
           "Choose an option: ",
           "%hu",
           &choice);

    switch(choice) {
        case 1:
            update_scene(ss, SCENE_guardian, 1);
            if_log_achievement(username, "Trap Expert");
            printf("You avoid most traps but come face to face with a powerful guardian as you reach the deepest depths of the dungeon.\n");
            break;
        case 2:
            update_scene(ss, SCENE_guardian, 1);
            printf("You rush into the dungeon as many traps go off, causing you to wince in pain but giving you the opportunity to catch this dungeons guardian by surprise.\n");
            break;
        case 9:
            printf("Exiting game.\n");
            update_scene(ss, SCENE_exit, 0);
            break;
        default:
            clear_console();
            printf("'%hu' is an invalid choice. Try again.\n\n", choice);
            break;
    }
}

void scene_goblin_favor(char *username, SceneState *ss) {
    uint16 choice = 0;

    prompt("The goblin chief asks you to retrieve a stolen idol from a rival tribe of kobolds.\n"\
           "What do you do?\n"\
           "1. Accept the task.\n"\
           "2. Refuse the task.\n"\
           "9. Save and exit.\n"\
           "Choose an option: ",
           "%hu",
           &choice);

    switch(choice) {
        case 1:
            update_scene(ss, SCENE_return, 1);
            if_log_achievement(username, "Artifact");
            if_log_achievement(username, "Goblin's Respect");
            printf("You sneakily infiltrate the kobold camp as they sleep, retrieves the golden idol, and gain the favor of the goblin camp. In return, they offer you a rare artifact.\n");
            break;
        case 2:
            update_scene(ss, SCENE_dungeon, 1);
            printf("The goblins angrily try to kill you, forcing you to dash away without your supplies and tumble downhill and into the entrance of a imposing dungeon.\n");
            break;
        case 9:
            printf("Exiting game.\n");
            update_scene(ss, SCENE_exit, 0);
            break;
        default:
            clear_console();
            printf("'%hu' is an invalid choice. Try again.\n\n", choice);
            break;
    }
}

void scene_cave(char *username, SceneState *ss) {
    uint16 choice = 0;

    prompt("You finally reach the cave filled with treasure but senses an ancient magical presence guarding it.\n"\
           "What do you do?\n"\
           "1. Take the treasure.\n"\
           "2. Leave the treasure.\n"\
           "9. Save and exit.\n"\
           "Choose an option: ",
           "%hu",
           &choice);

    switch(choice) {
        case 1:
            update_scene(ss, SCENE_guardian, 1);
            printf("The guardian of the cave awakens.\n");
            break;
        case 2:
            update_scene(ss, SCENE_return, 1);
            if_log_achievement(username, "Artifact");
            if_log_achievement(username, "Guardian's Respect");
            printf("As you leave, the entity in the cave, its guardian, honors you for the reverence you have shown it and rewards you with a powerful magical artifact that enthralls you immendately.\n");
            break;
        case 9:
            printf("Exiting game.\n");
            update_scene(ss, SCENE_exit, 0);
            break;
        default:
            clear_console();
            printf("'%hu' is an invalid choice. Try again.\n\n", choice);
            break;
    }
}

void scene_guardian(char *username, SceneState *ss) {
    uint16 choice = 0;

    prompt("You now face the dungeon’s final guardian, a massive stone golem protecting an artifact of deeply mysterious power.\n"\
           "What do you do?\n"\
           "1. Fight the golem head-on.\n"\
           "2. Use wit to bypass the golem.\n"\
           "9. Save and exit.\n"\
           "Choose an option: ",
           "%hu",
           &choice);

    switch(choice) {
        case 1:
            if (((rand() % 20) + 1) >= 13) {
                update_scene(ss, SCENE_return, 1);
                if_log_achievement(username, "Guardian Killer");
                printf("You have defeated the guardian!\n");
            } else {
                update_scene(ss, SCENE_death, 1);
                if_log_achievement(username, "dogwater");
                printf("The guardian defeated you.\n");
            }
            break;
        case 2:
            update_scene(ss, SCENE_return, 1);
            if_log_achievement(username,"Artifact");
            if_log_achievement(username,"Witty Bypass");
            printf("You trick the golem into letting them pass, avoiding a dangerous fight and securing the artifact.\n");
            break;
        case 9:
            printf("Exiting game.\n");
            update_scene(ss, SCENE_exit, 0);
            break;
        default:
            clear_console();
            printf("'%hu' is an invalid choice. Try again.\n\n", choice);
            break;
    }
}

void scene_death(char *username, SceneState *ss) {
    printf("Your journey has ended in tragedy.\nHere is what you accomplished:\n");
    display_achievements(username);
    printf("Press any key to return to the menu\n");
    
    scanf("%c");
    update_scene(ss, SCENE_menu, 1);
}

void scene_return(char *username, SceneState *ss) {
    uint16 choice = 0;

    prompt("You return to the old tavern, your quest complete. The hooded figure watches you with a knowing smile.\n"\
           "What do you do?\n"\
           "1. Keep the artifact for yourself.\n"\
           "2. Hand over the artifact.\n"\
           "9. Save and exit.\n"\
           "Choose an option: ",
           "%hu",
           &choice);

    switch(choice) {
        case 1:
            update_scene(ss, SCENE_tavern, 0); // We don't want to save this game state, since the next one exists; restart at the tavern.
            update_scene(ss, SCENE_exit, 1);
            if_log_achievement(username, "Power at Great Cost");
            printf("You feel the artifact corrupting you. You know it is only a matter of time before you are someone else entirely.\n");
            break;
        case 2:
            update_scene(ss, SCENE_tavern, 0); // We don't want to save this game state, since the next one exists; restart at the tavern.
            update_scene(ss, SCENE_exit, 1);
            if_log_achievement(username, "New Adventure");
            printf("The hooded figure thanks you. As he leaves a scrap of paper falls from his coat with a message.\n");
            printf("[follow me. tell no one. trust no one.]\n");
            break;
        case 9:
            printf("Exiting game.\n");
            update_scene(ss, SCENE_exit, 0);
            break;
        default:
            clear_console();
            printf("'%hu' is an invalid choice. Try again.\n\n", choice);
            break;
    }
}



/* call this after every decision scene */
/*in server.c*/

void save_game_state(const char* username, uint64 scene) {
    char file_path[256];
    FILE *f;

    snprintf(file_path, sizeof(file_path), "%s%s_state.txt", SAVE_PATH,
        username);
    if ((f = fopen(file_path, "w")) == NULL) {
        printf("Error: Could not create save file.\n");
        return;
    }
    fprintf(f, "%" PRIu64 "\n", scene);
    fclose(f);
    printf("Game saved.\n");
}

uint64 load_game_scene(const char* username) {
    char file_path[256];
    uint64 scene;
    FILE* f;

    snprintf(file_path, sizeof(file_path), "%s%s_state.txt", SAVE_PATH,
        username);
    if ((f = fopen(file_path, "r")) == NULL) {
        return SCENE_exit;  // if no save file exists
    }
    fscanf(f, "%lu", &scene);
    fclose(f);
    return scene;
}

char **get_recieved_achievements(const char *username, int *count) {
    char file_path[256];
    char **recieved_achievements = NULL;
    FILE *file;
    *count = 0;

    snprintf(file_path, sizeof(file_path), "%s%s_achievements.txt", ACHIEVEMENT_PATH, username);
    if ((file = fopen(file_path, "r")) == NULL) { return NULL; }

    // Allocate memory for achievement pointers
    if (!(recieved_achievements = malloc(MAX_ACHIEVEMENT_LENGTH * sizeof(char *)))) {
        perror("Failed to allocate memory for achievements");
        fclose(file);
        return NULL;
    }

    while (*count < total_achievements && fgets(file_path, sizeof(file_path), file)) {
        // Strip the newline character if present
        file_path[strcspn(file_path, "\n")] = '\0';

        // Remove "Achievement unlocked: " prefix if it exists
        char *achievement_name = strstr(file_path, "Achievement unlocked: ");
        if (achievement_name != NULL) {
            // Move the pointer to the actual achievement name after the prefix
            achievement_name += strlen("Achievement unlocked: ");
            // Allocate memory for the achievement name and copy it
            recieved_achievements[*count] = malloc(strlen(achievement_name) + 1);
            if (recieved_achievements[*count] == NULL) {
                perror("Failed to allocate memory for an achievement");
                fclose(file);
                // Free previously allocated memory
                for (int i = 0; i < *count; i++) {
                    free(recieved_achievements[i]);
                }
                free(recieved_achievements);
                return NULL;
            }
            strcpy(recieved_achievements[*count], achievement_name);
        } else {
            continue; // format is not as expected
        }

        (*count)++;
    }

    fclose(file);
    return recieved_achievements;
}

void release_recieved_achievements(char **received_achievements, int received_counter) {
    for (int i = 0; i < received_counter; ++i)
        free(received_achievements[i]);
    free(received_achievements);
}

int has_achievement(const char *achievement, char **achievements, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(achievement, achievements[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void decode_me(const char encoded[][MAX_ACHIEVEMENT_LENGTH], char decoded[][MAX_ACHIEVEMENT_LENGTH], int total) {
    for (int i = 0; i < total; i++) {
        decode((const unsigned char *)encoded[i], decoded[i]);
    }
}

void display_achievements(const char *username) {
    int recieved_counter;
    char **recieved_achievements = get_recieved_achievements(username, &recieved_counter);
    char decoded_achievements[total_achievements][MAX_ACHIEVEMENT_LENGTH];

    decode_me(all_achievements, decoded_achievements, total_achievements);

    printf("Achievements:\n");
    for (int i = 0; i < total_achievements; i++) {
        if (has_achievement(all_achievements[i], recieved_achievements, recieved_counter)) {
            printf("\033[0;32m%s\033[0m\n", all_achievements[i]); // Green for unlocked
        } else {
            printf("\033[0;31m%s\033[0m\n", all_achievements[i]); // Red for locked
        }
    }
    release_recieved_achievements(recieved_achievements, recieved_counter);
    putchar('\n');
}

void if_log_achievement(const char *username, const char *achievement) {
    int count = 0;
    char **achievements = get_recieved_achievements(username, &count);

    if (!has_achievement(achievement, achievements, count))
        log_achievement(username, achievement);
}


int main() {
    char username[64];
    SceneState ss = {SCENE_menu, SCENE_menu};

    do {
        scenes[ss.current_scene](username, &ss);
    } while (1); // terminated in scene_exit
}
