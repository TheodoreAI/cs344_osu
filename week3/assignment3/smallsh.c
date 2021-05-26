// Mateo Estrada
// OS Smallsh Assignment
// April 26, 2021
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h> // for waitpid
#include <stdio.h>    // for printf and perror
#include <stdlib.h>   // for exit
#include <unistd.h>   // for execv, getpid, fork

// I need a bolean data type:

typedef int bool;
enum
{
  false,
  true
};

const int maxLengthInput = 2048; // Maximum number of characters
int boolBackground = 1;
bool bgProcessFlag = false;
bool inputRedFlag = false;
bool outputRedFlag = false;


void captureSIGTSTP(int);

void getInputs(char *arr[], int pid, char inputFileName[], char outputFileName[], int *background, struct sigaction sa)
{
  char input[maxLengthInput];
  // Get input from the user:
  printf(": ");
  fflush(stdout);
  fgets(input, maxLengthInput, stdin);

  // remove the new lines
  int find = 0;
  int val = 0;

  while (val < maxLengthInput && !find)
  {
    if (input[val] == '\n')
    {
      input[val] = '\0';
      find = 1;
    }
    val++;
  }

  // When its a blank return it:
  if (strcmp(input, "") == 0)
  {
    arr[0] = strdup("");
    return;
  }
  // This function makes a token with the input string.
  const char arrSpace[2] = " ";
  char *token = strtok(input, arrSpace);

  // For loop will check for  & and for foreground/background processes
  for (int j = 0; token; j++)
  {
    // If there is & there is going to be the start of a background process
    if (!strcmp(token, "&"))
    {
      *background = 1; // change the boolean value
    }
    else if (strcmp(token, "<") == 0)
    {
      // if there is an input then copy the token to the inputFileName
      // also change the redirect flag to true for later consumption
      token = strtok(NULL, arrSpace);
      strcpy(inputFileName, token);
      inputRedFlag = true;
    }
    else if (strcmp(token, ">") == 0)
    {
      // if there is an output then copy the token to the outputFileName
      // also change the redirect flag to true for later consumption
      token = strtok(NULL, arrSpace);
      strcpy(outputFileName, token);
      outputRedFlag = true;
    }
    else
    {
      // string duplicate from the token to remove some of the characters.
      // Attempting something like this: https://stackoverflow.com/questions/40222582/segfault-resulting-from-strdup-and-strtok
      arr[j] = strdup(token);
      int k = 0;
      // Loop through the array to find $$ next to each other
      while (arr[j][k])
      {
        if (arr[j][k] == '$' && arr[j][k + 1] == '$')
        {
          arr[j][k] = '\0';
          snprintf(arr[j], 257, "%d%s", pid, arr[j]); // copy the pid onto the array that will give the name to the directories made with $$
        }
        k++;
      }
    }
    // Moving onto the next token
    token = strtok(NULL, arrSpace); 
  }
}

// To interpret the termination status I wrote this function that is adapted from this lecture:
// https://canvas.oregonstate.edu/courses/1810930/pages/exploration-process-api-monitoring-child-processes?module_item_id=20734141
void printingStatus(int childStatus, int childPid)
{
  // Printing status and signals when they are terminated.
  if (WIFEXITED(childStatus))
  {
    printf("Child %d exited normally with status %d \n", WEXITSTATUS(childStatus));
  }
  else if (WIFEXITED(childStatus) == 0)
  {
    printf("Child %d Terminated by signal %d \n", childPid, WTERMSIG(childStatus));
  }
  else
  {
    printf("Child %d exited abnormally due to signal %d\n", childPid, WTERMSIG(childStatus));
  }
}

void printingChildStatus(int childStatus)
{
  // Printing status and signals when they are terminated.
  if (WIFEXITED(childStatus))
  {
    printf("Child exited normally with status %d \n", WEXITSTATUS(childStatus));
  }
  else if (WIFEXITED(childStatus) == 0)
  {
    printf("Terminated by signal %d \n", WTERMSIG(childStatus));
  }
  else
  {
    printf("Child exited abnormally due to signal %d\n", WTERMSIG(childStatus));
  }
}

