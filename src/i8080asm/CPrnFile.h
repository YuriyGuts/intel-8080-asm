// -----------------------------------
// FILE: CPrnFile.h
// Copyright (c) Yuriy Guts, 2008
// Last revision date: 2008-12-16
// -----------------------------------
// Warning: This file is an integral part of "Intel 8080 CP/M Assembler" application 
// and should only be distributed with other application components.
//

#pragma once

#include "CSourceLine.h"

class CPrnFile
{
private:
	ofstream *fileStream;
	std::string name;

public:
	CPrnFile(std::string fileName);

	void Open();
	bool WriteLine(CSourceLine *sourceLine);
	void WriteSpacer(int length);
	void Close();

	~CPrnFile(void);
};
