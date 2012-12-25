// -----------------------------------
// FILE: SourceLineType.h
// Copyright (c) Yuriy Guts, 2008
// Last revision date: 2008-12-20
// -----------------------------------
// Warning: This file is an integral part of "Intel 8080 CP/M Assembler" application 
// and should only be distributed with other application components.
//

#pragma once

enum SourceLineType
{
	Error					= 0x00,
	Whitespace				= 0x01,
	Comment					= 0x02,
	Instruction				= 0x04,
	AlignmentDirective		= 0x08
};
