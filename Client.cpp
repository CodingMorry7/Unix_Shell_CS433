/*//==============================================
CS433HW2 Unix Shell
Names: Haley Minshell, Chris Morikawa
Date: 10/7/2016
Course: CS433

Assignment: Programming Assignment 2
Description: Designing a C program to serve as a sheel interface that accpets user
commands and thenexecutes each command in a separate process.

Instructions: 
1. In order to complie the program, first type make on the command line.
2. Then you will see a executable called cs_433_hw2.
3. Type ./cs433_hw2 to run the program.

Complier: g++
File type: source file
*///=============================================
#include "Shell.h"
#include <string>
#include <iostream>
using namespace std;
//============================
/*
Function: The driver to run the shell program.
*/
//=============================
int main()
{
	Shell shell; // new shell program
	string cmd_input; //used to take the user's input

	while(cmd_input!="exit")//While the user didn't enter exit.
	{
		cout << "MM_shell> ";//Prompt for the user to enter the command
		getline(cin,cmd_input);//grab the user's command
		shell.parseCmd(cmd_input);//Execute the command through the functions.
	}
	return 1;
}//end of main
