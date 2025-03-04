// --> main "Minimal-C" terminal header <--

#include <stdio.h>
#include <stdlib.h>

#ifndef TERMINAL_H_
#define TERMINAL_H_

// -- Messages --
#define _CONSOLE_TITLE_ "[Minimal-C Command Terminal]"
#define _INIT_MESSAGE_ "[-- Welcome to the Mininmal-C Command Terminal! --]"
#define _CONSOLE_LINECHAR_ ":\\Command-Terminal>"

// -- Console Commands for In-Built "Apps"
enum consoleAppCommands {
	CALCULATOR = 0, // calculator app -> calculator.c
	PORT_SCANNER = 1, // port scanner -> portScanner.c
	TASK_VIEWER = 2, // windows task viewer PID -> taskViewer.c
	SSH_CONNECT = 3, // connect via ssh -> ssh.c
	SQL_CONNECT = 4, // connect to sql database and use sql -> sql.c
	TEXT_EDITOR = 5, // implements the coffee editor -> coffeeEditor.c
	LOCAL_STRUCTURE = 6, // local file structure, I/O operations -> localStruct.c
	
};

#endif
