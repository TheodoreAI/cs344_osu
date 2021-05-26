#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#define PREFIX "movies_"

int count = 0;
int flag = 1;

struct movie
{

    char *title; // pointer address can be accessed with &title
    int year;
    char *languages[1000];
    float *rating;
    struct movie *next;
};
typedef struct
{
    char *title;
    int year;
} order;

// Array with the year and title to use for the new txt files.
order listMovie1[100000];
order listMovie2[100000];
order listMovie3[100000];

// order *listMovie = malloc(sizeof(order));
// order->title= malloc(sizeof(50));
// order->year = malloc(sizeof(50));

struct movie *createMovie(char *currLine)
{
    struct movie *currMovie = malloc(sizeof(struct movie));
    char *savePtr;
    // First token is the title
    char *token = strtok_r(currLine, ",", &savePtr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);
    // Second token is the year
    token = strtok_r(NULL, ",", &savePtr);

    currMovie->year = calloc(strlen(token) + 1, sizeof(char));
    currMovie->year = atoi(token);
    currMovie->next = NULL;
    return currMovie;
}

struct movie *processFile(char *filePath)
{
    /* 
        Processing the csv files and getting the important data with the createMovie().
    */

    FILE *movieFile = fopen(filePath, "r");
    char *currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    char *token;
    struct movie *head = NULL;
    struct movie *tail = NULL;
    while ((nread = getline(&currLine, &len, movieFile)) != -1)
    {
        struct movie *newNode = createMovie(currLine);
        count++;
        if (head == NULL)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }
    free(currLine);
    fclose(movieFile);
    return head;
}

bool ext_match(char *name, const char *ext)
{
    // Getting the correct file extension.  https://www.linuxquestions.org/questions/programming-9/how-to-know-the-file-extension-with-c-602471/
    size_t nl = strlen(name), el = strlen(ext);
    return nl >= el && !strcmp(name + nl - el, ext);
}

int make_new_files1(struct movie *list, char *nameDir)
{
    // Use the typeof struct order to place title and year in an array-like structure

    for (int i = 0; i < count; i++)
    {
        listMovie1[i].title = list->title;
        listMovie1[i].year = list->year;
        list = list->next;
    }

    // Store the address of the directory name to reuse it later
    char *directoryPath = malloc(1054 * sizeof(char));
    char *filenameYear = malloc(1054 * sizeof(char));
    snprintf(directoryPath, 10054, "%s", nameDir);

    for (int i = 0; i < count; i++)
    {
        FILE *dp[count]; // there are 24 years plus /0
        snprintf(directoryPath, 10054, "%s", nameDir);
        // printf("%s %d \n", directoryPath, count);
        // Passing the titles into each file
        char *src[10000] = {listMovie1[i].title};
        char *destPtr[10000];

        int yyyy = listMovie1[i].year;
        char year[1000];
        sprintf(year, "/%d.txt", yyyy);
        // Making the year of the movies as the file name.

        snprintf(filenameYear, 10054, "%s", year);
        // strcat(nameOfFile, filenameYear);
        // strcat(nameOfFile, ".txt");
        // sprintf(directoryPath,"/%d.txt", listMovie[i].year);
        if (yyyy == 0)
        {
            // printf("There is something wrong\n");
            continue;
        }
        else
        {
            // Make the right path for the files to be made inside the directory
            strncat(directoryPath, filenameYear, 9);
            dp[i] = fopen(directoryPath, "a");
            chmod(directoryPath, 0640); // Set the correct permissions for the files. rw-r-----
            // printf(" does this work? %s %d \n", directoryPath, i);
            if (dp[i] == NULL)
            {

                printf("There is something wrong with dp[i] %s\n", directoryPath);
                break;
            }
            strcpy(destPtr, src);
            fputs(*destPtr, dp[i]);
            fclose(dp[i]);
        }
    }
}

