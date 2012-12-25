// -----------------------------------
// FILE: CComFile.h
// Copyright (c) Yuriy Guts, 2008
// Last revision date: 2008-12-16
// -----------------------------------
// Warning: This file is an integral part of "Intel 8080 CP/M Assembler" application 
// and should only be distributed with other application components.
//

#pragma once
#include <fstream>
#include <string>

using namespace std;

class CComFile
{
private:
	ofstream *fileStream;
	std::string name;

public:
	CComFile(std::string name);

	bool WriteByte(unsigned char byte);
	bool WriteInstruction(unsigned long instructionCode);
	bool WriteSpacer(int byteCount);

	void Open();
	void Close();

	~CComFile(void);
};
