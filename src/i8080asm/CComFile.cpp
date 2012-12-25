// -----------------------------------
// FILE: CComFile.cpp
// Copyright (c) Yuriy Guts, 2008
// Last revision date: 2008-12-16
// -----------------------------------
// Warning: This file is an integral part of "Intel 8080 CP/M Assembler" application 
// and should only be distributed with other application components.
//

#include <fstream>

#include "CComFile.h"
#include "CCodeGenHelper.h"
#include "Errors.h"

using namespace std;

CComFile::CComFile(std::string fileName)
{
	name = fileName;
}

void CComFile::Open()
{
	fileStream = new ofstream(name.c_str(), ios::out | ios::binary | ios::trunc);
	if (!fileStream->is_open())
	{
		throw I8080_COM_OPEN_ERROR;
	}
}

void CComFile::Close()
{
	fileStream->close();
}

bool CComFile::WriteByte(unsigned char byte)
{
	try
	{
		*fileStream << byte;
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool CComFile::WriteInstruction(unsigned long instructionCode)
{
	bool result = true;

	if (instructionCode == 0)
	{
		WriteByte(0);
		return true;
	}
	int highestByteNumber = 0;
	for (int i = 3; i >= 0; i--)
	{
		if (instructionCode & (0xFF << (i * 8)))
		{
			highestByteNumber = i;
			break;
		}
	}
	for (int i = highestByteNumber; i >= 0; i--)
	{
		unsigned char byteToWrite = (instructionCode & (0xFF << (i * 8))) >> (i * 8);
		result &= WriteByte(byteToWrite);
	}
	return result;
}

bool CComFile::WriteSpacer(int byteCount)
{
	bool result = true;
	for (int i = 0; i < byteCount; i++)
	{
		result &= WriteByte((unsigned char)0);
	}
	return result;
}

CComFile::~CComFile(void)
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
