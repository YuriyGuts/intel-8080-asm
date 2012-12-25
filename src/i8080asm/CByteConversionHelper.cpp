// -----------------------------------
// FILE: CByteConversionHelper.cpp
// Copyright (c) Yuriy Guts, 2008
// Last revision date: 2008-12-16
// -----------------------------------
// Warning: This file is an integral part of "Intel 8080 CP/M Assembler" application 
// and should only be distributed with other application components.
//

#include "CByteConversionHelper.h"
#include "CCodeGenHelper.h"
#include "CStringUtils.h"

bool CByteConversionHelper::IsValidBinary(std::string binNumber)
{
	int length = binNumber.length();
	if (length <= 0 || (length == 1 && (binNumber[0] == 'B' || binNumber[0] == 'b')))
	{
		return false;
	}

	for (int i = 0; i < length; i++)
	{
		char currentChar = binNumber[i];
		if ((currentChar == 'B' || currentChar == 'b') && (i < length - 1))
		{
			return false;
		}
		if (currentChar != '0' && currentChar != '1' && currentChar != 'B' && currentChar != 'b')
		{
			return false;
		}
	}
	return true;
}

bool CByteConversionHelper::IsValidHexadecimal(std::string hexNumber)
{
	int length = hexNumber.length();
	if (length <= 0 || (length == 1 && (hexNumber[0] == 'H' || hexNumber[0] == 'h')))
	{
		return false;
	}

	for (int i = 0; i < length; i++)
	{
		char currentChar = hexNumber[i];
		if ((currentChar == 'H' || currentChar == 'h') && (i < length - 1))
		{
			return false;
		}
		if (!(currentChar >= '0' && currentChar <= '9') 
			&& !(currentChar >= 'A' && currentChar <= 'F')
			&& !(currentChar >= 'a' && currentChar <= 'f')
			&& currentChar != 'H' && currentChar != 'h')
		{
			return false;
		}
	}
	return true;
}

bool CByteConversionHelper::IsValidDecimal(std::string decimalNumber)
{
	int length = decimalNumber.length();
	if (length <= 0)
	{
		return false;
	}
	int startIndex = (decimalNumber[0] == '-') ? 1 : 0;
	if (startIndex == 1 && length == 1)
	{
		return false;
	}
	for (int i = startIndex; i < length; i++)
	{
		char currentChar = decimalNumber[i];
		if (!(currentChar >= '0' && currentChar <= '9'))
		{
			return false;
		}
	}
	return true;
}

unsigned long CByteConversionHelper::ConvertDecimalStringToLong(std::string decimalString)
{
	long result = 0;
	result = atol(decimalString.c_str());
	if (result < 0)
	{
		result = 0x10000 + result;
	}
	return (unsigned long)result;
}

unsigned long CByteConversionHelper::ConvertHexStringToLong(std::string hexString)
{
	unsigned long result = 0;
	int startIndex = hexString.length() - 1;
	if (hexString[startIndex] == 'H' || hexString[startIndex] == 'h')
	{
		startIndex--;
	}

	for (int i = startIndex, power = 0; i >= 0; i--, power++)
	{
		char currentChar = hexString[i];
		if (currentChar >= '1' && currentChar <= '9')
		{
			result += (unsigned long)((currentChar - '0') * (1 << (power * 4)));
		}
		if (currentChar >= 'A' && currentChar <= 'F')
		{
			result += (unsigned long)((currentChar - 'A' + 10) * (1 << (power * 4)));
		}
		if (currentChar >= 'a' && currentChar <= 'f')
		{
			result += (unsigned long)((currentChar - 'a' + 10) * (1 << (power * 4)));
		}
	}
	return result;
}

unsigned long CByteConversionHelper::ConvertBinaryStringToLong(std::string binString)
{
	unsigned long result = 0;
	int startIndex = binString.length() - 1;
	if (binString[startIndex] == 'B' || binString[startIndex] == 'b')
	{
		startIndex--;
	}
	
	for (int i = startIndex, power = 0; i >= 0; i--, power++)
	{
		if (binString[i] == '1')
		{
			result += (unsigned long)(1 << power);
		}
	}
	return result;
}

bool CByteConversionHelper::TryConvertNumericStringToLong(std::string numericString, unsigned long *result)
{
	if (numericString.length() > 0)
	{
		char lastChar = numericString[numericString.length() - 1];
		if ((lastChar >= '0' || lastChar <= '9') && CByteConversionHelper::IsValidDecimal(numericString))
		{
			*result = CByteConversionHelper::ConvertDecimalStringToLong(numericString);
			return (*result <= MAX_NUMERIC_OPERAND_VALUE);
		}
		if ((lastChar == 'H' || lastChar == 'h') && CByteConversionHelper::IsValidHexadecimal(numericString))
		{
			*result = CByteConversionHelper::ConvertHexStringToLong(numericString);
			return (*result <= MAX_NUMERIC_OPERAND_VALUE);
		}
		if ((lastChar == 'B' || lastChar == 'b') && CByteConversionHelper::IsValidBinary(numericString))
		{
			*result = CByteConversionHelper::ConvertBinaryStringToLong(numericString);
			return (*result <= MAX_NUMERIC_OPERAND_VALUE);
		}
	}
	*result = 0;
	return false;
}

std::string CByteConversionHelper::ConvertLongToBinaryString(unsigned long value, bool lowerBytesFirst)
{
	char bitChar;
	int bitInsertionIndex;

	std::string result = std::string(32, ' ');
	int length = result.length();

	for (int i = 0; i < length; i++)
	{
		bitChar = (value & (1 << i)) ? '1' : '0';
		bitInsertionIndex = (lowerBytesFirst) ? ((i / 8) * 8 + 7 - (i % 8)) : (length - i - 1);
		if (bitInsertionIndex >= 0 && bitInsertionIndex < length)
		{
			result[bitInsertionIndex] = bitChar;
		}
	}

	CStringUtils::TrimAll(result);
	return result;
}

std::string CByteConversionHelper::ConvertLongToHexString(unsigned long value)
{
	std::string result = std::string(8, ' ');
	for (int i = 3; i >= 0; i--)
	{
		char highTetrade = (value & (0xF0 << (i * 8))) >> (i * 8 + 4);
		char lowTetrade = (value & (0x0F << (i * 8))) >> (i * 8);

		result[6 - i * 2] = (highTetrade >= 0 && highTetrade <= 9) ? highTetrade + 48 : highTetrade + 55;
		result[6 - i * 2 + 1] = (lowTetrade >= 0 && lowTetrade <= 9) ? lowTetrade + 48 : lowTetrade + 55;
	}
	while (result.find("00") == 0 && result != "00")
	{
		result.erase(0, 2);
	}

	CStringUtils::TrimAll(result);
	return result;
}
