// -----------------------------------
// FILE: i8080asm.cpp
// Copyright (c) Yuriy Guts, 2008
// Last revision date: 2008-12-20
// -----------------------------------
// Warning: This file is an integral part of "Intel 8080 CP/M Assembler" application 
// and should only be distributed with other application components.
//

#include <algorithm>
#include <iostream>
#include <windows.h>

#include "CByteConversionHelper.h"
#include "CSourceFile.h"
#include "Errors.h"
#include "SourceLineType.h"

using namespace std;
void WriteDebugInfo(CSourceFile *source);

int main(int argCount, char *args[])
{
	cout << "Intel 8080 CP/M Assembler" << endl;
	cout << "Version 1.2 (Win32). Copyright (c) Yuriy Guts, 2008" << endl << endl;

	bool debugMode = (argCount == 3 && strcmp(args[2], "/debug") == 0);

	if (!(argCount == 2 || debugMode))
	{
		cout << "Error: Invalid number of parameters!" << endl << endl;
		cout << "Usage: I8080ASM <asmFile>[.ASM]" << endl;
		return 1;
	}

	LARGE_INTEGER startPoint, endPoint, frequency;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startPoint);

	std::string fileName = args[1];
	std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::toupper);

	if (fileName.find(".ASM") == string::npos)
	{
		fileName.append(".ASM");
	}

	CSourceFile *source = new CSourceFile(fileName);
	try
	{
		source->Open();
		cout << "Processing file '" << fileName << "'." << endl;
		cout << "Reading source... ";
		source->Read();
		cout << "OK" << endl;
		source->Close();
		cout << "Parsing lines... ";
		source->Parse();
		if (source->HasErrors())
		{
			cout << "FAILED" << endl;
			source->PrintErrors(cout);
			cout << endl << "Unable to generate COM file due to errors in the source code." << endl << endl;
		}
		else
		{
			cout << "OK" << endl;
			cout << "Generating COM and PRN files... ";
			source->WriteComAndPrnFile();
			cout << "OK" << endl;

			QueryPerformanceCounter(&endPoint);
			long timeSpent = (endPoint.QuadPart - startPoint.QuadPart) * 1e6 / frequency.QuadPart;
			cout << endl << "Done in " << (double)timeSpent / 1000 << " ms." << endl << endl;
		}
	}
	catch (I8080_EXCEPTION ex)
	{
		switch (ex)
		{
		case I8080_E_FILE_NOT_EXISTS:
			cout << "Error: File '" << args[1] << "' does not exist!" << endl << endl;
			cout << "Usage: I8080ASM <asmFile>[.ASM]" << endl;
			return 1;

		case I8080_COM_OPEN_ERROR:
			cout << endl << "Error: Unable to create COM file! Please check your write permissions." << endl;
			return 1;

		case I8080_COM_WRITE_ERROR:
			cout << endl << "Error: Unable to write to COM file! Please check your write permissions." << endl;
			return 1;

		case I8080_PRN_OPEN_ERROR:
			cout << endl << "Error: Unable to create PRN file! Please check your write permissions." << endl;
			return 1;

		case I8080_PRN_WRITE_ERROR:
			cout << endl << "Error: Unable to write to PRN file! Please check your write permissions." << endl;
			return 1;
		}
	}
	catch (char *ex)
	{
		cout << endl << "ERROR: Unhandled exception occurred while processing the file. The program will now terminate." << endl << endl;
		cout << "Exception details: " << ex << endl;
		return 1;
	}
	catch (...)
	{
		cout << endl << "ERROR: Unhandled exception occurred while processing the file. The program will now terminate." << endl << endl;
		return 1;
	}

	if (debugMode)
	{
		WriteDebugInfo(source);
	}

	bool hasErrors = source->HasErrors();
	delete source;

	return (hasErrors ? 1 : 0);
}

void WriteDebugInfo(CSourceFile *source)
{
	for (int i = 0; i < source->GetLineCount(); i++)
	{
		CSourceLine *currentLine = source->GetLines()[i];
		if (currentLine->GetLineType() == Instruction)
		{
			std::string binaryString = currentLine->GetBinaryString();
			unsigned long instructionBinaryCode = CByteConversionHelper::ConvertBinaryStringToLong(binaryString);
			std::string instructionHexCodeString = CByteConversionHelper::ConvertLongToHexString(instructionBinaryCode);

			cout << currentLine->GetAddress() 
				 << " : " 
				 << currentLine->GetLineNumber()
				 << " | "
				 << currentLine->GetLine()
				 << " | "
				 << instructionHexCodeString
				 << " | "
				 << binaryString
				 << endl;
		}
	}
}