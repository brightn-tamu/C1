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

#define MENU_STRING "Welcome!\n1. Start a new game\n2. Load an existing game\n3. View achievements\n0. Exit\n"
#define FETCH_USERNAME(dest) printf("Enter a username: "); scanf("%s", dest);

typedef unsigned long uint64;
typedef struct {
    uint64 current_scene;
    uint64 previous_scene;
} SceneState;

void update_scene(SceneState *ss, uint64 next_scene) {
    ss->previous_scene = ss->current_scene;
    ss->current_scene = next_scene;
}

const char *all_achievements[] = {
    "Adventurer",
    "Artifact",
    "Felled Goblins",
    "Goblin's Respect",
    "Guardian Killer",
    "Guardian's Respect",
    "Magical Weapon",
    "New Adventure",
    "Necromancer?",
    "Power at Great Cost",
    "Power at Greatest Cost",
    "Stealthy Get Away",
    "Trap Expert",
    "Walk This Way",
    "Witty Bypass"
};

int total_achievements = sizeof(all_achievements) / sizeof(all_achievements[0]);


// forward declarations of helper functions
void log_achievement(const char *, const char *);
void save_game_state(const char *, uint64);
void display_achievments(const char *);

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
};

/* This is always the final scene - the program is terminated from here */
void scene_exit(char *username, SceneState *ss) {
    save_game_state(username, ss->current_scene);
    exit(0);
}

uint64 load_game_scene(const char* username) {
    char file_path[256];
    uint64 scene;
    FILE* f;

    snprintf(file_path, sizeof(file_path), "%s%s_state.txt", SAVE_PATH,
        username);
    if ((f = fopen(file_path, "r")) == NULL) {
        printf("Could not find an existing game for '%s'.\n", username);
        return SCENE_exit;  // if no save file exists
    }
    fscanf(f, "%lu", &scene);
    fclose(f);
    return scene;
}

