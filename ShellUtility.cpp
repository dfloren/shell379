#include <string>
#include <vector>
#include <cstring> // strcmp()
#include <sstream> // std::istringstream
#include <iterator> // std::istream_iterator
#include <unistd.h> // read(), STDIN_FILENO
#include <sys/signal.h> // kill(), SIGKILL

using namespace std;

#ifndef SHELL_UTILITY_CPP
#define SHELL_UTILITY_CPP

#define LINE_LENGTH     100 // Max # of characters in an input line
#define MAX_ARGS	      7 // Max # of arguments to a command
#define MAX_LENGTH       20 // Max # of characters in an argument
#define MAX_PT_ENTRIES   32 // Max entries in the process table

class ShellUtility{
public:
	/*
	Split string into vector using a one-character delimiter
	*/
	vector<string> splitString(string str, char delim){

		vector<string> vStr;

		if(str.size() < 1){return vStr;}

		string token;
		istringstream strStream(str);
		while( getline(strStream, token, delim) ){
			if(token[token.size()-1] == '\n' && token != "\n"){
				token.erase(token.size()-1, 1);
			}
			vStr.push_back(token);
		}

		return vStr;
	}

	/*
	Remove all whitespaces and split string into vector
	*/
	vector<string> removeWhiteSpaces(string str){
		vector<string> vStr;

		if(str.size() < 1){return vStr;}

		istringstream stream(str);
		string token;

		while(stream >> token){
			vStr.push_back(token);
		}
		return vStr;
	}

	/*
	Outputs a null-terminated char* vector from an input string vector

	Note: Removes "&" of vStr if it is the last argument.

	WARNING: The char* elements will point to the elements of the string vector.
		   Deleting the input vector will cause char* elements to point to null!
	*/
	vector<char*> vStrToChar(vector<string> &vStr){
		if(vStr[vStr.size()-1].compare("&") == 0){
			vStr.erase(vStr.end());
		}

		vector<char*> vChar(vStr.size() + 1);
		for(int i = 0; i < (int)vStr.size(); ++i){
			vChar[i] = &(vStr[i][0]);
		}

		return vChar;
	}

	uint HHMMSStoS(string str){
		uint seconds = 0;

		vector<string> tokens = splitString(str, ':');

		if(tokens.size() != 3){return seconds;}

		uint hr = stoi(tokens[0])*60*60;
		uint min = stoi(tokens[1])*60;
		uint sec = stoi(tokens[2]);

		seconds = hr + min + sec;

		return seconds;
	}

	/*
	Returns true if the last string of vStr is a "&"
	*/
	bool isBgProcess(vector<string> vStr){
		bool isBgProcess = false;
		if(vStr[vStr.size()-1].compare("&") == 0){isBgProcess = true;}
		return isBgProcess;
	}

	/*
	Format: <cmd> <inputfile >outputfile
	*/
	bool isRedirect(vector<string> vStr){
		bool isRedirect = false;
		for(size_t i = 0; i < vStr.size(); i++){
			if(vStr[i][0] == '>' ||
			   vStr[i][0] == '<')
			{
				isRedirect = true;
				break;
			}
		}
		return isRedirect;
	}

	/*
	format: kill/resume/suspend <pid>
	*/
	bool isSignalCmd(char* cmd){
		bool isSignalCmd = false;
		if((strcmp(cmd, "kill") == 0)
			|| (strcmp(cmd, "resume") == 0) 
			|| (strcmp(cmd, "suspend") == 0)){
			isSignalCmd = true;
		}
		return isSignalCmd;
	}

	int getSignal(char* sig){
		int signal;

		if(strcmp(sig,"kill") == 0){
			signal = SIGKILL;
		}
		else if(strcmp(sig,"resume") == 0){
			signal = SIGCONT;
		}
		else if(strcmp(sig, "suspend") == 0){
			signal = SIGSTOP;
		}
		else{
			signal = 0;
		}

		return signal;
	}
};

#endif