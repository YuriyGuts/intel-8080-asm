// -----------------------------------
// FILE: CSourceLine.h
// Copyright (c) Yuriy Guts, 2008
// Last revision date: 2008-12-20
// -----------------------------------
// Warning: This file is an integral part of "Intel 8080 CP/M Assembler" application 
// and should only be distributed with other application components.
//

#pragma once

#include "SourceLineType.h"

class CSourceLine
{
private:
	int lineNumber;
	int opcodeIndex;
	unsigned long address;
	std::string lineBase;
	std::string label, opcode, firstOperand, secondOperand;
	std::string errorMessage;
	std::string binaryString;
	SourceLineType lineType;

	void Normalize();
	void ExtractLabel();
	void ExtractOpcode();
	void ExtractFirstOperand();
	void ExtractSecondOperand();

public:
	CSourceLine(std::string sourceLine, int number);

	bool TryParse();
	int GetLineNumber() const;
	int GetOpcodeIndex() const;
	unsigned long GetAddress() const;
	void SetAddress(unsigned long instructionAddress);
	SourceLineType GetLineType() const;
	void SetLineType(SourceLineType type);
	std::string GetLine() const;
	std::string GetLabel() const;
	std::string GetOpcode() const;
	std::string GetFirstOperand() const;
	std::string GetSecondOperand() const;
	std::string GetBinaryString() const;
	void SetBinaryString(std::string newString);
	std::string GetErrorMessage() const;
	void SetErrorMessage(std::string message);
	void SetErrorMessage(char *message);

	~CSourceLine(void);
};
