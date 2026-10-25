// IsNonBeginningChar(wchar_t ch) -- word-wrap kinsoku test: true when line-start prohibition is enabled (flag bit 0 of 0x013CE760) and ch is one of the 80 punctuation / small-kana characters that may not begin a line
// Grade: DIFFER(814v737) -- lane gap: custom register convention (LTCG-style): retail takes ch in ax (`cmp ax,imm16` chain; callers `mov eax,ecx ; call` and `mov ax,[esi-2] ; call` in WordWrap_FindNextLineW 0x00A10390) and returns in al/eax with a bare `ret`; VC7.1 without /GL passes the __fastcall wchar_t in cx (ours compares cx, 814 bytes)
// Leaf: IsNonBeginningChar
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0FE80, 739 bytes. Only callers: the word-wrap line finder 0x00A10390 (two
//   sites). Name: the chunk label (descriptive; the IME/word-wrap helper set at 0x00A0FE80..0x00A10800 is file-static).
// Globals: the word-wrap flags 0x013CE760 (bit 0 enables the test).
// Shape: one `||` chain in retail's comparison order (`cmp ax,imm ; je` to a shared `mov eax,1 ; ret`, fall-through
//   `xor eax,eax ; ret`); the disabled case is a separate `xor al,al ; ret`.
typedef unsigned short wchar_t;                     // VC7.1 default /Zc:wchar_t-

extern unsigned long g_dwWordWrapFlags;             // 0x013CE760

bool __fastcall IsNonBeginningChar(wchar_t ch)
{
    if (!(g_dwWordWrapFlags & 1))
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
