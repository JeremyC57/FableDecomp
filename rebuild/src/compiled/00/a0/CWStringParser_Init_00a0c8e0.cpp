// CWStringParser::Init(const wchar_t*) @ 0x00A0C8E0
// Flags: /O2 /Oy
// Leaf: ?Init@CWStringParser@@QAEXPBG@Z
#define FABLE_RETAIL_CWSTRINGPARSER_ICONTEXT_PASS30
#include "engine/CWStringParser.h"

void CWStringParser::Init(const wchar_t* text)
{
    String = text;
    StringPos = 0;
    Line = 0;
}
