#include <sys/wait.h>
#include "ShellExit.h"
#include "ShellPCB.h"

void ShellExit::exitShell(){
	while (wait(NULL) > 0){} // wait for all child processes to terminate

	printf("Resources used: \n");
	ShellPCB::Instance()->showCumulativeTime();
	exit(1);
};