//*arr[], &background, inputCmd[], outCmd[], sa, *childStatus, ignore_sig, 
int execFamily(char *arr[], int *background, char inputCmd[], char outputCmd[], struct sigaction sa, int *childStatus, struct sigaction ignore_sig)
{
  // Following the structure from the Monitoring child Processes Module for the example:
  int input; // booleans
  int output;
  int result;
  pid_t spawnPid = -5; // start the spawnPid
  spawnPid = fork(); // forking like in the lecture samples.
  char pathname[] = "/dev/null"; // pathname for open()
  switch (spawnPid)
  {
  case -1:
    printf("fork() failed\n");
    fflush(stdout);
    break;
  case 0:
    // This should take the process ^C
    // Handle input, This code is from the Processes and I/O lecture.
    if (bgProcessFlag == false)
    {
      sigaction(SIGINT, &sa, NULL);
      // When there is an input this will be executed.
      if(inputRedFlag == true){


        if (strcmp(inputCmd, "")) // if there is a non-empty input.
        {
          input = open(inputCmd, O_RDONLY); // open the file specified by the inputCmd to read only.
          if (input == -1) // handling errors on the input
          {
            fprintf(stderr, "Unable to open %s for input. \n", inputCmd);
            fflush(stderr);
            exit(1);
          }
          else
          {
            // if successful dup2() will duplicate the file descriptor 
            result = dup2(input, STDIN_FILENO); // assign it to result as an int
            if (result == -1)
            {
              perror("Unable to assign input file. \n");
              exit(1);
            }
            else
            {
              // Used to change the properties of the open input file:
              // F_SETFD: Set the file descriptor flags to the value specified by arg
              // FD_CLOEXEC: close on exec flag, if this is set, the file descriptor will automatically be closed during a successful execv
              fcntl(input, F_SETFD, FD_CLOEXEC);
            }
          }
        }

        if (outputRedFlag == true){
          // Handle the output redirection for background processes, this code is also from the lecture on I/O.
          if (strcmp(outputCmd, "") != 0)
          {
            // same as above where we open the file given the path, and we set it to write, create, and truncate and assign permissions since we need

            output = open(outputCmd, O_WRONLY | O_CREAT | O_TRUNC, 0666); // -rw-rw-rw-
            if (output == -1)
            {
              perror("Unable to open output file.\n");
              exit(1);
            }
            // assign it to the result so that it can be processed later
            result = dup2(output, STDOUT_FILENO);
            if (result == -1)
            {
              perror("Unable to open output file.\n");
              exit(1);
            }
            else
            {
              // Used to change the properties of the open output file:
              // F_SETFD: Set the file descriptor flags to the value specified by arg
              // FD_CLOEXEC: close on exec flag, if this is set, the file descriptor will automatically be closed during a successful execv
              fcntl(output, F_SETFD, FD_CLOEXEC);
            }
          }
        }
        }

        }
          
    else{
      // Background is allowed.
      input = open(pathname, O_RDONLY); // Open the file given the path to read only:
      if(input == -1){
        perror("Unable to open input file in background.\n");
        exit(1);
      }
      result = dup2(input, STDIN_FILENO); // duplicate a file descriptor
      if (result == -1){
        // error handling
        perror("Unable to do dup2()\n");
        exit(1);
      }

      // This loop will flag any child terminated background processes using the waitpid().
      // Using WNOHANG mwans parent does not wait if child does not terminate just check and return.
      // This is from the lecture on monitioring children: https://canvas.oregonstate.edu/courses/1810930/pages/exploration-process-api-monitoring-child-processes?module_item_id=20734141
      fcntl(input, F_SETFD, FD_CLOEXEC); 
      
      output = open(pathname, O_WRONLY | O_CREAT | O_TRUNC, 0666); // open and set the permissions
      if (output == -1){
        // error handling
        perror("can't open /dev/null for output");
        exit(1);  
      }

      result = dup2(output, STDOUT_FILENO); // duplicate file descriptor
      if(result == -1){
        // error handling
        perror("Error with dup2()");
        exit(1);
      }
      // This loop will flag any child terminated background processes using the waitpid().
      // Using WNOHANG mwans parent does not wait if child does not terminate just check and return.
      // This is from the lecture on monitioring children: https://canvas.oregonstate.edu/courses/1810930/pages/exploration-process-api-monitoring-child-processes?module_item_id=20734141

      fcntl(output, F_SETFD, FD_CLOEXEC); 
      if (inputRedFlag == true){

        
        if (strcmp(inputCmd, "") != 0){
          input = open(inputCmd, O_RDONLY);
          if (input == -1){
            perror("Can't open input\n");
            exit(1);
          }
          result = dup2(input, STDIN_FILENO);
          if(result == -1){
            perror("Error with dup2()\n");
            exit(1);
          }
          fcntl(input, F_SETFD, FD_CLOEXEC);
        }
      }
      if(outputRedFlag == true){

        if(strcmp(outputCmd, "")!= 0){
          output = open(outputCmd, O_WRONLY | O_CREAT | O_TRUNC, 0666);
          if(output == -1){
            perror("can't open for output\n");
            exit(1);
          }
          result = dup2(output, STDOUT_FILENO);
          if (result == -1){
            perror("Error with the dup2()\n");
            exit(1);
          }
          fcntl(output, F_SETFD, FD_CLOEXEC);
        }

      }
    }
    int execStatus = execvp(arr[0], arr);
    if (execStatus)
    {
      // Executer the files
      printf("%s: no such file or directory\n", arr[0]);
      fflush(stdout);
      exit(2);
    }
    break;

  default:
    // For the default, I should be able to execute a process in the background.
    if (*background && boolBackground)
    {
      pid_t PID = waitpid(spawnPid, childStatus, WNOHANG);
      printf("Background pid is %d\n", spawnPid);
      fflush(stdout);
    }else if(*background != boolBackground){
      pid_t PID = waitpid(spawnPid, childStatus, 0);
    }

    // This loop will flag any child terminated background processes using the waitpid().
    // Using WNOHANG mwans parent does not wait if child does not terminate just check and return.
    // This is from the lecture on monitioring children: https://canvas.oregonstate.edu/courses/1810930/pages/exploration-process-api-monitoring-child-processes?module_item_id=20734141

    while((spawnPid = waitpid(-1, childStatus, WNOHANG)) > 0){
      printf("child %d terminated\n", spawnPid);

        if (WIFEXITED(childStatus))
        {
          printf("Child %d exited normally with status %d \n", WEXITSTATUS(childStatus));
        }
        else if (WIFEXITED(childStatus) == 0)
        {
          printf("Child %d Terminated by signal %d \n", spawnPid, WTERMSIG(childStatus));
        }
        else
        {
          printf("Child %d exited abnormally due to signal %d\n", spawnPid, WTERMSIG(childStatus));
        }
        fflush(stdout);

    }
  }
}

