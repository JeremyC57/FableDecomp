// GFStringToInt(const wchar_t*) -- wide-string overload: parse a decimal integer, every '-' anywhere marks the result negative, every other character is taken as a digit
// Grade: DIFFER(75v75) -- one branch target: retail's zero-length guard `jbe` lands on the `cmp bl,1` sign test, ours threads it past the test to `mov eax,edi` (same gap as 0x009E1960)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E1A80, length 75 bytes (+ int3 padding).
// PDB (FableWin): ?GFStringToInt@@YIJPB_W@Z -- __fastcall, string in ecx, long result. The era build has no native
// wchar_t (`_W` in the newer PDB; VC7.1 without /Zc:wchar_t sees `unsigned short`), so the model uses the typedef.
// Callee: MSVCR71 wcslen (0x00BFED94, `__cdecl`, `add esp,4` after the call because its result is used).
// Shape: the current character is read before the loop and re-read at the bottom (`mov dx,[esi+ecx*2+2] ; inc ecx`),
// the explicit hoisted form; `cmp bl,1 ; jne` is the explicit `negative == true` test.
// Tried: this source under RTM and under QFE-4035 (same single target diff), the plain top-read `for` under QFE
// (no read rotation), an explicit `if (i < length) do {...} while` guard (same target).
typedef unsigned short wchar_t;
extern "C" unsigned int __cdecl wcslen(const wchar_t* text);   // 0x00BFED94

long __fastcall GFStringToInt(const wchar_t* text)
{
    long result = 0;
    bool negative = false;
    unsigned long length = wcslen(text);
    unsigned long i = 0;
    wchar_t c = *text;
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
