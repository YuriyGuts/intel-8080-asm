// -----------------------------------
// FILE: CSourceFile.cpp
// Copyright (c) Yuriy Guts, 2008
// Last revision date: 2008-12-16
// -----------------------------------
// Warning: This file is an integral part of "Intel 8080 CP/M Assembler" application 
// and should only be distributed with other application components.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "CByteConversionHelper.h"
#include "CCodeGenHelper.h"
#include "CComFile.h"
#include "CLabel.h"
#include "CPathUtils.h"
#include "CPrnFile.h"
#include "CSourceFile.h"
#include "CStringUtils.h"
#include "Errors.h"

using namespace std;

CSourceFile::CSourceFile(std::string sourceFileName)
{
	Assign(sourceFileName);
}

void CSourceFile::Assign(std::string sourceFileName)
{
	this->fileName = sourceFileName;
	operationAlignment = 0x100;
	lineCount = 0;

	fileStream = NULL;
	lines = NULL;
	labels = NULL;
}

void CSourceFile::Open()
{
	if (!CPathUtils::FileExists(fileName.c_str()))
	{
		throw I8080_E_FILE_NOT_EXISTS;
	}

	fileStream = new ifstream();
	fileStream->open(fileName.c_str(), ios::in);
}

void CSourceFile::Read()
{
	lineCount = 0;

	char *buffer = new char[1024];
	while (!fileStream->eof())
	{
		fileStream->getline(buffer, 1023);
		lineCount++;
	}

	lines = new CSourceLine *[lineCount];
	fileStream->clear();
	fileStream->seekg(0);

	int currentLineIndex = 0;
	while (!fileStream->eof())
	{
		fileStream->getline(buffer, 1024);
		std::string currentLine = buffer;
		lines[currentLineIndex] = new CSourceLine(currentLine, currentLineIndex + 1);
		currentLineIndex++;
	}

	delete [] buffer;
	return;
}

void CSourceFile::Close()
{
	fileStream->close();
}

void CSourceFile::InitializeParser()
{
	for (int i = 0; i < lineCount; i++)
	{
		lines[i]->TryParse();
		if (lines[i]->GetLineType() == AlignmentDirective)
		{
			operationAlignment = CByteConversionHelper::ConvertHexStringToLong(lines[i]->GetFirstOperand());
		}
		if (lines[i]->GetLineType() == Error && lines[i]->GetErrorMessage().empty())
		{
			lines[i]->SetErrorMessage((char *)I8080_ERROR_UNKNOWN_INSTRUCTION);
		}
	}
}

void CSourceFile::CountLabels()
{
	labelCount = 0;
	for (int i = 0; i < lineCount; i++)
	{
		if (!(lines[i]->GetLabel().empty()))
		{
			labelCount++;
		}
	}
}

void CSourceFile::CreateLabelList()
{
	labels = new CLabel *[labelCount];

	int currentLabelIndex = 0;
	unsigned long currentAddress = operationAlignment;
	for (int i = 0; i < lineCount; i++)
	{
		if (!(lines[i]->GetLabel().empty()))
		{
			labels[currentLabelIndex++] = new CLabel(lines[i]->GetLabel(), currentAddress);
		}
		if (lines[i]->GetLineType() == Instruction)
		{
			unsigned char bytesUsed = OPERATION_SIZES[lines[i]->GetOpcodeIndex()];
			lines[i]->SetAddress(currentAddress);
			currentAddress += bytesUsed;
		}
	}
}

void CSourceFile::AssignBinaryStrings()
{
	std::string registerCode;
	std::string registerPairCode;
	std::string byteString;
	std::string numString;
	std::string addressString;
	std::string wordString;

	for (int i = 0; i < lineCount; i++)
	{
		if (lines[i]->GetLineType() == Instruction)
		{
			std::string binaryString = OPCODE_MASKS[lines[i]->GetOpcodeIndex()];

			unsigned long immediateConstant;

			if (!lines[i]->GetFirstOperand().empty() && CCodeGenHelper::GetRegisterCode(lines[i]->GetFirstOperand(), registerCode))
			{
				CStringUtils::ReplaceAll(binaryString, std::string("{REG1}"), registerCode);
			}
			if (!lines[i]->GetSecondOperand().empty() && CCodeGenHelper::GetRegisterCode(lines[i]->GetSecondOperand(), registerCode))
			{
				CStringUtils::ReplaceAll(binaryString, std::string("{REG2}"), registerCode);
			}
			if (!lines[i]->GetFirstOperand().empty() && CCodeGenHelper::GetRegisterPairCode(lines[i]->GetFirstOperand(), registerPairCode))
			{
				CStringUtils::ReplaceAll(binaryString, std::string("{RP}"), registerPairCode);
			}
			if (!lines[i]->GetFirstOperand().empty() && CByteConversionHelper::TryConvertNumericStringToLong(lines[i]->GetFirstOperand(), &immediateConstant))
			{
				byteString = CByteConversionHelper::ConvertLongToBinaryString(immediateConstant, true);
				wordString = CByteConversionHelper::ConvertLongToBinaryString(immediateConstant, true);

				numString = byteString.substr(5, 3);
				CStringUtils::ReplaceAll(binaryString, std::string("{NUM}"), numString);
				CStringUtils::ReplaceAll(binaryString, std::string("{BYTE}"), byteString);
				CStringUtils::ReplaceAll(binaryString, std::string("{WORD}"), wordString);
			}
			if (!lines[i]->GetSecondOperand().empty() && CByteConversionHelper::TryConvertNumericStringToLong(lines[i]->GetSecondOperand(), &immediateConstant))
			{
				byteString = CByteConversionHelper::ConvertLongToBinaryString(immediateConstant, true);
				wordString = CByteConversionHelper::ConvertLongToBinaryString(immediateConstant, true);
				CStringUtils::ReplaceAll(binaryString, std::string("{BYTE}"), byteString);
				CStringUtils::ReplaceAll(binaryString, std::string("{WORD}"), wordString);
			}
			if (!lines[i]->GetFirstOperand().empty() && GetLabelAddressByName(lines[i]->GetFirstOperand()))
			{
				addressString = CByteConversionHelper::ConvertLongToBinaryString(GetLabelAddressByName(lines[i]->GetFirstOperand()), true);
				CStringUtils::ReplaceAll(binaryString, std::string("{ADDR}"), addressString);
			}

			if (!CByteConversionHelper::IsValidBinary(binaryString))
			{
				lines[i]->SetLineType(Error);
				if (lines[i]->GetErrorMessage().empty())
				{
					lines[i]->SetErrorMessage((char *)I8080_ERROR_INVALID_OPERANDS);
				}
			}

			lines[i]->SetBinaryString(binaryString);
		}
	}
}

