// -----------------------------------
// FILE: CPathUtils.h
// Copyright (c) Yuriy Guts, 2008
// Last revision date: 2008-12-16
// -----------------------------------
// Warning: This file is an integral part of "Intel 8080 CP/M Assembler" application 
// and should only be distributed with other application components.
//

#pragma once

#include <io.h>

class CPathUtils
{
public:
	static bool FileExists(const char *fileName);
};
