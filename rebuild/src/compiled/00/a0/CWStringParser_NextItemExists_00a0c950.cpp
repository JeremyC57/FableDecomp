// CWStringParser::NextItemExists() -- true when a non-whitespace character follows the cursor before the terminator (line feeds on the way are counted); the cursor itself does not move
// Leaf: ?NextItemExists@CWStringParser@@QAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0C950, 70 bytes, thiscall, `ret`.
//   PDB name ?NextItemExists@CWStringParser@@QAE_NXZ.
// Callees: iswspace 0x00BFEB90 (MSVCR71 import thunk, called directly).
// Layout: Ego_r.pdb CWStringParser (16 bytes): vptr, String +0x04, StringPos +0x08, Line +0x0c.
// Shape: a local index walked from StringPos (strength-reduced to a byte offset in edi, `lea edi,[eax+eax]`), String
//   re-read for every step after the call; the top-tested `while (c != 0)` is rotated with the test duplicated at the
//   entry, the two exits keep their own `pop edi ; xor al,al` / `pop edi ; mov al,1` tails.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t
extern "C" int __cdecl iswspace(wchar_t c);   // 0x00BFEB90 (CRT, wint_t argument)

class CWStringParser {
public:
    virtual ~CWStringParser();
    wchar_t*      String;                   // +0x04
    unsigned long StringPos;                // +0x08
    unsigned long Line;                     // +0x0c
    bool NextItemExists();
};

bool CWStringParser::NextItemExists()
{
    unsigned long pos = StringPos;
    wchar_t c = String[pos];
    while (c != 0)
    {
        if (c == L'\n')
            ++Line;
        if (!iswspace(c))
            return true;
        c = String[++pos];
    }
    return false;
}