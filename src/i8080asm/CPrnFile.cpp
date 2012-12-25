// -----------------------------------
// FILE: CPrnFile.cpp
// Copyright (c) Yuriy Guts, 2008
// Last revision date: 2008-12-16
// -----------------------------------
// Warning: This file is an integral part of "Intel 8080 CP/M Assembler" application 
// and should only be distributed with other application components.
//

#include <fstream>

#include "CByteConversionHelper.h"
#include "CCodeGenHelper.h"
#include "CComFile.h"
#include "CPrnFile.h"
#include "CSourceLine.h"
#include "Errors.h"

using namespace std;

CPrnFile::CPrnFile(std::string fileName)
{
	name = fileName;
}

void CPrnFile::Open()
{
	char *fileNameStr = new char[1024];
	fileStream = new ofstream(name.c_str(), ios::out | ios::binary | ios::trunc);
	if (!fileStream->is_open())
	{
		throw I8080_PRN_OPEN_ERROR;
	}
	delete [] fileNameStr;
}

void CPrnFile::WriteSpacer(int length)
{
	try
	{
		for (int i = 0; i < length; i++)
		{
			*fileStream << " ";
		}
	}
	catch (...)
	{
		throw I8080_PRN_WRITE_ERROR;
	}
}

bool CPrnFile::WriteLine(CSourceLine *sourceLine)
{
	const int ADDRESS_COLUMN_WIDTH = 6;
	const int BINARY_CODE_COLUMN_WIDTH = 10;
	const int LABEL_COLUMN_WIDTH = 15;
	const int OPCODE_COLUMN_WIDTH = 8;
	const int LABEL_INDENT_WIDTH = ADDRESS_COLUMN_WIDTH + BINARY_CODE_COLUMN_WIDTH + 1;
	const int OPCODE_INDENT_WIDTH = ADDRESS_COLUMN_WIDTH + BINARY_CODE_COLUMN_WIDTH + OPCODE_COLUMN_WIDTH;

	try
	{
		if (sourceLine->GetLineType() == Instruction)
		{
			unsigned long address = sourceLine->GetAddress();
			
			std::string addressString;
			std::string binaryString = sourceLine->GetBinaryString();
			std::string instructionHexCodeString;

			unsigned long instructionBinaryCode = CByteConversionHelper::ConvertBinaryStringToLong(binaryString);
			instructionHexCodeString = CByteConversionHelper::ConvertLongToHexString(instructionBinaryCode);

			addressString = CByteConversionHelper::ConvertLongToHexString(address);

			*fileStream << addressString << "H";
			WriteSpacer(ADDRESS_COLUMN_WIDTH - addressString.length());

			*fileStream << instructionHexCodeString;
			WriteSpacer(BINARY_CODE_COLUMN_WIDTH - instructionHexCodeString.length());

			if (!sourceLine->GetLabel().empty())
			{
				*fileStream << sourceLine->GetLabel() << ":";
				WriteSpacer(LABEL_COLUMN_WIDTH - sourceLine->GetLabel().length() - 1);
			}
			else
			{
				WriteSpacer(LABEL_COLUMN_WIDTH);
			}

			*fileStream << sourceLine->GetOpcode();
			WriteSpacer(OPCODE_COLUMN_WIDTH - sourceLine->GetOpcode().length());

			if (!sourceLine->GetFirstOperand().empty())
			{
				*fileStream << sourceLine->GetFirstOperand();
				if (!sourceLine->GetSecondOperand().empty())
				{
					*fileStream << ", ";
				}
			}
			if (!sourceLine->GetSecondOperand().empty())
			{
				*fileStream << sourceLine->GetSecondOperand();
			}

			*fileStream << endl;
		}

		if (sourceLine->GetLineType() == Whitespace)
		{
			if (!sourceLine->GetLabel().empty())
			{
				WriteSpacer(LABEL_INDENT_WIDTH);
				*fileStream << sourceLine->GetLabel() << ":";
			}
			*fileStream << endl;
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

void CPrnFile::Close()
{
	fileStream->close();
}

CPrnFile::~CPrnFile(void)
{
	if (fileStream)
	{
		if (fileStream->is_open())
		{
			fileStream->close();
		}
		delete fileStream;
	}
}