int make_new_files2(struct movie *list, char *nameDir)
{
    // Use the typeof struct order to place title and year in an array-like structure

    for (int i = 0; i < count; i++)
    {
        listMovie2[i].title = list->title;
        listMovie2[i].year = list->year;
        list = list->next;
    }

    // Store the address of the directory name to reuse it later
    char *directoryPath = malloc(1054 * sizeof(char));
    char *filenameYear = malloc(1054 * sizeof(char));
    snprintf(directoryPath, 10054, "%s", nameDir);

    // printf("does it change? %s %s \n", directoryPath);

    for (int i = 0; i < count; i++)
    {
        FILE *dp[count]; // there are 24 years plus /0
        snprintf(directoryPath, 10054, "%s", nameDir);
        // printf("%s %d \n", directoryPath, count);
        // Passing the titles into each file
        char *src[10000] = {listMovie2[i].title};
        char *destPtr[10000];

        int yyyy = listMovie2[i].year;
        char year[1000];
        sprintf(year, "/%d.txt", yyyy);
        // Making the year of the movies as the file name.

        snprintf(filenameYear, 10054, "%s", year);
        // strcat(nameOfFile, filenameYear);
        // strcat(nameOfFile, ".txt");
        // sprintf(directoryPath,"/%d.txt", listMovie[i].year);
        if (yyyy == 0)
        {
            // printf("There is something wrong\n");
            continue;
        }
        else
        {
            strncat(directoryPath, filenameYear, 9);
            // dp[i] = open(directoryPath, O_RDWR| O_CREAT);
            // printf(" does this work? %s %d \n", directoryPath, i);
            dp[i] = fopen(directoryPath, "a");
            chmod(directoryPath, 0640); // Set the correct permissions for the files. rw-r-----
            // printf(" does this work? %s %d \n", directoryPath, i);
            if (dp[i] == NULL)
            {

                printf("There is something wrong with dp[i] %s\n", directoryPath);
                break;
            }
            strcpy(destPtr, src);
            fputs(*destPtr, dp[i]);
            fclose(dp[i]);
        }
    }
}

int make_new_files3(struct movie *list, char *nameDir)
{
    // Use the typeof struct order to place title and year in an array-like structure
    for (int i = 0; i < count; i++)
    {
        listMovie3[i].title = list->title;
        listMovie3[i].year = list->year;
        list = list->next;
    }
    // Store the address of the directory name to reuse it later
    char *directoryPath = malloc(1054 * sizeof(char));
    char *filenameYear = malloc(1054 * sizeof(char));
    snprintf(directoryPath, 10054, "%s", nameDir);

    // printf("does it change? %s %s \n", directoryPath);

    for (int i = 0; i < count; i++)
    {
        FILE *dp[count]; // there are 24 years plus /0
        snprintf(directoryPath, 10054, "%s", nameDir);
        // printf("%s %d \n", directoryPath, count);
        // Passing the titles into each file
        char *src[10000] = {listMovie3[i].title};
        char *destPtr[10000];

        int yyyy = listMovie3[i].year;
        char year[1000];
        sprintf(year, "/%d.txt", yyyy);
        // Making the year of the movies as the file name.

        snprintf(filenameYear, 10054, "%s", year);
        // strcat(nameOfFile, filenameYear);
        // strcat(nameOfFile, ".txt");
        // sprintf(directoryPath,"/%d.txt", listMovie[i].year);
        if (yyyy == 0)
        {
            // printf("There is something wrong\n");
            continue;
        }
        else
        {
            strncat(directoryPath, filenameYear, 9);
            // dp[i] = open(directoryPath, O_RDWR| O_CREAT);
            // printf(" does this work? %s %d \n", directoryPath, i);
            dp[i] = fopen(directoryPath, "a");
            chmod(directoryPath, 0640); // Set the correct permissions for the files. rw-r-----
            // printf(" does this work? %s %d \n", directoryPath, i);
            if (dp[i] == NULL)
            {

                printf("There is something wrong with dp[i] %s\n", directoryPath);
                break;
            }
            strcpy(destPtr, src);
            fputs(*destPtr, dp[i]);
            fclose(dp[i]);
        }
    }
}

void make_new_dir1(entry)
{
    // Making random int
    int random = rand() % 99999;
    char nameDir[1000000];
    // Making the directory name.
    // Giving the random value to the directory.
    // printf("(The directory and random number:) %s %d", nameDir, random);

    char randomStr[1000];
    snprintf(nameDir, sizeof(nameDir), "/estradma.movies.%d", random);

    //Make the directory with the rwx-r-x--- permissions:
    printf("Now processing the chosen file named %s \n", entry);
    struct movie *list = processFile(entry); //ProcessFile() and createMovie() structures
    char cwd[100];                           // Get the correct path for the directory.
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        strcat(cwd, nameDir);
        int check_created_dir = mkdir(cwd, 0750);
        printf("Created directory with name %s \n", cwd);
    }
    make_new_files1(list, cwd); // Pass the list and the name of new directory
}