int has_achievement(const char *achievement, const char achievements[][50], int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(achievement, achievements[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void display_achievements(const char *username) {
    char recieved_achievements[100][50];
    int recieved_counter = 0;

    char file_path[256];
    snprintf(file_path, sizeof(file_path), "%s%s_achievements.txt", ACHIEVEMENT_PATH, username);
    FILE *file = fopen(file_path, "r");
    if (file != NULL) {
        while (fgets(recieved_achievements[recieved_counter], sizeof(recieved_achievements[recieved_counter]), file)) {
            // Strip the newline character if present
            recieved_achievements[recieved_counter][strcspn(recieved_achievements[recieved_counter], "\n")] = '\0';

            // Remove "Achievement unlocked: " prefix if it exists
            char *achievement_name = strstr(recieved_achievements[recieved_counter], "Achievement unlocked: ");
            if (achievement_name != NULL) {
                // Move the pointer to the actual achievement name after the prefix
                achievement_name += strlen("Achievement unlocked: ");
                // Copy the achievement name back to the array
                strncpy(recieved_achievements[recieved_counter], achievement_name, sizeof(recieved_achievements[recieved_counter]));
            }
            recieved_counter++;
        }
        fclose(file);
    } else {
        printf("No achievements found for user %s.\n", username);
        return;
    }

    // Print achievements
    printf("Achievements:\n");
    for (int i = 0; i < total_achievements; i++) {
        if (has_achievement(all_achievements[i], recieved_achievements, recieved_counter)) {
            printf("\033[0;32m%s\033[0m\n", all_achievements[i]); // Green for unlocked
        } else {
            printf("\033[0;31m%s\033[0m\n", all_achievements[i]); // Red for locked
        }
    }
}

void scene_menu(char *username, SceneState *ss) {
    uint64 choice;

    printf(MENU_STRING);
    printf("Choose an option: ");
    scanf("%lu", &choice);

    /*
    //Check clock times for reverser
    time_t start = time(NULL);
    sleep(5000);
    time_t end = time(NULL);

    if (difftime(end, start) < 0) {
        printf("Nope");
        sleep(5);
        while (1);
    }
    */

    switch (choice) {
        case 1:
            FETCH_USERNAME(username)
            update_scene(ss, SCENE_tavern); // tavern is the first scene
            break;
        case 2:
            FETCH_USERNAME(username);
            if (( (ss->current_scene = load_game_scene(username)) == SCENE_exit)) {
                printf("Please try again.\n\n");
                ss->current_scene = SCENE_menu; // reset the scene to menu
            }
            break;
        case 3:
            FETCH_USERNAME(username)
            display_achievements(username);
            /* scene is not updated so that the next scene remains the menu */
            break;
        case 0:
            printf("Exiting game.\n");
            exit(0);
        default:
            printf("Invalid choice. Try again.\n");
            /* again, scene not updated to keep the menu showing */
        break;
    }
}


/* the scene one function basically */
void scene_tavern(char *username, SceneState *ss) {
    int choice;

    printf("The you find yourself in a dimly lit tavern, filled with rowdy patrons. A hooded figure sits in the corner, gesturing for you to approach.\n");
    printf("What do you do?\n");
    printf("1. Sit down with the hooded figure.\n");
    printf("2. Ignore the figure and stay at the bar.\n");

    while(true) {
        scanf("%d", &choice);
        if (choice == 1) {
            printf("The figure offers a dangerous quest to retrieve an ancient artifact from a cursed dungeon. \n");
            update_scene(ss, SCENE_forest);
            log_achievement(username,"Adventurer");
            break;
        } else if (choice == 2) {
            printf("You ignore the man but instead hear the bartender's whispers of a rumor, a lost treasure hidden deep in the mountains. \n");
            printf("You leave to find this treasure.\n");
            update_scene(ss, SCENE_mountains);
            log_achievement(username,"Adventurer");
            break;
        } else {
            printf("Not a Valid Choice. \n");
        }
    }
}

void scene_forest(char *username, SceneState *ss){
    int choice;
    //*scene 2*
    //*Setting:* Deep in the eerie, dark woods, the player is on their way to the cursed dungeon when they hear rustling in the bushes.
    printf("Deep in the eerie, dark woods, you are alone on your way to the cursed dungeon when you hear rustling in the bushes.\n");
    printf("What do you do?\n");
    printf("1. Investigate the noise.\n");
    printf("2. Ignore the noise and keep moving.\n");
    while(true) {
        scanf("%d", &choice);
        if (choice == 1) {
            printf("You are ambushed by a group of goblins!\n");
            printf("*PRESS *1* TO TRY TO FIGHT BACK\n");
            printf("PRESS *2* TO SUBMIT QUIETLY\n");
            int choice2;
            while(true) {
                scanf("%d", &choice2);
                if (choice2==1){
                    int attack = (rand() % 20) + 1;
                    if(attack>=7){
                        printf("You have defeated the goblins and can now claim their loot as you continue on your quest!\n");
                        log_achievement(username,"Felled Goblins");
                        update_scene(ss, SCENE_dungeon);
                    }
                    else{
                        printf("The goblins defeated you and you are now their prisoner!\n");
                        update_scene(ss, SCENE_goblin_lair);
                    }
                    break;
                } else if (choice2 == 2) {
                    printf("You are captured by the goblins.\n");
                    update_scene(ss, SCENE_goblin_lair);
                    break;
                } else {
                    printf("Not a Valid Choice.\n");
                }
            }
            break;
        } else if (choice == 2) {
            printf("You ignore the noise and continue on your quest with a strange feeling you are being watched...\n");
            update_scene(ss, SCENE_dungeon);
            break;
        } else{
            printf("not a valid choice.\n");
        }
    }
}

void scene_mountains(char *username, SceneState *ss){
    int choice;
    
    printf("The you traverse treacherous mountain paths, the wind howling around you, when you come across an injured traveler lying on the ground.\n");
    printf("What do you do?\n");
    printf("1. Help the traveler.\n");
    printf("2. Leave the traveler.\n");
    
    while(true) {
        scanf("%d", &choice);
        if (choice == 1) {
            printf("The traveler reveals themselves to be a powerful wizard.\n");
            printf("He grants you his magical staff for your journey.\n");
            log_achievement(username,"Magical Weapon");
            update_scene(ss, SCENE_cave);
            break;
        } else if (choice == 2) {
            printf("You reaches the treasure cave but encounter a deadly trap... \n");
            update_scene(ss, SCENE_cave);
            break;
        } else {
            printf("Not a Valid Choice. \n");
        }
    }
}

void scene_goblin_lair(char *username, SceneState *ss){
    int choice;

    printf("You are tied up in a grimy goblin cave, surrounded by laughing goblins.\n");
    printf("What do you do?\n");
    printf("1. Attempt to break free.\n");
    printf("2. Bargain with the goblin chief.\n");
    
    while(true){
        scanf("%d", &choice);
        if (choice == 1) {
            printf("You manage to slip away when the goblins aren't looking but are forced to leave some of your valuables behind.\n");
            update_scene(ss, SCENE_dungeon);
            log_achievement(username,"Stealthy Get Away");
            break;
        } else if(choice == 2) {
            printf("The goblin chief offers you a deal: perform a task for them, and they will let the you go.\n");
            update_scene(ss, SCENE_goblin_favor);
            break;
        } else{
            printf("Not a Valid Choice.\n");
        }
    }
}

void scene_dungeon(char *username, SceneState *ss){
    int choice;

    printf("The cursed dungeon looms before you, filled with traps, monsters, and treasure.\n");
    printf("What do you do?\n");
    printf("1. Enter cautiously.\n");
    printf("2. Charge in boldly.\n");
    
    while(true){
        scanf("%d", &choice);
        if (choice == 1) {
            printf("You avoid most traps but come face to face a powerful guardian as you reach the deepest depths of the dungeon.\n");
            update_scene(ss, SCENE_guardian);
            log_achievement(username,"Trap Expert");
            break;
        } else if(choice == 2) {
            printf("You rush into the dungeon as many traps go off, causing you to wince in pain but giving you the opportunity to catch this dungeons guardian by surprise.\n");
            update_scene(ss, SCENE_guardian);
            break;
        } else{
            printf("Not a Valid Choice.\n");
        }
    }
}

void scene_goblin_favor(char *username, SceneState *ss) {
    int choice;

    printf("The goblin chief asks you to retrieve a stolen idol from a rival tribe of kobolds.\n");
    printf("What do you do?\n");
    printf("1. Accept the task.\n");
    printf("2. Refuse the task.\n");
    
    while(true){
        scanf("%d", &choice);
        if (choice == 1) {
            printf("You sneakily infiltrate the kobold camp as they sleep, retrieves the golden idol, and gain the favor of the goblin camp. In return, they offer you a rare artifact.\n");
            update_scene(ss, SCENE_return);
            log_achievement(username,"Artifact");
            log_achievement(username,"Goblin's Respect");
            break;
        } else if(choice == 2) {
            printf("The goblins angrily try to kill you, forcing you to dash away without your supplies and tumble downhill and into the entrance of a imposing dungeon.\n");
            update_scene(ss, SCENE_dungeon);
            break;
        } else{
            printf("Not a Valid Choice.\n");
        }
    }
}

void scene_cave(char *username, SceneState *ss) {
    int choice;

    printf("You finally reach the cave filled with treasure but senses an ancient magical presence guarding it.\n");
    printf("What do you do?\n");
    printf("1. Take the treasure.\n");
    printf("2. Leave the treasure.\n");
    
    while(true){
        scanf("%d", &choice);
        if (choice == 1) {
            printf("The guardian of the cave awakens.\n");
            update_scene(ss, SCENE_guardian);
            break;
        } else if(choice == 2) {
            printf("As you leave, the entity in the cave, its guardian, honors you for the reverence you have shown it and rewards you with a powerful magical artifact that enthralls you immendately.\n");
            log_achievement(username,"Artifact");
            log_achievement(username,"Guardian's Respect");
            update_scene(ss, SCENE_return);
            break;
        } else{
            printf("Not a Valid Choice.\n");
        }
    }
}

void scene_guardian(char *username, SceneState *ss) {
    int choice;

    printf("You now face the dungeon’s final guardian, a massive stone golem protecting an artifact of deeply mysterious power.\n");
    printf("What do you do?\n");
    printf("1. Fight the golem head-on.\n");
    printf("2. Use wit to bypass the golem.\n");
    
    while(true){
        scanf("%d", &choice);
        if (choice == 1) {
            int attack = (rand() % 20) + 1;
            if(attack>=13){
                printf("You have defeated the guardian!\n");
                update_scene(ss, SCENE_return);
                log_achievement(username,"Guardian Killer");
            }else{
                printf("The guardian defeated you.\n");
                update_scene(ss, SCENE_return);
            }
            break;
        } else if(choice == 2) {
            printf("You trick the golem into letting them pass, avoiding a dangerous fight and securing the artifact.\n");
            update_scene(ss, SCENE_return);
            log_achievement(username,"Artifact");
            log_achievement(username,"Witty Bypass");
            break;
        } else{
            printf("Not a Valid Choice.\n");
        }
    }
}

void scene_death(char *username, SceneState *ss) {
    
}

void scene_return(char *username, SceneState *ss) {
    int choice;

    printf("You return to the old tavern, your quest complete. The hooded figure watches you with a knowing smile.\n");
    printf("What do you do?\n");
    printf("1. Keep the artifact for yourself.\n");
    printf("2. Hand over the artifact.\n");
    
    while(true){
        scanf("%d", &choice);
        if (choice == 1) {
            printf("You feel the artifact corrupting you. You know it is only a matter of time before you are someone else entirely.\n");
            update_scene(ss, SCENE_exit);
            log_achievement(username,"Power at Great Cost");
            break;
        } else if(choice == 2) {
            printf("The hooded figure thanks you. As he leaves a scrap of paper falls from his coat with a message.\n");
            printf("[follow me. tell no one. trust no one.]\n");
            log_achievement(username,"New Adventure");
            update_scene(ss, SCENE_exit);
            break;
        } else{
            printf("Not a Valid Choice.\n");
        }
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

int main() {
    char username[64];
    SceneState ss = {SCENE_menu, SCENE_menu};

    do {
        scenes[ss.current_scene](username, &ss);
    } while (1); // terminated in scene_exit
}
