#ifndef EDITOR_H
#define EDITOR_H

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>

#define KEDITOR_VERSION "0.2.1"
#define KEDITOR_TAB_STOP 4
#define KEDITOR_QUIT_TIMES 1

#define CTRL_KEY(k) ((k) & 0x1f)

#endif