void make_new_dir2(entry)
{
    // Making random int
    int random = rand() % 99999;
    char nameDir[1000000];

    // Giving the random value to the directory.
    // printf("(The directory and random number:) %s %d", nameDir, random);

    char randomStr[1000];
    snprintf(nameDir, sizeof(nameDir), "/estradma.movies.%d", random);

    //Make the directory with the rwx-r-x--- permissions:
    // printf("This is the name of the directory: %s \n", nameDir);

    /* 
        Open the directory made and make a file with the processed data from the 
        file with name entry inside new directory.
    */
    printf("Now processing the chosen file named %s \n", entry);
    struct movie *list = processFile(entry); //ProcessFile() and createMovie() structures

    char cwd[100]; // Get the correct path for the directory.
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        strcat(cwd, nameDir);
        int check_created_dir = mkdir(cwd, 0750);
        printf("Created directory with name %s \n", nameDir);
    }
    make_new_files2(list, cwd); // Pass the list and the name of new directory
}

void make_new_dir3(entry)
{
    // Making random int
    int random = rand() % 99999;
    char nameDir[1000000];
    // Making the directory name.

    // Giving the random value to the directory.
    // printf("(The directory and random number:) %s %d", nameDir, random);

    char randomStr[1000];
    snprintf(nameDir, sizeof(nameDir), "/estradma.movies.%d", random);

    /* 
        Open the directory made and make a file with the processed data from the 
        file with name entry inside new directory.
    */
    printf("Now processing the chosen file named %s \n", entry);
    struct movie *list = processFile(entry); //ProcessFile() and createMovie() structures
    char cwd[100];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        strcat(cwd, nameDir);
        int check_created_dir = mkdir(cwd, 0750); // Make the directory with the rwx-r-x--- permissions:
        printf("Created directory with name %s \n", nameDir);
    }

    make_new_files3(list, cwd); // Pass the list and the name of new directory
}

void get_largest_file(char *dir)
{
    /*

        To get the largest file from current directory 
        I adapted this code such that I could also set the PREFIX: http://developerweb.net/viewtopic.php?pid=17083

    */
    // open the current directory
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    struct stat statbuffer;
    int i = 0;
    char *name;

    // gives an error in case can't open a directory: https://pubs.opengroup.org/onlinepubs/007904875/functions/readdir_r.html
    if ((dp = opendir(dir)) == NULL)
    {
        perror("couldn't open '.' ");
    }
    // Go through all the entries
    chdir(dir);
    while ((entry = readdir(dp)) != NULL)
    {
        lstat(entry->d_name, &statbuf);
        // Set the PREFIX as a condition
        if (strncmp(PREFIX, entry->d_name, strlen(PREFIX)) == 0)
        {
            if (S_ISREG(statbuf.st_mode))
            {
                lstat(entry->d_name, &statbuffer);
                char *fileExtension[1000];
                strcpy(fileExtension, entry->d_name);
                if (statbuffer.st_size > i)
                {

                    if (ext_match(fileExtension, ".csv"))
                    {
                        printf("The extension %s \n", fileExtension);
                        i = (statbuffer.st_size);
                        name = (entry->d_name);
                    }
                }
                else
                {
                    printf("Wrong extension");
                    continue;
                }
            }
        }
    }

    //Name will not display here
    // printf("The largest file in the current directory is %s \n", name);
    // printf("It is %i bytes \n", i);
    make_new_dir1(name);
    closedir(dp);
}

