Specifications:

The program shell379 is a command line interface. It accepts a specific set of simple commands.
The use of system() is not allowed for pedagogical purposes.

exit 			End the execution of shell379. Wait until all processes initiated by the
				shell are complete. Print out the total user and system time for all
				processes run by the shell.

jobs 			Display the status of all running processes spawned by shell379.

kill <int>		Kill process <int>

resume <int>	Resume the execution of process <int>. This undoes a suspend.

sleep <int>		Sleep for <int> seconds

suspend <int>	Suspend execution of process <int>. A resume will reawaken it.

wait <pid>		Wait until process <int> has completed execution.

If none of the above commands is input, then the resulting input string is to be executed by shell379.
<cmd> <arg>* Spawn a process to execute command <cmd> with 0 or more arguments <arg>.
			 <cmd> and <arg> are each one or more sequences of nonblank characters.

There are three special arguments that a command may have:
& 		If used, this must be the last argument and indicates that the command is
		to be executed in the background.

<fname 	This argument is the “<” character followed by a string of characters, a file
		name to be used for program input.

>fname 	This argument is the “>” character followed by a string of characters, a file
		name to be used for program output.





Notes:
1. The constants MAX_ARGS, MAX_LENGTH, MAX_PT_ENTRIES defined in ShellUtility.cpp are not used
as a result of using vectors and strings that are dynamically allocated.

2. Commands run in the foreground, i.e. commands without a '&' at the end, are not inserted into
the local process control block table since the assumption is user cannot issue a 'jobs' command while
said foreground process is executing. Time consumption of foreground process is still accounted
for by getrusage(). 


Known bugs:
1. Grandchildren processes are not being tracked and added to the pidTable.
Example: 'time xeyes &' spawn both 'time' and 'xeyes' but only 'time' is in pidTable

2. pid header length not being updated upon removal of a pid.