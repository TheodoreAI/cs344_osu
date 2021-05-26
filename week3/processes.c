#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>



// The following is an example of the wait() function to alter processes.


// int main(){
// 	pid_t spawnpid = -5;
// 	int childStatus;
// 	int childPid;
// 	// If fork is successful, the value of spawnpid will be 0 in the child, the child's pid in the parent
// 	spawnpid = fork();
// 	switch (spawnpid){
// 	case -1:
// 		perror("fork() failed!");
// 		exit(1);
// 		break;
// 	case 0:
// 		// spawnpid is 0 in the child
// 		printf("I am the child. My pid  = %d\n", getpid());
// 		break;
// 	default:
// 		// spawnpid is the pid of the child
// 		printf("I am the parent. My pid  = %d\n", getpid());
// 		childPid = wait(&childStatus);
// 		printf("Parent's waiting is done as the child with pid %d exited\n", childPid);
// 		break;
// 	}
// 	printf("The process with pid %d is returning from main\n", getpid());
// 	return 0;
// }


/*
     The following is an example of waitpid()
*/


// int main(){
// 	int   childStatus;
// 	printf("Parent process's pid = %d\n", getpid());
// 	pid_t firstChild = fork();
// 	if(firstChild == -1){
// 		perror("fork() failed!");
// 		exit(1);
// 	} else if(firstChild == 0){
// 		// The first child process execute this
// 		printf("First child's pid = %d\n", getpid());
// 		sleep(10);
// 	} else{
// 		// Parent process executes this
// 		// Fork another child
// 		pid_t secondChild = fork();
// 		if(secondChild == -1){
// 			perror("fork() failed!");
// 			exit(1);
// 		} else if(secondChild == 0){
// 			// The second child process execute this
// 			printf("Second child's pid = %d\n", getpid());
// 			sleep(10);
// 		} else{
// 			// Parent process executes this to
// 			// wait for the second child
// 			pid_t childPid = waitpid(firstChild, &childStatus, 0);
// 			printf("The parent is done waiting. The pid of child that terminated is %d\n", childPid);
// 		}
// 	}
// 	printf("The process with pid %d is returning from main\n", getpid());
// 	return 0;
    
// }


 /*
     The following examples helps troubleshoot to show where the child process might have terminated abnormally.
          
*/

int main(){
	printf("Parent process's pid = %d\n", getpid());

	int   childStatus;
	pid_t childPid = fork();

	if(childPid == -1){
		perror("fork() failed!");
		exit(1);
	} else if(childPid == 0){
		// Child process
		sleep(10);
	} else{
		printf("Child's pid = %d\n", childPid);
		childPid = waitpid(childPid, &childStatus, 0);
		printf("waitpid returned value %d\n", childPid);
		if(WIFEXITED(childStatus)){
			printf("Child %d exited normally with status %d\n", childPid, WEXITSTATUS(childStatus));
		} else{
			printf("Child %d exited abnormally due to signal %d\n", childPid, WTERMSIG(childStatus));
		}
	}
	return 0;
}


/*
     Zombie processes: If the parent of a process does not wait for a child to terminate, then the child becomes a “zombie” process on termination. Most of the resources allocated to a zombie process are recycled, e.g., its memory, open file descriptors, etc. However, an entry for a zombie process is retained in the process table. If the parent process of a zombie later uses waitpid to check the termination status of this child, then the zombie process is removed from the process table.

If a parent process terminates without cleaning its zombie child processes, 
then the zombies become children of the init process. 
The init process periodically waits for all its terminated children, 
including zombies and the zombies will get cleaned up by the init process.


*/


// Process API: Executing a new Progrm





