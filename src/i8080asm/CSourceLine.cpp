// -----------------------------------
// FILE: CSourceLine.cpp
// Copyright (c) Yuriy Guts, 2008
// Last revision date: 2008-12-20
// -----------------------------------
// Warning: This file is an integral part of "Intel 8080 CP/M Assembler" application 
// and should only be distributed with other application components.
//

#include <algorithm>
#include <cctype>
#include <string>

#include "CByteConversionHelper.h"
#include "CCodeGenHelper.h"
#include "CSourceLine.h"
#include "CStringUtils.h"
#include "Errors.h"
#include "SourceLineType.h"

CSourceLine::CSourceLine(std::string sourceLine, int number)
{
	lineBase = sourceLine;
	lineNumber = number;
	opcodeIndex = -1;

	label = "";
	opcode = "";
	firstOperand = "";
	secondOperand = "";
	binaryString = "";
	errorMessage = "";
}

void CSourceLine::Normalize()
{
	// Removing comments
	int commentIndex = lineBase.find(COMMENT_CHAR);
	if (commentIndex > 0)
	{
		lineBase = lineBase.substr(0, commentIndex);
	}

	// Removing tabs and extra whitespaces
	CStringUtils::ReplaceAll(lineBase, std::string("\t"), std::string(" "));
	CStringUtils::TrimAll(lineBase);
	while (lineBase.find("  ") != std::string::npos)
	{
		CStringUtils::ReplaceAll(lineBase, std::string("  "), std::string(" "));
	}
	CStringUtils::ReplaceAll(lineBase, std::string(": "), std::string(":"));
	CStringUtils::ReplaceAll(lineBase, std::string(", "), std::string(","));
	std::transform(lineBase.begin(), lineBase.end(), lineBase.begin(), ::toupper);
}

bool CSourceLine::TryParse()
{
	const int MAX_BINARY_STRING_SIZE = 24;

	Normalize();
	if (lineBase.length() == 0)
	{
		lineType = Whitespace;
		return true;
	}
	if (lineBase[0] == COMMENT_CHAR)
	{
		lineType = Comment;
		return true;
	}

	ExtractLabel();
	ExtractOpcode();
	ExtractFirstOperand();
	ExtractSecondOperand();

	if (lineBase.length() == 0)
	{
		lineType = Whitespace;
		return true;
	}
	if (opcode == ALIGNMENT_DIRECTIVE)
	{
		if (CByteConversionHelper::IsValidHexadecimal(firstOperand))
		{
			unsigned long alignment = CByteConversionHelper::ConvertHexStringToLong(firstOperand);
			if (alignment >= 0x100 && alignment <= 0xFFFF)
			{
				lineType = AlignmentDirective;
				return true;
			}
			else
			{
				SetErrorMessage((char *)I8080_ERROR_INVALID_ALIGNMENT_DIRECTIVE);
				lineType = Error;
				return false;
			}
		}
	}

	unsigned long opcodeHash = CCodeGenHelper::GetOpcodeHash(opcode);
	int opcodeHashIndex = CCodeGenHelper::GetOpcodeHashIndex(opcodeHash);
	if (opcodeHashIndex >= 0)
	{
		lineType = Instruction;
		opcodeIndex = opcodeHashIndex;
		binaryString = "";
		return true;
	}

	lineType = Error;
	return false;
}

void CSourceLine::ExtractLabel()
{
	int labelEndIndex = lineBase.find(LABEL_END_CHAR);
	if (labelEndIndex > 0)
	{
		if (lineBase[0] >= '0' && lineBase[0] <= '9')
		{
			SetErrorMessage((char *)I8080_ERROR_INVALID_LABEL);
			return;
		}

		for (int i = 0; i < labelEndIndex; i++)
		{
			char currentChar = lineBase[i];
			if (!((currentChar >= 'A' && currentChar <= 'Z') 
				|| (currentChar >= 'a' && currentChar <= 'z') 
				|| (currentChar >= '0' && currentChar <= '9') 
				|| currentChar == '_'))
			{
				SetErrorMessage((char *)I8080_ERROR_SYNTAX_ERROR);
				return;
			}
		}

		label = lineBase.substr(0, labelEndIndex);
		lineBase.erase(0, labelEndIndex + 1);
		CStringUtils::TrimAll(lineBase);
	}
}

void CSourceLine::ExtractOpcode()
{
	int spaceIndex = lineBase.find(' ');
	if (spaceIndex > 0)
	{
		opcode = lineBase.substr(0, spaceIndex);
		return;
	}
	else
	{
		opcode = lineBase;
		return;
	}
	opcode = "";
}

void CSourceLine::ExtractFirstOperand()
{
	int spaceIndex = lineBase.find(' ');
	int commaIndex = lineBase.find(',');

	if (spaceIndex > 0 && commaIndex > 0 && commaIndex > spaceIndex)
	{
		firstOperand = lineBase.substr(spaceIndex + 1, commaIndex - spaceIndex - 1);
		return;
	}
	if (spaceIndex > 0 && commaIndex < 0 && spaceIndex < (int)lineBase.length() - 1)
	{
		firstOperand = lineBase.substr(spaceIndex + 1);
		return;
	}

	firstOperand = "";
}

void CSourceLine::ExtractSecondOperand()
{
	int commaIndex = lineBase.find(',');
	if (commaIndex >= 0 && commaIndex < (int)lineBase.length() - 1)
	{
		secondOperand = lineBase.substr(commaIndex + 1);
	}
	else
	{
		secondOperand = "";
	}
}

int CSourceLine::GetOpcodeIndex() const
{
	return opcodeIndex;
}

int CSourceLine::GetLineNumber() const
{
	return lineNumber;
}

unsigned long CSourceLine::GetAddress() const
{
	return address;
}

void CSourceLine::SetAddress(unsigned long instructionAddress)
{
	address = instructionAddress;
}

std::string CSourceLine::GetLine() const
{
	return lineBase;
}

std::string  CSourceLine::GetLabel() const
{
	return label;
}

SourceLineType CSourceLine::GetLineType() const
{
	return lineType;
}

void CSourceLine::SetLineType(SourceLineType type)
{
	lineType = type;
}

std::string CSourceLine::GetOpcode() const
{
	return opcode;
}

std::string CSourceLine::GetFirstOperand() const
{
	return firstOperand;
}

std::string CSourceLine::GetSecondOperand() const
{
	return secondOperand;
}

std::string CSourceLine::GetBinaryString() const
{
	return binaryString;
}

void CSourceLine::SetBinaryString(std::string newString)
{
	binaryString = newString;
}

std::string CSourceLine::GetErrorMessage() const
{
	return errorMessage;
}

void CSourceLine::SetErrorMessage(std::string message)
{
	errorMessage = message;
}

void CSourceLine::SetErrorMessage(char *message)
{
	errorMessage = message;
}

CSourceLine::~CSourceLine(void)
{
}
