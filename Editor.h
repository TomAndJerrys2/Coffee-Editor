#pragma once

#ifndef EDITOR_H
#define EDITOR_H

#include <stdint.h>

typedef struct editor_rows {
	uint8_t row_size; 		// 1. Each row has a buffer size
	char *  buffer_content; 	// 2. Content of buffer
} rows;

#endif
