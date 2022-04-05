#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAX_BUFFER 500
#define maxMcNameLength 16

typedef struct
{
    int amountOfKills;
    char ign[maxMcNameLength + 1];
} killer;

void findName(char *line, char *pKill, char *nameBuffer)
{
    int i = 10;

    while (*(pKill + i) != ' ')
    {
        nameBuffer[i - 10] = *(pKill + i);
        i++;
    }
    nameBuffer[i - 10 + 1] = '\0';
}

int checkNamePresent(killer *killers, char *nameBuffer)
{
    for (int i = 0; i < 32; i++)
    {
        if (strncmp(killers[i].ign, nameBuffer, 17) == 0)
        {
            return i;
        }
    }
    return 32;
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("Not enough arguments\n");
        system("PAUSE");
        return 0;
    }

    char buffer[MAX_BUFFER];
    char stringToFind[50];
    sprintf(stringToFind, "[CHAT] [SurvivalGames]");

    FILE *log = fopen(argv[1], "r");

    // printf("buffer: %s\n", buffer);
    int amountValidLines = 0;
    int amountKillLines = 0;
    int foundKillers = 0;

    killer *killers = calloc(32, sizeof(killer));

    // Takes a line until newline until EOF
    while (fgets(buffer, MAX_BUFFER - 1, log) != NULL)
    {
        // searches for [CHAT] [SurvivalGames]
        if (strstr(buffer, stringToFind) != 0)
        {
            amountValidLines++;

            char *pKill = strstr(buffer, "wurde von");
            if (pKill != 0)
            {
                amountKillLines++;
                // ANFANGSBUCHSTABE VON JEDEM KILLER: printf("pkill: %c\n", *(pKill + 10));
                char *nameBuffer = calloc(17, sizeof(char));
                findName(buffer, pKill, nameBuffer);
                int killerPosition = checkNamePresent(killers, nameBuffer);

                if (killerPosition < 32)
                {
                    killers[killerPosition].amountOfKills++;
                    free(nameBuffer);
                }
                else
                {
                    sprintf(killers[foundKillers].ign, nameBuffer);
                    killers[foundKillers].amountOfKills++;
                    foundKillers++;
                    free(nameBuffer);
                }
            }
        }
    }
    printf("amountValidLines: %d\n", amountValidLines);
    printf("amountKillLines: %d\n", amountKillLines);
    // printf("killers[1]: %s\n", killers[1].ign);

    for (int j = 0; j < 32; j++)
    {
        printf("%s\n", killers[j].ign);
        printf("%d\n", killers[j].amountOfKills);
    }

    free(killers);
    printf("Reached end of program\n");
    system("PAUSE");
    return 0;
}