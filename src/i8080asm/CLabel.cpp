// -----------------------------------
// FILE: CLabel.cpp
// Copyright (c) Yuriy Guts, 2008
// Last revision date: 2008-12-16
// -----------------------------------
// Warning: This file is an integral part of "Intel 8080 CP/M Assembler" application 
// and should only be distributed with other application components.
//

#include "CLabel.h"

CLabel::CLabel(std::string labelName, unsigned long labelAddress)
{
	name = labelName;
	address = labelAddress;
}

std::string  CLabel::GetName() const
{
	return name;
}

unsigned long CLabel::GetAddress() const
{
	return address;
}

CLabel::~CLabel(void)
{
}
