#include <cstring>
#include <sys/resource.h> // rusage, RUSAGE_CHILDREN
#include "ShellPCB.h"
#include "ShellUtility.cpp"

using std::to_string;
using std::stoi;

#define MIN_PADDING 1;

ShellPCB* ShellPCB::instance = 0;


ShellPCB* ShellPCB::Instance(){
	if(instance == NULL){
		instance = new ShellPCB();
	}
	return instance;
}


void ShellPCB::showCumulativeTime(){
	struct rusage rUsage;

	getrusage(RUSAGE_CHILDREN, &rUsage);

	printf("User time:	%ld seconds\n", rUsage.ru_utime.tv_sec);
	printf("Sys  time:	%ld seconds\n", rUsage.ru_stime.tv_sec);
	return;
}


/*
Read the output of ps. For each pid appearing in the PCB,
update the status and time. Print all entries of PCB.
*/
void ShellPCB::showJobs(){
	FILE* f;
	int chars;
	int activeCount = -1;
	int runningCount = -1;
	char psChars[BUFSIZ + 1];
	memset(psChars, '\0', sizeof(psChars));

	f = popen("ps -o pid,stat=S,time=SEC,comm --no-headers","r");

	if(f != NULL){
		chars = fread(psChars, sizeof(char), BUFSIZ, f);
		if(chars > 0){
			ShellUtility mShellUtility;

			string psString(psChars);
			vector<string> psLines = mShellUtility.splitString(psString, '\n');

			for(size_t j = 0; j < psLines.size(); j++){
				string tmpLine1 = psLines[j];
				vector<string> tmpTokens = mShellUtility.removeWhiteSpaces(tmpLine1);
				
				pid_t tmpId = stoi(tmpTokens[0]);
				for(size_t i = 0; i < PCB.size(); i++){
					if(PCB[i].pid == tmpId){
						PCB[i].status = tmpTokens[1].at(0);
						PCB[i].time = mShellUtility.HHMMSStoS(tmpTokens[2]);
						if(PCB[i].status == "R"){
							runningCount++;
						}
						activeCount++;

						// update status, time header spacing
						if(hdrLength.hStatus < PCB[i].status.size()){
							hdrLength.hStatus = PCB[i].status.size();
						}
						if(hdrLength.hTime < to_string(PCB[i].time).size()){
							hdrLength.hTime = to_string(PCB[i].time).size();
						}
						break;
					}
				}
			}
		}
	}
	pclose(f);
	printf("Running processes: \n");
	if(activeCount > -1){
		// an attempt to space-format the output
		// format: <#> <PID> <STATUS> <TIME> <COMMAND>
		int padNum, padPid, padStat, padTime;
		string strCount = "#";
		string strPid = "PID";
		string strStatus = "S";
		string strTime = "SEC";
		string strCommand = "COMMAND";

		if(to_string(activeCount).size() > strCount.size()){
			padNum = to_string(activeCount).size();
		}
		else{
			padNum = MIN_PADDING;
		}

		padPid = getHdrPadding(hdrLength.hPid, strPid.size());
		padStat = getHdrPadding(hdrLength.hStatus, strStatus.size());
		padTime = getHdrPadding(hdrLength.hTime, strTime.size());

		printf("%*s%s %*s%s%*s%s%*s%s %s\n", padNum, " ", strCount.c_str(), padPid, " ", 
			strPid.c_str(), padStat, " ", strStatus.c_str(), padTime, "", 
			strTime.c_str(), strCommand.c_str());

		int hdrNum = padNum + strCount.size();
		int hdrPid = padPid + strPid.size();
		int hdrStat = padStat + strStatus.size();
		int hdrTime = padTime + strTime.size();

		int eNum, ePid, eStat, eTime;
		for(size_t i = 0; i < PCB.size(); i++){
			eNum = getEntryPadding(to_string(i).size(), hdrNum);
			ePid = getEntryPadding(to_string(PCB[i].pid).size(), hdrPid);
			eStat = getEntryPadding(PCB[i].status.size(), hdrStat);
			eTime = getEntryPadding(to_string(PCB[i].time).size(), hdrTime);

			printf("%*s%d:%*s%d%*s%s%*s%d %s", eNum, " ", (int)i, ePid, " ", PCB[i].pid, eStat, "", 
				PCB[i].status.c_str(), eTime, " ", PCB[i].time, PCB[i].command.c_str());
		}
	}
	printf("Processes = 	%d active\n", runningCount+1);
	printf("Completed processes:\n");
	showCumulativeTime();

	return;
}


int ShellPCB::getHdrPadding(int entry, int header){
	int result;
	if(entry > header){
		result = entry - header + MIN_PADDING;
	}
	else{
		result = MIN_PADDING;
	}
	return result;
}


int ShellPCB::getEntryPadding(int entry, int header){
	int result;
	if(entry < header){
		result = header - entry;
	}
	else{
		result = MIN_PADDING;
	}
	return result;
}


void ShellPCB::addPID(pid_t id, char command[]){
	pcbEntry* newEntry = new pcbEntry(id, 0, "", string(command));
	PCB.push_back(*newEntry);

	// update pid header spacing
	if(hdrLength.hPid < to_string(id).size()){
		hdrLength.hPid = to_string(id).size();
	}
	return;
}


void ShellPCB::removePID(pid_t id){
	if(PCB.size() < 1){return;}
	for(size_t i = 0; i < PCB.size(); i++){
		if(PCB[i].pid == id){
			PCB.erase(PCB.begin() + i);
			break;
		}
	}
	return;
}

int ShellPCB::getProcessCount(){
	return PCB.size();
}
