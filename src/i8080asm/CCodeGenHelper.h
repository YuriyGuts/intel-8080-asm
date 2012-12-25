// -----------------------------------
// FILE: CCodeGenHelper.h
// Copyright (c) Yuriy Guts, 2008
// Last revision date: 2008-12-16
// -----------------------------------
// Warning: This file is an integral part of "Intel 8080 CP/M Assembler" application 
// and should only be distributed with other application components.
//

#pragma once

#ifndef NULL
#define NULL 0
#endif

#include <string>

static const int OPCODE_COUNT = 78;
static const int REGISTER_MNEMONIC_COUNT = 8;
static const int REGISTER_PAIR_MNEMONIC_COUNT = 5;

static const unsigned long MAX_NUMERIC_OPERAND_VALUE = 0xFFFF;
static const int MAX_OPCODE_LENGTH = 4;
static const char COMMENT_CHAR = ';';
static const char LABEL_END_CHAR = ':';
static const char ALIGNMENT_DIRECTIVE[] = "ORG";

static const char OPCODES[OPCODE_COUNT][MAX_OPCODE_LENGTH + 1] =
{
	"CC",   "JC",   "RC",   "DI",   "EI",   "CM",   "JM",   "RM",   "IN",   "CP",
	"JP",   "RP",   "CZ",   "JZ",   "RZ",   "DAA",  "LDA",  "CMA",  "ANA",  "ORA",
	"XRA",  "STA",  "SBB",  "SUB",  "ADC",  "RLC",  "CMC",  "CNC",  "JNC",  "RNC",
	"RRC",  "STC",  "DAD",  "ADD",  "CPE",  "JPE",  "RPE",  "SBI",  "ACI",  "ADI",
	"ANI",  "CPI",  "ORI",  "XRI",  "SUI",  "MVI",  "LXI",  "RAL",  "CPO",  "JPO",
	"RPO",  "CMP",  "JMP",  "NOP",  "POP",  "RAR",  "DCR",  "INR",  "RET",  "HLT",
	"RST",  "OUT",  "MOV",  "DCX",  "INX",  "CNZ",  "JNZ",  "RNZ",  "LHLD", "SHLD",
	"XCHG", "PUSH", "PCHL", "SPHL", "XTHL", "CALL", "LDAX", "STAX"
};

static const char OPCODE_MASKS[OPCODE_COUNT][20] =
{
	// CC,                JC,                  RC,                DI,                EI,         
	"11011100{ADDR}",     "11011010{ADDR}",    "11011000",        "11110011",        "11111011",       
	// CM,                JM,                  RM,                IN,                CP,
	"11111100{ADDR}",     "11111010{ADDR}",    "11111000",        "11011011{BYTE}",  "11110100{ADDR}",
	// JP,                RP,                  CZ,                JZ,                RZ,         
	"11110010{ADDR}",     "11110000",          "11001100{ADDR}",  "11001010{ADDR}",  "11001000",       
	// DAA,               LDA,                 CMA,               ANA,               ORA,
	"00100111",           "00111010{WORD}",    "00101111",        "10100{REG1}",     "10110{REG1}",
	// XRA,               STA,                 SBB,               SUB,               ADC,        
	"10101{REG1}",        "00110010{WORD}",    "10011{REG1}",     "10010{REG1}",     "10001{REG1}",    
	// RLC,               CMC,                 CNC,               JNC,               RNC,
	"00000111",           "00111111",          "11010100{ADDR}",  "11010010{ADDR}",  "11010000",
	// RRC,               STC,                 DAD,               ADD,               CPE,        
	"00001111",           "00110111",          "00{RP}1001",      "10000{REG1}",     "11101100{ADDR}", 
	// JPE,               RPE,                 SBI,               ACI,               ADI,
	"11101010{ADDR}",     "11100000",          "11011110{BYTE}",  "11001110{BYTE}",  "11000110{BYTE}",
	// ANI,               CPI,                 ORI,               XRI,               SUI,        
	"11100110{BYTE}",     "11111110{BYTE}",    "11110110{BYTE}",  "11101110{BYTE}",  "11010110{BYTE}", 
	// MVI,               LXI,                 RAL,               CPO,               JPO,
	"00{REG1}110{BYTE}",  "00{RP}0001{WORD}",  "00010111",        "11100100{ADDR}",  "11100010{ADDR}",
	// RPO,               CMP,                 JMP,               NOP,               POP,        
	"11100000",           "10111{REG1}",       "11000011{ADDR}",  "00000000",        "11{RP}0001",     
	// RAR,               DCR,                 INR,               RET,               HLT,
	"00011111",           "00{REG1}101",       "00{REG1}100",     "11001001",        "01110110",
	// RST,               OUT,                 MOV,               DCX,               INX,       
	"11{NUM}111",         "11010011{BYTE}",    "01{REG1}{REG2}",  "00{RP}1011",      "00{RP}0011",     
	// CNZ,               JNZ,                 RNZ,               LHLD,              SHLD,
	"11000100{ADDR}",     "11000010{ADDR}",    "11000000",        "00101010{WORD}",  "00100010{WORD}",
	// XCHG,              PUSH,                PCHL,              SPHL,              XTHL,       
	"11101011",           "11{RP}0101",        "11101001",        "11111001",        "11100011",       
	// CALL,              LDAX,                STAX
	"11001101{ADDR}",     "00{RP}1010",        "00{RP}0010"
};

