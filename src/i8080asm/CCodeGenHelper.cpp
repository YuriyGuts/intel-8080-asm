// -----------------------------------
// FILE: CCodeGenHelper.cpp
// Copyright (c) Yuriy Guts, 2008
// Last revision date: 2008-12-16
// -----------------------------------
// Warning: This file is an integral part of "Intel 8080 CP/M Assembler" application 
// and should only be distributed with other application components.
//

#include "CCodeGenHelper.h"

CCodeGenHelper::CCodeGenHelper(void)
{
}

unsigned long CCodeGenHelper::GetOpcodeHash(std::string opcode)
{
	unsigned long result = 0;

	if (opcode.length() > MAX_OPCODE_LENGTH)
	{
		return -1;
	}
	for (int i = 0; i < (int)opcode.length(); i++)
	{
		result += (unsigned long)(opcode[i] * pow((double)256, i));
	}

	return result;
}

int CCodeGenHelper::GetOpcodeHashIndex(unsigned long opcodeHash)
{
	int left = 0;
	int right = OPCODE_COUNT - 1;

	while (left <= right)
	{
		int middle = (left + right) >> 1;
		if (opcodeHash == OPCODE_HASHES[middle])
		{
			return middle;
		}
		if (opcodeHash > OPCODE_HASHES[middle])
		{
			left = middle + 1;
		}
		if (opcodeHash < OPCODE_HASHES[middle])
		{
			right = middle - 1;
		}
	}
	return -1;
}

bool CCodeGenHelper::GetRegisterCode(std::string mnemonic, std::string result)
{
	for (int i = 0; i < REGISTER_MNEMONIC_COUNT; i++)
	{
		if (mnemonic == REGISTER_MNEMONICS[i])
		{
			result = REGISTER_CODES[i];
			return true;
		}
	}
	return false;
}

bool CCodeGenHelper::GetRegisterPairCode(std::string mnemonic, std::string result)
{
	for (int i = 0; i < REGISTER_PAIR_MNEMONIC_COUNT; i++)
	{
		if (mnemonic == REGISTER_PAIR_MNEMONICS[i])
		{
			result = REGISTER_PAIR_CODES[i];
			return true;
		}
	}
	return false;
}

CCodeGenHelper::~CCodeGenHelper(void)
{
}
