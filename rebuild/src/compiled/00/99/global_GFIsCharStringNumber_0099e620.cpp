// GFIsCharStringNumber(const CCharString& text) -- true when the text is a decimal number: digits, at most one '.', and '-' only in first place
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099E620, 104 bytes. FableWin.pdb ?GFIsCharStringNumber@@YI_NABVCCharString@@@Z:
// a free `__fastcall` (text in ecx), plain `ret`.
// Callees: isdigit @ 0x00BFEDA6 (MSVCR71 import). Literal "" @ 0x0129AAF4 (the empty-string fallback).
// Shape: a fetch-ahead loop -- the first character is read before the loop and the next one (`GetChar(++i)`) at the
// bottom, so the block pointer loaded there feeds the next length test (a `for` with the read at the top keeps the
// load in the loop head). The inline CCharString accessors: GetLength() (null block -> 0, signed long with a signed
// index, so the test is `jge`) and GetChar(i) through the null-safe text pointer; the three rejecting cases share one
// `return false`.
// Layout (landed CCharString model): CCharStringData = CBasicString<char> {PData +0, StringLength +4, ...}.
// Leaf: ?GFIsCharStringNumber@@
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

bool __fastcall GFIsCharStringNumber(const CCharString& text)
{
    bool seenPoint = false;
    long i = 0;
    char c = text.GetChar(i);
    while (i < text.GetLength()) {
        if (c == '.') {
            if (seenPoint)
                return false;
            seenPoint = true;
        } else if (c == '-') {
            if (i != 0)
                return false;
        } else if (!isdigit((unsigned char)c))
            return false;
        c = text.GetChar(++i);
    }
    return true;
}