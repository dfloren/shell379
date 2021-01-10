#ifndef SHELL_PA_H
#define SHELL_PA_H

class ShellProcessAction{
public:
	bool ProcessAction();
private:
	void IORedirect(char command[], bool isBgProcess);
};

#endif