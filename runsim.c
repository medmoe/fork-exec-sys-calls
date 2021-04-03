#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int MAX_NUM_OF_CHILDREN = 10000;
size_t MAX_SIZE = 200;


int main(int argc, char * argv[]){
	if(argc == 1){
		perror("no given arguments");
		exit(EXIT_FAILURE);
	}
	int running_processes = 0;
	char * buffer =(char *)malloc(sizeof(char) * MAX_SIZE);
	char * arguments[MAX_SIZE];
	int j = 0;
	pid_t pid;
	pid_t p;
	pid_t w;
	int status;

	if(buffer == NULL){
		perror("unable to allocate buffer");
		exit(EXIT_FAILURE);
	}

	int e = atoi(argv[1]) ; //convert char array to integer

	if (e >= 1 && e <= MAX_NUM_OF_CHILDREN){
		while(!feof(stdin)){
			fgets(buffer, MAX_SIZE, stdin);
			buffer[strlen(buffer)-1]='\0';
			//extract arguments from the given command
			arguments[j++] = buffer + 2; 
			for(size_t i = 0; i < MAX_SIZE; ++i){
				if(!(buffer[i] == '\0')){
					if(buffer[i] == ' ' || buffer[i] == '\t'){
						buffer[i] = '\0';
						arguments[j++] = buffer + i + 1;
					}
				}else{
					arguments[j++] = NULL;
					break;
				}
			}
			//wait for processes to terminate if limit is reached
			if(e == running_processes){
				p = wait(&status);
				if(p == -1){
					perror("wait");
				}else{
					running_processes = running_processes - 1 ;
					if(WIFEXITED (status)){
							printf("Process with pid %d :Normal termination with exit status=%d\n", p, WEXITSTATUS(status) );
						}else if(WIFSIGNALED (status)){
							printf("Process with pid %d :Killed by signal %d\n", p, WTERMSIG (status));
						}else if(WIFSTOPPED(status)){
							printf("Process with pid %d :stopped by signal %d\n", p, WSTOPSIG(status));
						}else if(WIFCONTINUED(status)){
							printf("Process with pid %d :continued\n", p);
						}
				}
			}else{
			//fork a new process
				pid = fork();
				if(pid == -1){
					perror("fork");
					exit(EXIT_FAILURE);
				}
				//code that runs by the parent
				if(pid > 0){
					printf("Process with pid %d successfully forked\n", pid);
					running_processes = running_processes + 1;
					w = waitpid(pid, &status, WNOHANG|WUNTRACED|WCONTINUED);
					if(w == -1){
						perror("waitpid");
					}else if (w != 0){
						if(WIFEXITED(status)){
							printf("parent : Process with pid %d: Normal termination with exit status=%d\n",w , WEXITSTATUS(status));
						}else if(WIFSIGNALED(status)){
							printf("Process with pid %d: Killed by signal%d\n", w, WTERMSIG(status));
						}else if(WIFSTOPPED(status)){
							printf("Process with pid %d: Stopped by signal=%d\n", w, WSTOPSIG(status));
						}else if(WIFCONTINUED(status)){
							printf("Process with pid %d: continued\n", w);
						}
					}
					
				}
				else if(!pid){
					//execute the child process
					int ret;
					ret = execv("/root/Documents/cisc3150/hw4/test", arguments);
					if(ret == -1){
						perror("execv");
						exit(EXIT_FAILURE);
					}
				}
			}
		}
		//wait for the running processes to terminate once EOF is reached
		while(running_processes > 0){
			pid_t child_pid = wait(&status);

			if(child_pid == -1){
			
				perror("wait second loop");
			}else{
				running_processes = running_processes - 1;
				if(WIFEXITED (status)){
							printf("Process with pid %d :Normal termination with exit status=%d\n", child_pid, WEXITSTATUS(status) );
						}else if(WIFSIGNALED (status)){
							printf("Process with pid %d :Killed by signal %d\n", child_pid, WTERMSIG (status));
						}else if(WIFSTOPPED(status)){
							printf("Process with pid %d :stopped by signal %d\n", child_pid, WSTOPSIG(status));
						}else if(WIFCONTINUED(status)){
							printf("Process with pid %d :continued\n", child_pid);
						}
			}
		}
		

	}else{
		perror("cannot generate that amount of processes");
		exit(EXIT_FAILURE);
	}
	free(buffer);

	exit(EXIT_SUCCESS);
}