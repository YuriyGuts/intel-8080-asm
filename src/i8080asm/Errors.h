// -----------------------------------
// FILE: Errors.h
// Copyright (c) Yuriy Guts, 2008
// Last revision date: 2008-12-16
// -----------------------------------
// Warning: This file is an integral part of "Intel 8080 CP/M Assembler" application 
// and should only be distributed with other application components.
//

#pragma once

// Exception definitions
typedef int I8080_EXCEPTION;

static const I8080_EXCEPTION	I8080_E_FILE_NOT_EXISTS		= 1001;
static const I8080_EXCEPTION	I8080_COM_OPEN_ERROR		= 1002;
static const I8080_EXCEPTION	I8080_COM_WRITE_ERROR		= 1003;
static const I8080_EXCEPTION	I8080_PRN_OPEN_ERROR		= 1004;
static const I8080_EXCEPTION	I8080_PRN_WRITE_ERROR		= 1005;

// Error messages
static const char I8080_ERROR_INVALID_ALIGNMENT_DIRECTIVE[] =
	"Invalid alignment directive. Address must be in [0100H-0FFFFH] range.";
static const char I8080_ERROR_INVALID_LABEL[] =
	"Invalid label. Label names should not start with digits and should contain only alpha characters, digits and underscore characters.";
static const char I8080_ERROR_UNKNOWN_INSTRUCTION[] =
	"Unknown instruction";
static const char I8080_ERROR_INVALID_OPERANDS[] = 
	"Invalid operands provided for the operation";
static const char I8080_ERROR_SYNTAX_ERROR[] = 
	"Syntax error";
