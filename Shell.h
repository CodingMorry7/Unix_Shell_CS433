// CS433: Shell header file for HW2
// ====================================================
// HW2 Shell header
// Names: Haley Minshell, Chris Morikawa
// Compiler:  g++ compiler
// File type: header file Shell.h
//=====================================================

#ifndef SHELL_H
#define SHELL_H
#include <string>
#include <vector>
using namespace std;

class Shell
{
private:
	bool runBackground; // boolean if there is an ampersand at the end of the command
	int hist_num; // number of commands in history
	vector<int> PIDList; // Vector list of processes.
	vector<string> history; // vector of commands entered.

public:
	//Constructor and Destructor
	Shell();
	~Shell();

	/*
	KillAllProcesses function: Go through the list of
	processes that were made by the shell program
	and kill every processes.
	param: int sig
	*/
	void killAllProcesses(int);

	/*
	addHistory Function: This function helps with
	adding commands to list of commands used before.
	Param: string newCmd
	*/
	void addHistory(string);

	/*
	display History Function: This is to display
	the last ten commands that been used.
	*/
	void displayHistory();

	/*
	 runRecentHistory Function: To pop off of the
	 last command used and run it again.
	*/
	void runRecentHistory();

	/*
	 runNthHistory Function: Pick out the specific command
	 that the user wants to run again.
	 Param: int nthCmd
	*/
	void runNthHistory(int);

	/*
	executeCmd function: Read the command, make a fork() system call,
	and push the command onto the terminal screen and execute it.
	Param: char* cmd[]
	*/
	void executeCmd (char* []);

	/*
	parseCmd Function: To view the user's input,
	check for any specific commands like !!, !#, exit,
	history, and if any other regular command then
	execute the command.
	Param: string cmd_input
	*/
	void parseCmd(string);
};

#endif
