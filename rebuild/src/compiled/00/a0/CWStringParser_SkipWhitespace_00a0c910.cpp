// CWStringParser::SkipWhitespace() -- step the cursor over whitespace (counting line feeds) and return how many characters were skipped
// Leaf: ?SkipWhitespace@CWStringParser@@QAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0C910, 61 bytes (+3 int3 padding), thiscall, `ret`.
//   PDB name ?SkipWhitespace@CWStringParser@@QAEJXZ (long result).
// Callees: iswspace 0x00BFEB90 (MSVCR71 import thunk, called directly).
// Layout: Ego_r.pdb CWStringParser (16 bytes): vptr, String +0x04, StringPos +0x08, Line +0x0c (the landed
//   ReadNextItem 0x00A0CFC0 / ReadAsStringUntilNonWhitespace 0x00A0CC50 model).
// Shape: the ReadAsStringUntilNonWhitespace loop without the terminator test (iswspace(0) is false): the count lives in
//   edi, each step re-reads String[StringPos + ++count] through both members, and StringPos advances once at the end.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t
extern "C" int __cdecl iswspace(wchar_t c);   // 0x00BFEB90 (CRT, wint_t argument)

class CWStringParser {
public:
    virtual ~CWStringParser();
    wchar_t*      String;                   // +0x04
    unsigned long StringPos;                // +0x08
    unsigned long Line;                     // +0x0c
    long SkipWhitespace();
};

long CWStringParser::SkipWhitespace()
{
    long count = 0;
    wchar_t c = String[StringPos];
    for (;;)
    {
        if (c == L'\n')
            ++Line;
        if (!iswspace(c))
            break;
        c = String[StringPos + ++count];
    }
    StringPos += count;
    return count;
}