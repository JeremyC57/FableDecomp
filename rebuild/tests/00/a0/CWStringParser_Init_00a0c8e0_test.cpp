// CWStringParser::Init(const wchar_t*) @ 0x00A0C8E0
// Leaf: ?Init@CWStringParser@@QAEXPBG@Z
#define FABLE_RETAIL_CWSTRINGPARSER_ICONTEXT_PASS30
#include "engine/CWStringParser.h"

void CWStringParser::Init(const wchar_t* text)
{
    String = text;
    StringPos = 0;
    Line = 0;
}
#include <assert.h>
#include <stdio.h>
#include <string.h>

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}

static void CheckParser(const CWStringParser& parser, const wchar_t* text)
{
    const unsigned char* memory = reinterpret_cast<const unsigned char*>(&parser);
    const wchar_t* actual;
    unsigned long position, line;
    memcpy(&actual, memory + 4, sizeof(actual));
    memcpy(&position, memory + 8, sizeof(position));
    memcpy(&line, memory + 12, sizeof(line));
    assert(actual == text && position == 0 && line == 0);
}

int main()
{
    const wchar_t first[] = L"first";
    const wchar_t second[] = L"second";
    CWStringParser parser(first);
    CheckParser(parser, first);
    unsigned char* memory = reinterpret_cast<unsigned char*>(&parser);
    unsigned char original_table[sizeof(void*)];
    memcpy(original_table, memory, sizeof(original_table));
    memset(memory + 8, 0x56, 8);
    parser.Init(0);
    CheckParser(parser, 0);
    memset(memory + 8, 0x7d, 8);
    parser.Init(second);
    CheckParser(parser, second);
    assert(memcmp(original_table, memory, sizeof(original_table)) == 0);
    puts("00a0c8e0_TEST PASS");
    return 0;
}
