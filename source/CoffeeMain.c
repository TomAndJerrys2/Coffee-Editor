// Main File for Handling the Text-Editor by Kyle Brady

#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>

// -- Definitions --
// -----------------
#define CTRL_KEY(k) ((k) & 0x1f)

// -- Data --
// ----------
struct termios original_termios;

// -- Function Prototypes --
// -------------------------
unsigned int enableRawMode();
unsigned int disableRawMode();
void updateKeyPress();
void kill();
char readKey();
void processKey();
void refreshEditor();

// -- Main execution --
// --------------------
int main(int argc, char** argv) {

	// Function Calls
	enableRawMode();

	while(1) {
		processKey();
	}

	return EXIT_SUCCESS;
}


// -- I/O Functions --
// -------------------
unsigned int enableRawMode() {
	// defines a termios struct
	// attributes from the terminal are read by tcgetattr();
	// after modification, we apply them by tcsetattr();
	// TCSAFLSUH is a flah that waits for all input
	// and discards any that is unread
	
	if(tcgetattr(STDIN_FILENO, &original_termios) == -1) {
		kill("tcgetattr");
	}
	atexit(disableRawMode);
	
	// Flag operations
	struct termios raw = original_termios;
	raw.c_iflag &= ~(BRKINT | ICTRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_flag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG); // Input flags come from termios.h
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;	

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

	return 1; // for success
}

// disable the processing
unsigned int disableRawMode() {
	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios) == -1) {
		kill("tcsetattr");
	}
}

// Display Key Presses
void updateKeyPress() {
	char inputChar;
	// detects for input on the buffer
	while(read(STDIN_FILENO, &inputChar, 1) == 1 && inputChar != "q") {
		// Built-in Keyboard functions - ctype.h
		if(iscntrl(c)) {
			printf("%d\r\n", inputChar);
		} else {
			printf("&d ('%c')\r\n", inputChar, inputChar);
		}

		if(inputChar == CTRL_KEY("q"))
			break;
	}
}

// kill str attribute
void kill(const char* ptr) {
	perror(ptr);
	exit(1);
}

// -- Keyboard input functions --
// ------------------------------

char readKey() {
	unsigned int numRead;
	char inputChar;
	while((numRead = read(STDIN_FILENO, &inputChar, 1) != 1) {
		if(numRead == -1 && errno != EAGAIN)
			kill("read");
	}
	return inputChar;
}

void processKey() {
	char inputChar = readKey();
	switch(inputChar) {
		case CTRL_KEY("q"):
			exit(0);
			break;
	}
}

void refreshEditor() {
	write(STDOUT_FILENO, "\x1b[2]", 4);
}
