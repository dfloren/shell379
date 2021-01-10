#include <string>
#include <vector>

#ifndef SHELL_PCB_H
#define SHELL_PCB_H

using std::string;
using std::vector;

// Singleton implementation of PCB
class ShellPCB{
public:
	static ShellPCB* Instance();

	void showCumulativeTime();

	void showJobs();

	void addPID(pid_t id, char command[]);

	void removePID(pid_t id);

	int getProcessCount();

private:
	ShellPCB(){};
	ShellPCB(ShellPCB const&);
	ShellPCB& operator=(ShellPCB const&);

	static ShellPCB* instance;

	int getHdrPadding(int entry, int header);
	int getEntryPadding(int entry, int header);


	struct pcbEntry{
		pid_t pid;
		uint time;
		string status;
		string command;

		pcbEntry(pid_t pid, uint time, string status, string command)
		:pid(pid)
		,time(time)
		,status(status)
		,command(command)
		{}
	};


	struct headerLength{
		unsigned long int hPid = 0;
		unsigned long int hTime = 0;
		unsigned long int hStatus = 0;
	} hdrLength;


	vector<pcbEntry> PCB;

};

#endif