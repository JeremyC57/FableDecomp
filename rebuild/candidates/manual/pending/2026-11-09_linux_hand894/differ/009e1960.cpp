// GFStringToInt(const char*) -- parse a decimal integer: every '-' anywhere marks the result negative, every other character is taken as a digit
// Grade: DIFFER(74v74) -- one branch target: retail's zero-length guard `je 0x9e199e` lands on the `cmp al,1` sign test, ours threads it to `mov eax,edi` (negative known false on that edge)
// Chunk 894 (13 grades, probe gf/a-k + pragma optimize("s") + /O1): return form, do-while guard, ternary, for loop, braces, declaration order, inline template helper, `*= -1`, separate parse helper: all thread or are worse; optimize("s") and /O1 change the whole shape. The landed GFStringToFloat siblings (0x009E18B0 / 0x009E19B0) carry no pragmas. Next: make `negative` not provably false on the guard edge (e.g. read it through a by-reference helper in the same TU).
// Wave-25 chunk 623 retry (tried, inert): strlen in the for condition (not hoisted: 2 strlen loops); char flag with `== 1` (same target); `long sign` multiply (imul shape)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E1960, length 74 bytes (+ int3 padding).
// Leaf: ?GFStringToInt@@YIJPBD@Z
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