void get_smallest_file(char *dir)
{
    /*

        To get the smallest file from current directory 
        I adapted this code such that I could also set the PREFIX: http://developerweb.net/viewtopic.php?pid=17083

    */
    // open the current directory
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    struct stat statbuffer;
    int i = 9999999;
    char *name;

    // gives an error in case can't open a directory: https://pubs.opengroup.org/onlinepubs/007904875/functions/readdir_r.html
    if ((dp = opendir(dir)) == NULL)
    {
        perror("couldn't open '.' ");
        return;
    }
    // Go through all the entries
    chdir(dir);
    while ((entry = readdir(dp)) != NULL)
    {
        lstat(entry->d_name, &statbuf);
        // Set the PREFIX as a condition
        if (strncmp(PREFIX, entry->d_name, strlen(PREFIX)) == 0)
        {
            if (S_ISREG(statbuf.st_mode))
            {
                lstat(entry->d_name, &statbuffer);
                char *fileExtension[1000];
                strcpy(fileExtension, entry->d_name);
                // printf("The smallest file in the current directory is %d \n", statbuffer.st_size);
                if (statbuffer.st_size < i)
                {

                    if (ext_match(fileExtension, ".csv"))
                    {
                        printf("The extension %s \n", fileExtension);
                        i = (statbuffer.st_size);
                        name = (entry->d_name);
                    }
                    else
                    {
                        printf("Wrong extension");
                        continue;
                    }
                    // printf("The smallest file in the current directory is %d %s \n", i, name);
                }
            }
        }
    }
    make_new_dir2(name);
    closedir(dp);
}

int get_file_name(char *dir)
{
    /*

        To get the smallest file from current directory 
        I adapted this code such that I could also set the PREFIX: http://developerweb.net/viewtopic.php?pid=17083

    */
    // open the current directory
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    struct stat statbuffer;
    int sizeBytes;
    char *name;
    char fileNameInput[100];

    // Get a filename from the user:
    printf("Enter the complete file name: ");
    scanf("%s", fileNameInput);
    //gives an error in case can't open a directory: https://pubs.opengroup.org/onlinepubs/007904875/functions/readdir_r.html
    if ((dp = opendir(dir)) == NULL)
    {
        perror("couldn't open '.' ");
    }
    // Go through all the entries
    chdir(dir);
    while ((entry = readdir(dp)) != NULL)
    {
        lstat(entry->d_name, &statbuf);
        // Set the PREFIX as a condition
        if (strncmp(fileNameInput, entry->d_name, strlen(fileNameInput)) == 0)
        {
            if (S_ISREG(statbuf.st_mode))
            {
                lstat(entry->d_name, &statbuffer);

                sizeBytes = (statbuffer.st_size); //capture the file size in bytes
                name = (entry->d_name);           // capture the filename
                // printf("Found a match of size %d and name %s: \n", sizeBytes, name);
            }
        }
    }
    if (strcmp(name, fileNameInput))
    {
        printf("No file found with the name: %s\n", fileNameInput);
        return 0;
    }
    make_new_dir3(name);
    closedir(dp);
}
int menu()
{
    int choice;
    int secondChoice;

    while (1)
    {
        // Give the menu options to choose from
        printf("1. Select file to process \n");
        printf("2. Exit the program \n");
        printf("Enter a choice 1 or 2:");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("\nWhich file you want to process? \n");
            printf("Enter 1 to pick the largest file \n");
            printf("Enter 2 to pick the smallest file \n");
            printf("Enter 3 to specify the name of a file \n");
            printf("Enter a choice from 1 to 3: ");
            scanf("%d", &secondChoice);
            switch (secondChoice)
            {
            case 1:
                /*
                                Run a function that picks the largest file with the extension csv in the current 
                                directory whose name starts with the prefix movies_ automatically process it
                            */
                printf("You picked the largest file!\n");
                get_largest_file(".");
                break;

            case 2:
                /*
                                Program finds the smallest file with the extension csv in the current directory
                                whose name starts with the prefix movies_
                            */
                printf("You picked the smallest file!\n");
                get_smallest_file("."); // changed the function
                break;
            case 3:
                /*
                                The program asks the user to enter the name of a file
                                Program checks if file exists in current directory. If file doesn't exist it gives error msg and gives 3 options again.
                            */
                printf("You picked to enter a file name! \n");
                get_file_name(".");
                break;

            default:
                printf("That option doesn't exist. Try again \n");
                continue;
            }
            break;
        case 2:
            printf("Exited \n");
            exit(0);
        default:
            printf("That option doesn't exit. Try again \n");
            continue;
        }
    }
    return 0;
}
int main()
{
    srand(time(NULL));
    menu();
    return 0;
}