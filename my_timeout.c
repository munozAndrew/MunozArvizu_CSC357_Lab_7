#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <signal.h>
#include<sys/types.h>
#include<sys/wait.h>


void alarm_handler(int s) {

}

int main (int argc, char *argv[]){
	if (argc != 3) {
		perror("Too many or little Arguments, only input Seconds, program");
		return 1;
	}


	pid_t pid;
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

			return WEXITSTATUS(status);
		} else {
			return 1;
		}

		//printf("partent");
	}


	return 0;
}
