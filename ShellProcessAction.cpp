#include <stdio.h> // fopen(), fclose()
#include <string>
#include <cstring> // strcpy(), strcmp()
#include <vector>
#include <errno.h> // perror()
#include <unistd.h> // execvp(), fork(), sleep()
#include <sys/wait.h>
#include <signal.h> // signal()
#include "ShellProcessAction.h"
#include "ShellPCB.h"
#include "ShellUtility.cpp"

using std::string;
using std::vector;

#define EXIT_SHELL true;

void chSigHandler(int sig){
	int wstatus = 1;

	pid_t chpid = waitpid(-1, &wstatus, WNOHANG|WUNTRACED|WCONTINUED);
	if(chpid > 0){
		if( WIFEXITED(wstatus) || WIFSIGNALED(wstatus) ){
			ShellPCB::Instance()->removePID(chpid);
		}
	}
	else{
	}
	
	return;
}


void ShellProcessAction::IORedirect(char command[], bool isBgProcess){
	FILE *rPipe, *wFile;

	if(isBgProcess){// remove '&'
		command[strlen(command)-1] = '\0';
	}

	ShellUtility mShellUtility;

	vector<string> vCommand = mShellUtility.splitString(string(command), ' ');
	// extract output path to outPath
	string outTmp = "";
	char* outPath;
	for(size_t i = 0; i < vCommand.size(); i++){
		if(vCommand[i][0] == '>'){
			outTmp = vCommand[i];
			vCommand.erase(vCommand.begin() + i);
			break;
		}
	}

	if(outTmp.size() > 0){
		outTmp = outTmp.erase(0,1);
		outPath = &(outTmp[0]);
	}
	else{
		outPath = NULL;
	}

	// convert vCommand to char*
	string cmdTmp;
	for(const auto &tok : vCommand){
		cmdTmp += " " + tok;
	}
	char* inCmd = &cmdTmp[0];

	// open a read pipe and pass the output to specified path (stdout if not specified)
	char buffer[LINE_LENGTH];
	if( ( rPipe = popen(inCmd, "r")) == NULL){
		printf("popen() failed.");	
		_exit(1);
	}

	if(outPath != NULL){
		if( (wFile = fopen(outPath, "w")) == NULL){
		perror("Could not open output file");
		_exit(1);
		}
	}
	else{
		wFile = stdout;
	}

	while( fgets(buffer, LINE_LENGTH, rPipe) != NULL){
			fputs(buffer, wFile);
	}

	pclose(wFile);
	pclose(rPipe);
	_exit(1);
}



bool ShellProcessAction::ProcessAction(){
	signal(SIGCHLD, chSigHandler);

	id_t pid;
	bool isBgProcess, isRedirect;

	ShellUtility mShellUtility;
	ShellPCB* mShellPCB = ShellPCB::Instance(); 

	char prompt[] = "SHELL379: ";
	if( write(STDOUT_FILENO, prompt, sizeof(prompt)) == -1){
		perror("Error writing prompt");
	}
	fflush(stdout);

	char buf[LINE_LENGTH];
	memset(buf, '\0', sizeof(buf));
	if( read(STDIN_FILENO, buf, sizeof(buf)) == -1){
		perror("Error reading command");
	}

	vector<string> tokens = mShellUtility.splitString(string(buf), ' ');
	if(tokens.size() > 0){
		isBgProcess = mShellUtility.isBgProcess(tokens);
		isRedirect = mShellUtility.isRedirect(tokens);
		vector<char*> args = mShellUtility.vStrToChar(tokens);

		char* cmd = args[0];

		if(strcmp(cmd, "exit") == 0){
			if(mShellPCB->getProcessCount() > 0){
				char prompt[] = "Terminate all child processes with 'kill <pid>'"
								" before exiting.\nUse 'jobs' to find pid's.\n";
				if( write(STDOUT_FILENO, prompt, sizeof(prompt)) == -1){
					perror("Error writing prompt");
				}
				return !EXIT_SHELL
			}
			return EXIT_SHELL;
		}
		else if(strcmp(cmd, "jobs") == 0){
			mShellPCB->showJobs();
		}
		else if(strcmp(cmd, "sleep") == 0){
			int i = atoi(args[1]);
			sleep(i);
		}
		else if(strcmp(cmd, "wait") == 0){
			int i = atoi(args[1]);
			waitpid((pid_t)i, NULL, 0);
		}
		else if(mShellUtility.isSignalCmd(cmd)){
			
			int sig = mShellUtility.getSignal(cmd);
			pid_t killPid  = (pid_t)atoi(args[1]);

			if(kill(killPid, sig) < 0){
				perror("kill() failed");
			}
		}
		else{
			if ((pid = fork()) < 0){
				perror("fork() failed");
				exit(1);
			}
			else if (pid == 0){ // child
				if(isRedirect){
					IORedirect(buf,isBgProcess);
				}
				else if(execvp(cmd, args.data()) < 0){
					perror("execvp() failed");
					_exit(1);
				}
			}
			else{ // parent
				// this works assuming '&' is always at the end of the command arg
				if(!isBgProcess){
					waitpid(pid, NULL, 0);
				}
				else{
					mShellPCB->addPID(pid, buf);
				}
			}
		}
	}
	return !EXIT_SHELL;
}