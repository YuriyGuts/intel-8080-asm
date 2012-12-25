// -----------------------------------
// FILE: CStringUtils.cpp
// Copyright (c) Yuriy Guts, 2008
// Last revision date: 2008-12-16
// -----------------------------------
// Warning: This file is an integral part of "Intel 8080 CP/M Assembler" application 
// and should only be distributed with other application components.
//

#include <cctype>

#include "CStringUtils.h"

void CStringUtils::ReplaceAll(std::string& str, std::string& from, std::string& to)
{
	if (from.empty())
	{
		return;
	}

	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

void CStringUtils::TrimAll(std::string &str)
{   
	std::string s = str;
	std::string::size_type pos = s.find_last_not_of(' ');
	if (pos != std::string::npos)
	{
		s.erase(pos + 1);
		pos = s.find_first_not_of(' ');
		if (pos != std::string::npos)
		{
			s.erase(0, pos);
		}
	}
	else
	{
		s.erase(s.begin(), s.end());
	}
	str = s;
}