void CSourceFile::Parse()
{
	InitializeParser();
	CountLabels();
	CreateLabelList();
	AssignBinaryStrings();
}

void CSourceFile::GetName(std::string buffer) const
{
	buffer = fileName;
}

CSourceLine ** CSourceFile::GetLines() const
{
	return lines;
}

int CSourceFile::GetLineCount() const
{
	return lineCount;
}

int CSourceFile::GetOperationAlignment() const
{
	return operationAlignment;
}

unsigned long CSourceFile::GetLabelAddressByName(std::string labelName) const
{
	if (labels)
	{
		for (int i = 0; i < labelCount; i++)
		{
			if (labels[i]->GetName() == labelName)
			{
				return labels[i]->GetAddress();
			}
		}
	}
	return 0;
}

bool CSourceFile::HasErrors()
{
	for (int i = 0; i < lineCount; i++)
	{
		if (lines[i]->GetLineType() == Error)
		{
			return true;
		}
	}
	return false;
}

void CSourceFile::PrintErrors(ostream &outputStream)
{
	for (int i = 0; i < lineCount; i++)
	{
		if (lines[i]->GetLineType() == Error)
		{
			outputStream << "Error at line " << lines[i]->GetLineNumber();
			if (!lines[i]->GetLine().empty())
			{
				outputStream << " (" << lines[i]->GetLine() << ")";
			}
			if (!lines[i]->GetErrorMessage().empty())
			{
				outputStream << ": " << lines[i]->GetErrorMessage();
			}
			outputStream << endl;
		}
	}
}

void CSourceFile::WriteComFile()
{
	std::string comFileName = fileName;
	std::transform(comFileName.begin(), comFileName.end(), comFileName.begin(), ::toupper);
	CStringUtils::ReplaceAll(comFileName, std::string(".ASM"), std::string(".COM"));

	CComFile *comFile = new CComFile(comFileName);
	comFile->Open();

	if (!comFile->WriteSpacer(operationAlignment - 0x100))
	{
		throw I8080_COM_WRITE_ERROR;
	}

	for (int i = 0; i < lineCount; i++)
	{
		if (lines[i]->GetLineType() == Instruction)
		{
			unsigned long instructionCode = CByteConversionHelper::ConvertBinaryStringToLong(lines[i]->GetBinaryString());
			if (!comFile->WriteInstruction(instructionCode))
			{
				throw I8080_COM_WRITE_ERROR;
			}
		}
	}
	comFile->Close();

	delete comFile;
}

void CSourceFile::WritePrnFile()
{
	std::string prnFileName = fileName;
	std::transform(prnFileName.begin(), prnFileName.end(), prnFileName.begin(), ::toupper);
	CStringUtils::ReplaceAll(prnFileName, std::string(".ASM"), std::string(".PRN"));

	CPrnFile *prnFile = new CPrnFile(prnFileName);
	prnFile->Open();
	for (int i = 0; i < lineCount; i++)
	{
		if (!prnFile->WriteLine(lines[i]))
		{
			throw I8080_PRN_WRITE_ERROR;
		}
	}
	prnFile->Close();

	delete prnFile;
}

void CSourceFile::WriteComAndPrnFile()
{
	WriteComFile();
	WritePrnFile();
}

CSourceFile::~CSourceFile(void)
{
	if (fileStream)
	{
		delete fileStream;
	}
	if (lines)
	{
		for (int i = 0; i < lineCount; i++)
		{
			if (lines[i])
			{
				delete lines[i];
			}
		}
		delete [] lines;
	}
	if (labels)
	{
		for (int i = 0; i < labelCount; i++)
		{
			if (labels[i])
			{
				delete labels[i];
			}
		}
		delete [] labels;
	}
}
