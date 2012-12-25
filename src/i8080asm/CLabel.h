// -----------------------------------
// FILE: CLabel.h
// Copyright (c) Yuriy Guts, 2008
// Last revision date: 2008-12-16
// -----------------------------------
// Warning: This file is an integral part of "Intel 8080 CP/M Assembler" application 
// and should only be distributed with other application components.
//

#pragma once

#include <string>

class CLabel
{
private:
	unsigned long address;
	std::string name;

public:
	CLabel(std::string labelName, unsigned long labelAddress);

	std::string  GetName() const;
	unsigned long GetAddress() const;

	~CLabel(void);
};
