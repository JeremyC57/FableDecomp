// CWParsedItem::InitAsInteger(const wchar_t*, unsigned long) -- parse a decimal (optionally '-') integer item
// Grade: DIFFER(94v106) -- zero register shared by Integer and the bool (ours xor ebx / retail xor edx + xor bl,bl) and retail threads the empty-loop exit to its own Type=3 store; loop body matches
// Retail: Fable.exe @ 0x00A0CA20, 106 bytes (+int3 padding). No callees.
// PDB ?InitAsInteger@CWParsedItem@@QAEXPB_WK@Z. CWParsedItem (12 bytes, PDB): +0x00 EParsedItemType Type,
// +0x04 union (long Integer / float Float), +0x08 CWideString String.
// Shape: Integer is cleared first and accumulated in memory (Integer*10 + c - '0' via two lea's); every
// '-' anywhere in the run sets the negative flag; Type = 3 (integer) is stored on both exits; the
// flag test is `cmp bl,1` (bool compared with true).
#include <stddef.h>   // wchar_t is a typedef (unsigned short) under VC7.1 without /Zc:wchar_t

struct CWideStringData;
struct CWideString { CWideStringData* PStringData; };

struct CWParsedItem {
    long        Type;      // +0x00 EParsedItemType (3 = integer)
    long        Integer;   // +0x04 (union with the float value)
    CWideString String;    // +0x08
    void InitAsInteger(const wchar_t* text, unsigned long length);
};

void CWParsedItem::InitAsInteger(const wchar_t* text, unsigned long length)
{
    unsigned long i = 0;
    Integer = 0;
    bool negative = false;
    for (wchar_t c = text[0]; i < length; c = text[++i])
    {
        if (c == L'-')
            negative = true;
        else
            Integer = Integer * 10 + c - L'0';
    }
    Type = 3;
    if (negative == true)
        Integer = -Integer;
}
