// -----------------------------------
// FILE: CPathUtils.cpp
// Copyright (c) Yuriy Guts, 2008
// Last revision date: 2008-12-16
// -----------------------------------
// Warning: This file is an integral part of "Intel 8080 CP/M Assembler" application 
// and should only be distributed with other application components.
//

#include "CPathUtils.h"

bool CPathUtils::FileExists(const char *fileName)
{
	_finddata_t findData;
	intptr_t findHandle = _findfirst(fileName, &findData);
	if (findHandle != -1)
	{
		_findclose(findHandle);
		return true;
	}
	return false;
}
