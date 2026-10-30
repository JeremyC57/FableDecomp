// WordWrap_CanBreakLineAt(const unsigned short* text, const unsigned short* start) -- whether a line may break before `text`
// Shape (retry1): parked as an "LTCG custom convention (char in ax)" gap; the word-wrap module's character classes are file statics, so the TU holds IsNonBeginningChar / IsNonEndingChar / IsEastAsianChar as `static __declspec(noinline)` bodies (the Ego_r PDB lists them as local procs) with their caller WordWrap_CanBreakLineAt, and VC7.1 /O2 passes the character in ax itself. The early `text == start` exit is its own return (`xor al,al`); the rest is one &&/|| expression (shared `mov eax,1` / `xor eax,eax` tail).
// Leaf: ?WordWrap_CanBreakLineAt@@YI_NPBG0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A102B0, length 92 bytes (+ int3 padding).
// Callees: IsEastAsianChar 0x00A10240 (twice), IsNonBeginningChar 0x00A0FE80, IsNonEndingChar 0x00A10170 (all ax-convention).
// Shape: `__fastcall` (text in ecx, start in edx, plain `ret`); `text == start` early out (`xor al,al`); the
// white-space macro on the loaded char (dx); the tail returns `mov eax,1` / `xor eax,eax`.
// Convention: the character arrives in ax (callers `mov eax,edx` / `mov ax,[ecx-2]` then `call`) -- VC7.1's own private
//   convention for these same-TU statics; the callers keep ecx/edx live across the calls.
extern unsigned long g_WordWrapOptions;             // 0x013CE760
#define WORDWRAP_IS_WHITE_SPACE(c) ((c) == 0x0009 || (c) == 0x000D || (c) == 0x0020 || (c) == 0x3000)

static __declspec(noinline) bool IsNonBeginningChar(unsigned short ch)   // 0x00A0FE80
{
    if (!(g_WordWrapOptions & 1))
        return false;
    return
        ch == 0xFF0C || ch == 0xFF0E || ch == 0xFF1A || ch == 0xFE30 || ch == 0xFE50 || ch == 0x00B7 || ch == 0xFE56 || ch == 0xFF5D ||
        ch == 0x300F || ch == 0xFE5C || ch == 0x201D || ch == 0x003A || ch == 0x005D || ch == 0x2022 || ch == 0x2027 || ch == 0x2026 ||
        ch == 0xFE51 || ch == 0xFE54 || ch == 0xFE57 || ch == 0x3011 || ch == 0x300D || ch == 0xFE5E || ch == 0x301E || ch == 0x003B ||
        ch == 0x007D || ch == 0xFF1B || ch == 0xFF01 || ch == 0xFE52 || ch == 0xFE55 || ch == 0x3015 || ch == 0x3009 || ch == 0xFE5A ||
        ch == 0x2019 || ch == 0x2032 || ch == 0x0025 || ch == 0x00B0 || ch == 0x2033 || ch == 0x2103 || ch == 0x300B || ch == 0xFF05 ||
        ch == 0xFF3D || ch == 0xFFE0 || ch == 0x2013 || ch == 0x2014 || ch == 0xFF61 || ch == 0xFF64 || ch == 0x3063 || ch == 0x3083 ||
        ch == 0x3085 || ch == 0x3087 || ch == 0x30C3 || ch == 0x30E3 || ch == 0x30E5 || ch == 0x30E7 || ch == 0x3041 || ch == 0x3043 ||
        ch == 0x3045 || ch == 0x3047 || ch == 0x3049 || ch == 0x308E || ch == 0x30A1 || ch == 0x30A3 || ch == 0x30A5 || ch == 0x30A7 ||
        ch == 0x30A9 || ch == 0x30EE || ch == 0x0022 || ch == 0x0021 || ch == 0x0029 || ch == 0x002C || ch == 0x002E || ch == 0x003F ||
        ch == 0x3001 || ch == 0x3002 || ch == 0x30FC || ch == 0xFF09 || ch == 0xFF1F || ch == 0xFF70 || ch == 0xFF9E || ch == 0xFF9F;
}

static __declspec(noinline) bool IsNonEndingChar(unsigned short c)        // 0x00A10170
{
    if (!(g_WordWrapOptions & 1))
        return false;
    return c == 0x3010 || c == 0x300C || c == 0xFE5D || c == 0x301D || c == 0x005B || c == 0x3014 || c == 0x3008
        || c == 0xFE59 || c == 0x2018 || c == 0x2035 || c == 0x007B || c == 0xFF5B || c == 0x300E || c == 0xFE5B
        || c == 0x201C || c == 0x005C || c == 0x0024 || c == 0xFFE1 || c == 0x300A || c == 0xFF04 || c == 0xFF3B
        || c == 0xFFE6 || c == 0xFFE5 || c == 0x0022 || c == 0x0028 || c == 0xFF08;
}

static __declspec(noinline) bool IsEastAsianChar(unsigned short c)        // 0x00A10240
{
    if (g_WordWrapOptions & 4) {
        if ((c >= 0x1100 && c <= 0x11FF) || (c >= 0x3130 && c <= 0x318F) || (c >= 0xAC00 && c <= 0xD7A3))
            return false;
    }
    return (c >= 0x1100 && c <= 0x11FF) || (c >= 0x3000 && c <= 0xD7AF) || (c >= 0xF900 && c <= 0xFAFF) || (c >= 0xFF00 && c <= 0xFFDC);
}

bool __fastcall WordWrap_CanBreakLineAt(const unsigned short* text, const unsigned short* start)
{
    if (text == start)
        return false;
    return (WORDWRAP_IS_WHITE_SPACE(*text) || IsEastAsianChar(*text) || IsEastAsianChar(text[-1]))
        && !IsNonBeginningChar(*text) && !IsNonEndingChar(text[-1]);
}