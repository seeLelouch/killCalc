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

/*
    Simple insertion sort to sort kills in descending order
*/
void sort(killer *killers, int foundKillers)
{

    int key, j;
    char keyName[17];
    for (int i = 1; i < foundKillers; i++)
    {

        key = killers[i].amountOfKills;
        sprintf(keyName, killers[i].ign);
        j = i;

        while (j > 0 && killers[j - 1].amountOfKills < key)
        {
            killers[j].amountOfKills = killers[j - 1].amountOfKills;
            sprintf(killers[j].ign, killers[j - 1].ign);
            j = j - 1;
        }
        killers[j].amountOfKills = key;
        sprintf(killers[j].ign, keyName);
    }
}

/*
    Takes in the current line, a pointer to the start of "wurde getötet"
    and the nameBuffer Array to store the following name into
*/
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

/*
    Takes in the array of killers, checks every killer with the current name in the buffer,
    if already present returns the place in the array where that name is stored, otherwise returns 32.
*/
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

void printFoundGame(killer *killers, int foundGames, int foundKillers, bool onlyOnce, int inputNumber)
{
    if (!onlyOnce)
    {
        printf("GAME %d", foundGames);
        printf("\n\n");
    }
    else
    {
        printf("GAME: %d", inputNumber);
        printf("\n\n");
    }

    for (int i = 0; i < foundKillers; i++)
    {
        printf("%s: ", killers[i].ign);
        printf("%d\n", killers[i].amountOfKills);
    }
    printf("\n\n");
}

/*
    Finds amount of games before printing results
*/
long searchFileForGames(FILE *log)
{
    char buffer[MAX_BUFFER];
    long amountGames = 0;
    while (fgets(buffer, MAX_BUFFER - 1, log) != NULL)
    {
        if (strstr(buffer, "hat die Runde SurvivalGames gewonnen") != 0)
        {
            amountGames++;
        }
    }
    fseek(log, 0, SEEK_SET);
    return amountGames;
}

void getPositionsForGames(FILE *log, long amountGames, long *positions)
{
    char buffer[MAX_BUFFER];
    int i = 1;
    positions[0] = 0;

    while (fgets(buffer, MAX_BUFFER - 1, log) != NULL)
    {
        if (strstr(buffer, "hat die Runde SurvivalGames gewonnen") != 0)
        {
            positions[i] = ftell(log);
            i++;
        }
    }
}

/*
    Searches the file for kills and calls all other needed functions to print or write the names.
    Calls itself resursively to search for more than one game
*/
void searchFile(FILE *log, long startOfFile, int foundGames, bool onlyOnce, int inputNumber)
{
    fseek(log, startOfFile, SEEK_SET);

    char buffer[MAX_BUFFER];
    char stringToFind[50];
    sprintf(stringToFind, "[SurvivalGames]");

    int amountValidLines = 0;
    int amountKillLines = 0;
    int foundKillers = 0;

    killer *killers = calloc(32, sizeof(killer));

    // Takes a line until newline until EOF
    while (fgets(buffer, MAX_BUFFER - 1, log) != NULL)
    {
        // searches for [SurvivalGames]
        if (strstr(buffer, stringToFind) != 0)
        {
            amountValidLines++;

            if (strstr(buffer, "hat die Runde SurvivalGames gewonnen"))
            {
                foundGames++;
                startOfFile = ftell(log);

                sort(killers, foundKillers);
                printFoundGame(killers, foundGames, foundKillers, onlyOnce, inputNumber);
                free(killers);

                // Early termination if we only want to display one game.
                if (onlyOnce)
                    return;

                // Recursively calls the file search loop, break condition is EOF, start of file is set as function parameter (startOfFile)
                searchFile(log, startOfFile, foundGames, onlyOnce, inputNumber);
            }

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

    printf("reached termination\n");
    // TERMINATE RECURSION HERE === EOF
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("Drag and Drop the log into the program file\n");
        system("PAUSE");
        return 0;
    }

    char input[4];
    long startOfFile;
    bool onlyOnce = false;
    int inputNumber;

    FILE *log = fopen(argv[1], "r");

    long amountGames = searchFileForGames(log);

    long *positions = calloc(amountGames, sizeof(long));
    getPositionsForGames(log, amountGames, positions);

    printf("FOUND %ld GAMES, which one do you wish to display?\n", amountGames);
    printf("Example: 1, 2, 3, all\n");

    scanf(" %3s", input);
    input[3] = '\0';

    system("cls");

    if (strncmp(input, "all", 4) == 0)
    {
        startOfFile = 0;
    }
    else
    {
        inputNumber = atoi(input);

        if (inputNumber == 0)
        {
            printf("Wrong input!\n");
            printf("Example: 1, 2, 3, all\n");
            printf("Termianting.\n");
            free(positions);

            system("PAUSE");
            return 0;
        }

        if (inputNumber > amountGames)
        {
            printf("You input a number that is bigger than the amount of games, terminating.\n");
            free(positions);

            system("PAUSE");
            return 0;
        }

        startOfFile = positions[inputNumber - 1];
        onlyOnce = true;
    }

    int foundGames = 0;
    searchFile(log, startOfFile, foundGames, onlyOnce, inputNumber);

    free(positions);
    printf("Reached end of program\n");
    system("PAUSE");
    return 0;
}