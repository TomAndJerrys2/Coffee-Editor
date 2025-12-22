#pragma once

#ifndef EDITOR_CONFIG_H
#define EDITOR_CONFIG_H

#include "Editor.h"

typedef struct config {
	size_t editor_rows;		// Rows generated buffer
	size_t editor_columns;		// Columns generated buffer
	
	rows current_row;		// Current Row Selected
} EditorConfig;

#endif
