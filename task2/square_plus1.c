#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/resource.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<ctype.h>
#include<fcntl.h>


void limit_fork(rlim_t max_procs) {

	struct rlimit rl;

	if (getrlimit(RLIMIT_NPROC, &rl)) {
		perror("getrlimit");
		exit(-1);
	}
	else{
		//printf("Current max processes limit: %lu\n", (unsigned long)rl.rlim_cur);
		//printf("Max processes limit (hard limit): %lu\n", (unsigned long)rl.rlim_max);	
	}
	
	rl.rlim_cur = max_procs;


	if (setrlimit(RLIMIT_NPROC, &rl)) {
		perror("setrlimit");
		exit(-1);
	}

}


int main(int argc, char *argv[]) {


	//limit_fork(127);

	int userInput, square, result;
	int pipe1[2], pipe2[2], pipe3[2];

	if(pipe(pipe1) == -1 || pipe(pipe2) == -1 || pipe(pipe3) == -1 ) {
		perror("Pipe Failed");
		exit(-1);
	}

	//Read From User
	/*
	printf("Insert Integer(EOF to Stop): ");
	if (scanf("%d", &userInput) != 1) {
		if(feof(stdin)){
			printf("Exiting\n");
			break;
		}

	}
*/

	//Create first Child (square value)


	pid_t child1_pid, child2_pid;  
	child1_pid = fork();



	if(child1_pid == -1) {
		perror("Error forking Child1");
		exit(-1);
	}

	//child 1 process starts here
	if(child1_pid == 0) {

		close(pipe1[1]); //Close write end of Pipe 1 (receiving from parent	
		close(pipe2[0]); //close read end from pipe2 (sending to child2

		close(pipe3[1]);	
		close(pipe3[0]);

		
		while (read(pipe1[0], &userInput, sizeof(userInput)) >0) {
			//perror("read failed \n");
			//exit(-1);
			square = userInput * userInput;
			write(pipe2[1], &square, sizeof(square));
		}
		/*
		square = userInput * userInput;
		
		if (write(pipe2[1], &square, sizeof(square)) == -1) {
			perror("write failed \n");
			exit(-1);
		}
		*/
		
		close(pipe1[0]);
		close(pipe2[1]);

		exit(0);
	//Child 2 starts here
	} else {	
	child2_pid = fork();
	
	if(child2_pid == -1) {
		perror("child2 fork failed");
		exit(-1);
	}

	if(child2_pid == 0) {
		close(pipe1[0]);
		//close(pipe1[1]);
		close(pipe1[1]);
		close(pipe2[1]); // reading from pipe2 (from child1)
		close(pipe3[0]);


		while (read(pipe2[0], &square, sizeof(result)) >0 ) {
			result = square + 1;
			write(pipe3[1], &result, sizeof(result));

			//perror("Pipe2 error reading");
			//exit(-1);
		}	
/*
		result = square + 1;
		
		if(write(pipe3[1], &result, sizeof(result)) == -1 ) {
			perror("Pipe1 writng error");
			exit(-1);
		}

*/
		close(pipe3[1]);
		close(pipe2[0]);
		exit(0); 	
	} else {

	//parent
		close(pipe1[0]);
		close(pipe2[1]);
		close(pipe3[1]);
			
		int scanResult;
		int Intread;
	while((Intread = scanf("%d", &userInput)) != EOF) {
			
			
		

			if(Intread == 1) {
				
			write(pipe1[1], &userInput, sizeof(userInput));
			read(pipe3[0], &result, sizeof(result));
			printf("Return Value: %d\n", result);

				
			} else {

			printf("Enter valid input only");
			while(getchar() != '\n');
			//int c;
			//while ((c= getchar()) != '\n' && c != EOF);
}

}
				


		close(pipe1[1]);
		close(pipe3[0]);
		wait(NULL);
		wait(NULL);
		}	
	}
	//printf("%d\n", userInput);
		






	return 0;


}
