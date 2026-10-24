// GFCharStringToInt(const CCharString& text) -- parse a decimal integer: '-' anywhere makes it negative, '.' ends it, other non-digits are skipped
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099E7F0, 122 bytes. FableWin.pdb ?GFCharStringToInt@@YIJABVCCharString@@@Z:
// a free `__fastcall` (text in ecx) returning long, plain `ret`.
// Callees: isdigit @ 0x00BFEDA6 (MSVCR71 import). Literal "" @ 0x0129AAF4 (the empty-string fallback).
// Shape: the GFIsCharStringNumber (0x0099E620) fetch-ahead loop with an UNSIGNED index (the length test is folded to
// `je` on a null block plus `jae` against StringLength); value = value * 10 + (c - '0') is the `lea` pair; the
// negative flag lives in a frame byte (ebx/esi/edi/ebp hold c, i, value, &text).
// Layout (landed CCharString model): CCharStringData = CBasicString<char> {PData +0, StringLength +4, ...}.
// Leaf: ?GFCharStringToInt@@
extern "C" int __cdecl isdigit(int c);                  // 0x00BFEDA6

struct CBasicStringChar {
    char*         PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
};
struct CCharStringData {
    CBasicStringChar Data;                              // +0x00
};

class CCharString
{
public:
    CCharStringData* PStringData;                       // +0x00
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->Data.PData; }
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->Data.StringLength; }
    char GetChar(long index) const { return GetString()[index]; }
};

long __fastcall GFCharStringToInt(const CCharString& text)
{
    long value = 0;
    bool negative = false;
    unsigned long i = 0;
    char c = text.GetChar(i);
    while (i < (unsigned long)text.GetLength()) {
        if (c == '-')
            negative = true;
        else if (c == '.')
            break;
        else if (isdigit((unsigned char)c))
            value = value * 10 + (c - '0');
        c = text.GetChar(++i);
    }
    if (negative)
        value = -value;
    return value;
}