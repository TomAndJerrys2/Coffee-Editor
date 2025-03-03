// Main File for Handling the Text-Editor by Kyle Brady

// -- Possible Features --
// -----------------------

// 1) Multiple language compiler / interpreter support with visual highlighting
// 2) Custom File manager
// 3) Custom Intellisense
// 4) CoffeeLang in-built support
// 5) JSON Reading/Writing for configs such as Custom key bindings, macros etc
// 6) Front-end interface re-implementation

// -- Libraries -- 
// ---------------

#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/iotcl.h>
#include <sys/types.h>
#include <string.h>

// -- Definitions --
// -----------------
#define CTRL_KEY(k) ((k) & 0x1f) // control key

#define BUF_SIZE 64 // stream buffer size

#define APPENDBUFFER_INIT {NULL, 0} // appended buffer size

#define COFFEE_VERSION 1.2	// version

// -- Data --
// ----------
typedef struct editorRow {
	int size;
	char* rowBuffer;
} erow;

struct coffeeConfig {
	
	int Cursor_x;
	int Cursor_y;
	
	int screenRows;
	int screenColumns;
	
	int numRows;
	erow row;
	
	struct termios original_termios;
};

// appending file buffer
struct appendBuffer {
	char* buffer;
	int len;
};

// struct object for the overall editor
struct coffeeConfig configObj;

// Editor programmed-keys
enum editorKeys {
	ARROW_LEFT = 1000,
	ARROW_RIGHT,
	ARROW_UP,
	ARROW_DOWN,
	PAGE_UP,
	PAGE_DOWN,
	HOME_KEY,
	END_KEY,
	DEL_KEY
};

// -- Function Prototypes --
// -------------------------
unsigned int enableRawMode();
unsigned int disableRawMode();
void updateKeyPress();
void kill();
char readKey();
void processKey();
const void refreshEditor();
void drawEditorRows();
int getWindowSize(int* rows,int* columns);
int getCursorPos(int* rows, int* columns);

// Buffer Operations
void aBuffer_Free(struct appendBuffer* _AB);
void aBuffer_add(struct appenBuffer* _AB, const char* str, int len);

void editorMoveCursor(char key);
void spawnEditor();

// Initialize Editor
void initEditor() {
	configObj.Cursor_x = 0;
	configObj.Cusor_y = 0;
	configObj.numRows = 0;
	
	if(getWindowSize(&configObj.screenRows, &configObj.screenColumns) == -1) {
		kill("getWindowSize");
	}
}

// -- Main execution --
// --------------------
int main(int argc, char** argv) {

	// Function Calls
	enableRawMode();
	initEditor();
	spawnEditor();

	while(1) {
		// Will implement threading on different function calls
		refreshEditor();
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
	
	if(tcgetattr(STDIN_FILENO, &configObj.original_termios) == -1) {
		kill("tcgetattr");
	}
	atexit(disableRawMode);
	
	// Flag operations
	struct termios raw = configObj.original_termios;
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
	// killing the  attribute tcsetattr when raw mode is disabled
	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &configObj.original_termios) == -1) {
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
	refreshEditor();
	// throws an error if it fails
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
	// This primarily checked which sequence of stream characters is being
	// inputted into the editor, and based of this if special keys are used as
	// command keys the editor will respond accordingly with functionaility
	// spread throughout the different functions
	if(inputChar == '\x1b') {
		char sequence[3];
		
		if(read(STDIN_FILENO, &sequence[0], 1) != 1) return '\x1b';
		if(read(STDIN_FILENO, &sequence[1], 1) != 1) return '\x1b';
		
		if(sequence[0] == '[') {
			if(sequence[1] >= '0' && sequence[1] <= '9') return '\x1b';
			if(sequence[2] == '~') {
				switch(sequence[1]) {
					case '1': return HOME_KEY;
					case '3': return DEL_KEY;
					case '4': return END_KEY;
					case '5': return PAGE_UP;
					case '6': return PAGE_DOWN;
					case '7': return HOME_KEY;
					case '8': return END_KEY;
				}
			}
		} else if(sequence[0] == 'O') {
			switch(sequence[1]) {
				case 'H': return HOME_KEY;
				case 'F': return END_KEY;
			}
		} else {
			switch(sequence[1]) {
				case 'A': return ARROW_UP;
				case 'B': return ARROW_DOWN;
				case 'C': return ARROW_RIGHT;
				case 'D': return ARROW_LEFT;
				case 'H': return HOME_KEY;
				case 'F': return END_KEY;
			}
		}
		
		return '\x1b';
	} else {
		return inputChar;
	}
}

// Processing the different keys via the configObj
void processKey() {
	char inputChar = readKey();
	switch(inputChar) {
		case CTRL_KEY("q"):
			write(STDOUT_FILENO, "\x1b[2J", 4);
			write(STDOUT_FILENO, "\x1b[H", 3);
			exit(0);
			break;
			
		case HOME_KEY:
			configObj.Cursor_x = 0;
			break;
			
		case END_KEY:
			configObj.Cursor_x = configObj.screenColumns - 1;
			break;
			
		case PAGE_UP: case PAGE_DOWN: {
			int times = configObj.screenRows;
			while(times--) {
				editorMoveCursor(inputChar == PAGE_UP ? ARROW_UP : ARROW_DOWN);
			}
		} break;
			
		case ARROW_UP: case ARROW_DOWN: case ARROW_LEFT: case ARROW_RIGHT:
			editorMoveCursor(inputChar);
			break;
	}
}

