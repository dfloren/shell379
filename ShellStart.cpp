#include "ShellStart.h"
#include "ShellProcessAction.h"
#include "ShellExit.h"

void ShellStart::Init(){
	ShellProcessAction mShellProcessAction;
	ShellExit mShellExit;

	bool exitShell = false;
	while(!exitShell){
		exitShell = mShellProcessAction.ProcessAction();
	}
	mShellExit.exitShell();
}

int main(){
    ShellStart mShellStart;
    mShellStart.Init();
    return 0;
}