static const unsigned long OPCODE_HASHES[OPCODE_COUNT] = 
{
	// CC,      JC,         RC,         DI,         EI,         CM,         JM,         RM,         IN,         CP,
	17219,      17226,      17234,      18756,      18757,      19779,      19786,      19794,      20041,      20547,
	// JP,      RP,         CZ,         JZ,         RZ,         DAA,        LDA,        CMA,        ANA,        ORA,
	20554,      20562,      23107,      23114,      23122,      4276548,    4277324,    4279619,    4279873,    4280911,
	// XRA,     STA,        SBB,        SUB,        ADC,        RLC,        CMC,        CNC,        JNC,        RNC,
	4280920,    4281427,    4342355,    4347219,    4408385,    4410450,    4410691,    4410947,    4410954,    4410962,
	// RRC,     STC,        DAD,        ADD,        CPE,        JPE,        RPE,        SBI,        ACI,        ADI,
	4411986,    4412499,    4473156,    4473921,    4542531,    4542538,    4542546,    4801107,    4801345,    4801601,
	// ANI,     CPI,        ORI,        XRI,        SUI,        MVI,        LXI,        RAL,        CPO,        JPO,
	4804161,    4804675,    4805199,    4805208,    4805971,    4806221,    4806732,    4997458,    5197891,    5197898,
	// RPO,     CMP,        JMP,        NOP,        POP,        RAR,        DCR,        INR,        RET,        HLT,
	5197906,    5262659,    5262666,    5263182,    5263184,    5390674,    5391172,    5393993,    5522770,    5524552,
	// RST,     OUT,        MOV,        DCX,        INX,        CNZ,        JNZ,        RNZ,        LHLD,       SHLD,
	5526354,    5526863,    5656397,    5784388,    5787209,    5918275,    5918282,    5918290,    1145849932, 1145849939,
	// XCHG,    PUSH,       PCHL,       SPHL,       XTHL,       CALL,       LDAX,       STAX
	1195918168, 1213420880, 1279804240, 1279807571, 1279808600, 1280065859, 1480672332, 1480676435
};

static const unsigned char OPERATION_SIZES[OPCODE_COUNT] = 
{
	// CC,      JC,         RC,         DI,         EI,         CM,         JM,         RM,         IN,         CP,
	3,          3,          1,          1,          1,          3,          3,          1,          2,          3,
	// JP,      RP,         CZ,         JZ,         RZ,         DAA,        LDA,        CMA,        ANA,        ORA,
	3,          1,          3,          3,          1,          1,          3,          1,          1,          1,
	// XRA,     STA,        SBB,        SUB,        ADC,        RLC,        CMC,        CNC,        JNC,        RNC,
	1,          3,          1,          1,          1,          1,          1,          3,          3,          1,
	// RRC,     STC,        DAD,        ADD,        CPE,        JPE,        RPE,        SBI,        ACI,        ADI,
	1,          1,          1,          1,          3,          3,          1,          2,          2,          2,
	// ANI,     CPI,        ORI,        XRI,        SUI,        MVI,        LXI,        RAL,        CPO,        JPO,
	2,          2,          2,          2,          2,          2,          3,          1,          3,          3,
	// RPO,     CMP,        JMP,        NOP,        POP,        RAR,        DCR,        INR,        RET,        HLT,
	1,          1,          3,          1,          1,          1,          1,          1,          1,          1,
	// RST,     OUT,        MOV,        DCX,        INX,        CNZ,        JNZ,        RNZ,        LHLD,       SHLD,
	1,          2,          1,          1,          1,          3,          3,          1,          3,          3,
	// XCHG,    PUSH,       PCHL,       SPHL,       XTHL,       CALL,       LDAX,       STAX
	1,          1,          1,          1,          1,          3,          1,          1			            
};

static char REGISTER_MNEMONICS[REGISTER_MNEMONIC_COUNT][5] = 
	{ "A", "B", "C", "D", "E", "H", "L", "M" };

static char REGISTER_CODES[REGISTER_MNEMONIC_COUNT][5] = 
	{ "111", "000", "001", "010", "011", "100", "101", "110" };

static char REGISTER_PAIR_MNEMONICS[REGISTER_PAIR_MNEMONIC_COUNT][5] = 
	{ "B", "D", "H", "SP", "PSW" };

static char REGISTER_PAIR_CODES[REGISTER_PAIR_MNEMONIC_COUNT][5] =
	{ "00", "01", "10", "11", "11" };


// Common functions
class CCodeGenHelper
{
public:
	CCodeGenHelper(void);
	
	static bool FileExists(char *fileName);
	static unsigned long GetOpcodeHash(std::string opcode);
	static int GetOpcodeHashIndex(unsigned long opcodeHash);
	static bool GetRegisterCode(std::string mnemonic, std::string result);
	static bool GetRegisterPairCode(std::string mnemonic, std::string result);

	~CCodeGenHelper(void);
};
