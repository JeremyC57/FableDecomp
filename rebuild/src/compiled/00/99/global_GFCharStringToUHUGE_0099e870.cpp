// GFCharStringToUHUGE(const CCharString& text) -- parse an unsigned 64-bit decimal: '-' is ignored, '.' ends it, other non-digits are skipped
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099E870, 140 bytes. FableWin.pdb ?GFCharStringToUHUGE@@YI_KABVCCharString@@@Z:
// a free `__fastcall` (text in ecx) returning unsigned __int64 in edx:eax, plain `ret`. Manifest label "ParseLongLong"
// is descriptive.
// Callees: isdigit @ 0x00BFEDA6 (MSVCR71 import), __allmul @ 0x00BFEB50. Literal "" @ 0x0129AAF4.
// Shape: the GFCharStringToInt (0x0099E7F0) fetch-ahead loop with an unsigned index; value = value * 10 + (c - '0')
// goes through __allmul with the sign-extended (`cdq`) digit added by add/adc; &text is spilled to the frame (all
// four callee-saved registers hold c, i and the 64-bit value).
// Layout (landed CCharString model): CCharStringData = CBasicString<char> {PData +0, StringLength +4, ...}.
// Leaf: ?GFCharStringToUHUGE@@
extern "C" int __cdecl isdigit(int c);                  // 0x00BFEDA6

struct CBasicStringChar {
    char*         PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
};
class CCharStringData { public:
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

unsigned __int64 __fastcall GFCharStringToUHUGE(const CCharString& text)
{
    unsigned __int64 value = 0;
    unsigned long i = 0;
    char c = text.GetChar(i);
    while (i < (unsigned long)text.GetLength()) {
        if (c == '-')
            ;                                           // an unsigned value has no sign
        else if (c == '.')
            break;
        else if (isdigit((unsigned char)c))
            value = value * 10 + (c - '0');
        c = text.GetChar(++i);
    }
    return value;
}