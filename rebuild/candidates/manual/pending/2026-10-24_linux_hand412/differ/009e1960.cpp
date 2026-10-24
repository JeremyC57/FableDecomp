// GFStringToInt(const char*) -- parse a decimal integer: every '-' anywhere marks the result negative, every other character is taken as a digit
// Grade: DIFFER(74v74) -- one branch target: retail's zero-length guard `je` lands on the `cmp al,1` sign test, ours threads it past the test to `mov eax,edi` (negative is known false on that edge)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E1960, length 74 bytes (+ int3 padding).
// PDB (FableWin): ?GFStringToInt@@YIJPBD@Z -- __fastcall, string in ecx, long result.
// Shape: strlen is the /Oi intrinsic (the `mov bl,[edx] ; inc edx ; test bl,bl ; jne` scan, length = end - (start+1));
// the current character is read BEFORE the loop and re-read at the bottom (`mov bl,[esi+ecx+1] ; inc esi`), which
// only the explicit hoisted form reproduces (`c = *(++i + text)` gives retail's base/index order; `text[i + 1]`
// gives `[ecx+esi+1]`, a plain `text[i]` read at the top is not rotated by the lane); `x * 10 + c - '0'` is the
// two-`lea` multiply-add; `cmp al,1 ; jne` is the explicit `negative == true` test.
// Tried: plain top-read for loop (70 bytes, no rotation), hoisted `text[i + 1]`, hoisted `*(++i + text)` (this
// source, one target differs), the `negative == true ? -result : result` ternary (same target).
#include <string.h>

long __fastcall GFStringToInt(const char* text)
{
    long result = 0;
    bool negative = false;
    unsigned long length = strlen(text);
    unsigned long i = 0;
    char c = *text;
    while (i < length)
    {
        if (c == '-')
            negative = true;
        else
            result = result * 10 + c - '0';
        c = *(++i + text);
    }
    if (negative == true)
        result = -result;
    return result;
}