void captureSIGTSTP(int signo)
{
  // This was adapted from this source: https://github.com/kryoli/master/shell.c
  if (boolBackground == 1)
  {
    // printf("Gets here %d", boolBackground);
    write(1, "Entering foreground-only mode (& is now ignored)\n", 49);
    fflush(stdout);
    boolBackground == 0;
  }
  else if (boolBackground == 0)
  {
    // My attempt at Exiting foreground-only mode:
    write(1, "Exiting foreground-only mode\n", 29);
    fflush(stdout);
    boolBackground == 1;
  }
}

void main()
{
  // The following variables I will need to run while loops and to keep count of the loops:
  int count = 0;
  int childStatus = -5;
  int background = 0;
  int flag = 1;
  int pid = getpid(); 
  char exit[] = "exit"; // The commands I need to implement with functions
  char cd[] = "cd";
  char status[] = "status";

  char inputFile1[257] = "";
  char outputFile1[257] = "";
  // saving the command inputs
  char *input[513];
  for (int i = 0; i < 513; i++)
  {
    input[i] = NULL;
  }

  // Signal handlers for ^C and ^Z
  // My attempt at handling signals was taken from an example on signal handler functions from:
  // https://en.wikipedia.org/wiki/Sigaction
  // I also followed some of the logic for sigaction from the following: https://github.com/jmreyes/simple-c-shell/blob/master/simple-c-shell.c
  
    // if background hasn't changed

    // ^C
    struct sigaction CTRLC = {0};  // set up the structure to specify the new action
    CTRLC.sa_handler = SIG_DFL;    // set up the sigaction handler
    // initialize and fill a signal set pointed to by the set aka CTRLC.sa_mask
    if (sigfillset(&CTRLC.sa_mask) == 0){
      CTRLC.sa_flags = 0;
    }else{
      printf("Error with the sigfillset()");
      fflush(stdout);
      
    }
      


    //^Z
    struct sigaction CTRLZ = {0};
    CTRLZ.sa_handler = captureSIGTSTP;
    if(sigfillset(&CTRLZ.sa_mask) == 0){
      CTRLZ.sa_flags = 0;
      sigaction(SIGTSTP, &CTRLZ, NULL);
    }else{
      printf("Error with the sigfillset()");
      fflush(stdout);
    }
    

    // signal stuff for ignoring signals
    struct sigaction ignore_sig = {0};
    ignore_sig.sa_handler = SIG_IGN;
    // calling sigaction
    sigaction(SIGINT, &ignore_sig, NULL);
    


 
  // Running the program until this flag turns false
  while (flag)
  {
    // Get the input from the terminal
    getInputs(input, pid, inputFile1, outputFile1, &background, CTRLC); // call this function

    // if the input is a comment ignore it
    if (input[0][0] == '#' || input[0][0] == '\0')
    {
      continue;
    }

    else if (strcmp(input[0], exit) == 0) // if the input is the exit command then change the flag and also break;
    {
      flag = 0;
      if (flag == 0)
      {
        break;
      }
    }
    else if (strcmp(input[0], cd) == 0)
    {
      // If there is a directory specified change to that one.
      if (input[1] > 0)
      {
        // If you can't find the directory say so:
        if (chdir(input[1]) == -1)
        {
          printf("Directory/folder not found. \n");
          fflush(stdout);
        }
      }
      else
      {
        // Getting the current working directory: I used the following code to resolve the issue with environment variables.
        // https://stackoverflow.com/questions/2910377/get-home-directory-in-linux

        const char *homedir;
        if ((homedir = getenv("HOME")) == NULL)
        {
          homedir = getpwuid(getuid())->pw_dir;
        }
        chdir(homedir);
      }
    }
        // give the status of the exit value
    else if (strcmp(input[0], status) == 0)
    {
      if (WIFEXITED(childStatus) != 0)
      {
        printf("exit value %d\n", WEXITSTATUS(childStatus));
      }
      else if (WIFSIGNALED(childStatus) != 0)
      {
        // indicate by what signal was it terminated.
        printf("terminated by signal %d\n", WTERMSIG(childStatus));
      }
      fflush(stdout);
    }

    else
    {
      // If its an external command run this function:
      /**********************************************
       * execFamily()
       * variables: 
       * input --> this will be from the command line
       * &background --> this is a boolean value set to zero
       * inputFile1 --> this will be from the file that will give inputs using the > operator 
       * outputFile1 --> this will be from the fiel that will get outputs using the < operator
       * CTRLC --> needs the signal handler
       * childStatus --> needs the child status to handle the printing
       * ignore_sig --> this one is for ignoring signals but I still can't figure out a bug
      **********************************************/

      execFamily(input, &background, inputFile1, outputFile1, CTRLC, &childStatus, ignore_sig);
    }

    count++;
    // Resetting the used variables from the beginning.
    background = 0;
    inputFile1[0] = '\0';
    outputFile1[0] = '\0';
    for (int val = 0; input[val]; val++)
    {
      input[val] = NULL;
    }
  }
}