// -----------------------------------
// FILE: CByteConversionHelper.h
// Copyright (c) Yuriy Guts, 2008
// Last revision date: 2008-12-16
// -----------------------------------
// Warning: This file is an integral part of "Intel 8080 CP/M Assembler" application 
// and should only be distributed with other application components.
//

#pragma once

#include <string>

class CByteConversionHelper
{
public:
	static bool IsValidBinary(std::string binNumber);
	static bool IsValidHexadecimal(std::string hexNumber);
	static bool IsValidDecimal(std::string decimalNumber);
	static unsigned long ConvertHexStringToLong(std::string hexString);
	static unsigned long ConvertDecimalStringToLong(std::string decimalString);
	static unsigned long ConvertBinaryStringToLong(std::string binString);
	static bool TryConvertNumericStringToLong(std::string numericCstring, unsigned long *result);
	static std::string ConvertLongToBinaryString(unsigned long value, bool lowerBytesFirst);
	static std::string ConvertLongToHexString(unsigned long value);
};