// For refreshing line buffers and mouse position etc.
const void refreshEditor() {
	struct appendBuffer _AB = APPENDBUFFER_INIT;
	
	aBuffer_add(&_AB, "\x1b[?25l", 6);
	aBuffer_add(&_AB, "\x1b[H", 3);
	
	drawEditorRows(&_AB);
	
	char internal_buffer[32];
	snprintf(internal_buffer, sizeof(internal_buffer), "\x1b[%d:%dH", configObj.Cursor_y + 1, configObj.Cursor_x + 1);
	aBuffer_add(&_AB, internal_buffer, strlen(internal_buffer));
	
	aBuffer_add(&_AB, "\x1b[H", 3);
	aBuffer_add(&_AB, "\x1b[?25H", 6);
	
	write(STDOUT_FILENO, _AB.buffer, _AB.len);
	aBuffer_Free(&_AB);
}

// Draw the editor for the user interface
void drawEditorRows(struct appendBuffer* _AB) {
	int num;
	for(num = 0; num < configObj.screenRows; num++){
		if(num >= configObj.numRows) {
			if(num == configObj.screenRows / 3) {
				char welcome[80];
				int welcomeLength = snprintf(welcome, sizeof(welcome), "Coffee Editor -- Version %s", COFFEE_VERSION);
				
				if(welcomeLength > configObj.screenColumns) welcomeLength = configObj.screenColumns;
				
				int padding = (configObj.screenColumns - welcomeLength) / 2;
				if(padding) {
					aBuffer_add(_AB, "~",  1);
					padding--;
				}
				while(padding--) aBuffer_add(_AB, " ", 1);
				
				aBuffer_add(_AB, welcome, welcomeLength);
			} else {
				aBuffer_add(_AB, "~", 1);
			}
		} else {
			int length = configObj.row.size;
			if(length > configObj.screenColumns)
				length = config.screenColumns;
			aBuffer_add(_AB, configObj.row.rowBuffer, length);
		}
		
		aBuffer_add(_AB, "\x1b[K", 3);
		if(num < configObj.screenRows - 1) {
			aBuffer_add(_AB, "\r\n", 2);
		}
	}
}

// gets and applies window size of the editor back to the editor config
int getWindowSize(int* rows, int* columns) {
	struct WindowSize winSize;
	
	if(ioctl(STDOU_FILENO, TIOCGWINSZ, &winSize) == -1 || winSize.winSize_col == 0) {
		if(write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
 
		return getCursorPos(rows, columns);
		
		else {
			*columns = winSize.winSize_col;
			*rows = winSize.winSize.row;
			return 0;
		}
	}
}

// Editor cursor for moving around the text editor
int getCursorPos(int* rows, int* columns) {
	char buffer[BUF_SIZE];
	unsigned int num = 0;
	
	if(write(STDOU_FILENO, "\x1b[6n", 4) != 4) 
		return -1;
	
	while(num < sizeof(buffer) - 1) {
		if(read(STDIN_FILENO, &buffer[num], 1) != 1)
			break;
		
		if(buffer[num] == 'R')
			break;
		
		num++;
	} buffer = '\0';
	
	if(buf[0] != '\x1b' || buf[1] != '[') return -1;
	if(sscanf(&buf[2], "%d;%d", rows, columns) != 2) return -1;
	
	return 0;
}

// add a new appended buffer
void aBuffer_add(struct appendBuffer* _AB, const char* str, int len) {
	char* new = realloc(_AB->buffer, _AB->len + len);
	
	if(new = NULL) return;
	memccpy(&new[_AB->len], str, len);
	_AB->buffer = new;
	_AB->len += len;
}

// free the buffer for a memory-safe operation
void aBuffer_Free(struct appendBuffer* _AB) {
	free(_AB->buffer);
}

// moving the cursor increases/decreases the config value
// depending on direction moved via a key press																			
void editorMoveCursor(char key) {
	switch(tolower(&key)) {
		case ARROW_LEFT:
			if(configObj.Cursor_x != 0)
				configObj.Cursor_x++;
			break;
		case ARROW_RIGHT:
			if(configObj.Cursor_x != configObj.screenColumns - 1)
				configObj.Cursor_x++;
			break;
		case ARROW_UP:
			if(configObj.Cursor_y != 0)
				configObj.Cursor_y--;
			break;
		case ARROW_DOWN:
			if(configObk.Cursor_y != configObj.screenRows - 1)
				configObk.Cursor_y++;
			break;
	}
}

// -- File I/O Operations --
// -------------------------
void spawnEditor() {
	char* line = "Hello, World!";
	ssize_t lineLength = 13;
	
	configObj.row.size = lineLength;
	configObj.row.rowBuffer = malloc(lineLength +  1);
	
	memcpy(configObj.row.rowBuffer, line, lineLength);
	
	configObj.row.rowBuffer[lineLength] = '\0';
	configObj.numRows = 1;
}