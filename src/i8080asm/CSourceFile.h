// -----------------------------------
// FILE: CSourceFile.h
// Copyright (c) Yuriy Guts, 2008
// Last revision date: 2008-12-16
// -----------------------------------
// Warning: This file is an integral part of "Intel 8080 CP/M Assembler" application 
// and should only be distributed with other application components.
//

#pragma once

#include <fstream>
#include <iostream>
#include "CLabel.h"
#include "CSourceLine.h"

using namespace std;

class CSourceFile
{
private:
	ifstream *fileStream;
	std::string fileName;
	CSourceLine **lines;
	CLabel **labels;
	int operationAlignment;
	int lineCount;
	int labelCount;

	void InitializeParser();
	void CountLabels();
	void CreateLabelList();
	void AssignBinaryStrings();
	unsigned long GetLabelAddressByName(std::string labelName) const;
	void WriteComFile();
	void WritePrnFile();

public:
	CSourceFile(std::string sourceFileName);

	void Assign(std::string sourceFileName);
	void Open();
	void Read();
	void Close();

	void Parse();
	bool HasErrors();
	void PrintErrors(ostream &outputStream);
	void WriteComAndPrnFile();

	void GetName(std::string buffer) const;
	CSourceLine ** GetLines() const;
	int GetLineCount() const;
	int GetOperationAlignment() const;

	~CSourceFile(void);
};
