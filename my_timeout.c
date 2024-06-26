#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/resource.h>


pid_t pid;

void alarm_handler(int s) {
	if(pid != 0) {
		printf("Killing child ...\n");
		kill(pid, SIGKILL);
	}
}

int main (int argc, char *argv[]){
	if (argc < 3) {
		perror("Too many or few Arguments, only input Seconds, program");
		return 1;
	}


	//pid_t pid;
	int seconds = atoi(argv[1]);

	//printf("%d", seconds);
	
	pid = fork();

	if(pid == -1) {
		perror("Forked Failed");
		return 1;
	} else if(pid == 0) {
	
		execvp(argv[2], &argv[2]);

		perror("Error with execvp");
		exit(1);
	} else {


		struct sigaction sa;
		sa.sa_handler = alarm_handler;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		sigaction(SIGALRM, &sa, NULL);
		
		alarm(seconds);

		int status;
		waitpid(pid, &status, 0);

		if (WIFEXITED(status)) {
			int exit_status = WEXITSTATUS(status);
			//printf("Child process exited normally with status %d\n", exit_status);
			return exit_status;
		} else {
			//printf("Child did not return normally");
			return 1;
		}

		//printf("partent");
	}


	return 0;
}
