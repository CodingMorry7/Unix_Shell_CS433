// CS433: Shell source file for HW2
// ====================================================
// HW2 Shell source file
// Names: Haley Minshell, Chris Morikawa
// Compiler:  g++ compiler
// File type: source file Shell.cpp
//=====================================================
#include "Shell.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <vector>
#include <iostream>
using namespace std;

Shell::Shell()
{
	hist_num = 0;
	runBackground = false;
}

Shell::~Shell()
{
	killAllProcesses(0);
}

//=============================================
/*
KillAllProcesses function: Go through the list of
processes that were made by the shell program
and kill every processes.
param: int sig
*/
//=============================================
void Shell::killAllProcesses(int sig)
{
	if(PIDList.size() > 0)//If any processes are in the vector list of PIDs than kill.
	{
		cout << "Ending... Killing all Processes from this session" << endl;
		for(int i = 0; i < (int)PIDList.size(); i++) // Kill all processes that are left runnning then
			// kill the program.
		{
			kill(PIDList[i], SIGKILL);
		}
	} //end of IF
	exit(sig); // Kill the program
} //end of void Function

//===================================
/*
addHistory Function: This function helps with
adding commands to list of commands used before.
Param: string newCmd
*/
//===================================
void Shell::addHistory(string newCmd)
{
	hist_num++; // increment number of commands in history
	history.push_back(newCmd); // insert new command to history vector
}

//================================
/*
display History Function: This is to display
the last ten commands that been used.
*/
//================================
void Shell::displayHistory()
{
	if (history.size() == 0) //If nothing in history than display error.
	{
		cout << "Error, no commands in history."<< endl;
	}
	else //otherwise display the last ten commands used
	{
		int tenthCmd; // index for where history should stop displaying
		if (hist_num > 10) // if hist_num is greater than 10.
		{
			tenthCmd = hist_num - 10;// subtract 10 from hist_num.
		}
		else
		{
			tenthCmd = 0;
		}
		for (int nthCmd = hist_num - 1; nthCmd >= tenthCmd; nthCmd--) // will only display the last 10 items in history
		{
			cout << nthCmd+1 << " " << history[nthCmd] << endl;
		}
	}// end of the else
}

//====================================
/*
 runRecentHistory Function: To pop off of the
 last command used and run it again.
*/
//====================================
void Shell::runRecentHistory()
{
	cout << history.back() << endl;
	parseCmd(history.back());
}

//======================================
/*
 runNthHistory Function: Pick out the specific command
 that the user wants to run again.
 Param: int nthCmd
*/
//======================================
void Shell::runNthHistory(int nthCmd)
{
	cout << history[nthCmd-1] << endl;
	parseCmd(history[nthCmd-1]);
}

//===============================
/*
executeCmd function: Read the command, make a fork() system call,
and push the command onto the terminal screen and execute it.
Param: char* cmd[]
*/
//===============================
void Shell::executeCmd(char* cmd_array[])
{
	pid_t pid = fork(); //fork the process
	PIDList.push_back(getpid()); //grab the process id and place it in the vector.
	//DEBUG cout << "After the fork command, pid is: " << pid << endl;
	if(pid < 0)// If the pid is below zero then report a Fork Failed!
	{
		cout << "Fork Failed!" << endl;
		killAllProcesses(-1);
	}
	if(pid == 0)
	{
		// DEBUG cout << "Child Pid: " << pid << endl;
		if(execvp(cmd_array[0], cmd_array) == -1) //If the command doesn't exist then tell the user.
		{
			cout << "This command doesn't exists!" << endl;
			exit(0);
		}
	}
	if(pid > 0)//If the Process is the Parent than wait
	{
		if (!runBackground)
		{
			if (wait(0) == -1)
				perror("wait");
			// DEBUG cout << "Parent ID: " << pid << endl;
		}
		else
		{
			runBackground = false;
			cout << endl << endl;
		}
	}// end of if (pid > 0)
}//end of the function

//========================================
/*
parseCmd Function: To view the user's input,
check for any specific commands like !!, !#, exit,
history, and if any other regular command then
execute the command.
Param: string cmd_input
*/
//=======================================
void Shell::parseCmd(string cmd_input)
{
	// check for case where empty line is entered as a command
	if (cmd_input == "")
	{
		cout << "Command not found!" << endl;
		return;
	}
	// add commands to history
	if (cmd_input.at(0) != '!')
	{
		addHistory(cmd_input); // add the command to history only if it is not a !! or !# command
	}
  	string key("&"); //To use for the rfind function next line.
	size_t found = cmd_input.rfind(key);//Starts from the rear of the string and
	//finds if there is an '&'
	// check if & is at end of command
	if (found!=std::string::npos) // check if parent should not wait
	{
	  	cmd_input.replace(found,key.length(),"");
                cout << cmd_input << endl;
		runBackground = true;
	}

	// Parse out different types of commands
	if (cmd_input == "exit") // if user enters exit command
	{
		killAllProcesses(-1);
	}
	else if (cmd_input == "history") //If the user entered the command "history"
	{
		displayHistory();
	}
	else if (cmd_input == "!") // exceptin case for when just ! entered
	{
		cout << "Command not found!" << endl;
		return;
	}
	else if (cmd_input == "!!") // if the user enters "!!"
	{
		if (hist_num <= 0) // exception case for no commands in history
		{
			cout << "No commands in history." << endl;
		}
		else //otherwise run this function.
		{
			runRecentHistory();
		}
	}
	else if (cmd_input.at(0) == '!' && isdigit(cmd_input.at(1))) // if user enters "!#"
	{
		cmd_input.erase(cmd_input.begin()); // remove the ! from the command
		int nthCmd = atoi(cmd_input.c_str());// convert string to int for nth command
		if (nthCmd < 1 || nthCmd > (int)(history.size()))
		{
			cout << "No such command in history." << endl;
		}
		else //run the command that the user wants to run again.
		{
			runNthHistory(nthCmd);
		}
	}
	else // for all other commands, execute with unix shell commands
	{
		//Tokenize the user's command, view the command, parse the first part of it.
		char command [50];
		char* cmd_array[30];
		for(int i = 0; i < 30; i++)//Nullify the array
			{
				cmd_array[i] = NULL;
			}
		strcpy(command, cmd_input.c_str());//copy the string into a char array.
		char* p = strtok(command, " ");//tokenize the first word of the command line.
		int x = 0;
		while(p!= NULL) //while there is input til you hit NULL
		{
			cmd_array[x] = p;//insert the word into the command array.
			x++;//On the the next word.
			p = strtok(NULL, " ");//Check the next section if it is NULL. If it is exit while,
			// if not then grab the next data and place it in the cmd_array.
			
		}//After parsing then execute the command.
		executeCmd(cmd_array);
	}//End of the else
}// end of the function
