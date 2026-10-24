// WordWrap_FindNonWhiteSpaceBackward(const unsigned short* text, const unsigned short* start) -- step back over white space and line feeds while inside the text; return the position, or null once it passes the start
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A10350, length 53 bytes (+ int3 padding). No callees.
// Module: the DirectX-sample word-wrap code (WordWrap_SetOption 0x00A0FDB0 .. WordWrap_TokenizeA 0x00A10800); called by
// WordWrap_FindNextLineW/A (0x00A103E8, 0x00A10416). WCHAR is `unsigned short` (no built-in wchar_t).
// Shape: `__fastcall` (text in ecx, start in edx, plain `ret`); the loop is VC7.1's rotated form of a top-tested
// `while (text >= start && (white space || line feed))` (entry `cmp ; jb` to the exit test, bottom `cmp ; jae`);
// the five character compares are the sample's white-space macro plus the line feed; `text >= start ? text : 0`
// is the branchless `cmp ; sbb ; not ; and`.
// Leaf: ?WordWrap_FindNonWhiteSpaceBackward@@YIPBGPBG0@Z
#define WORDWRAP_IS_WHITE_SPACE(c) ((c) == 0x0009 || (c) == 0x000D || (c) == 0x0020 || (c) == 0x3000)

const unsigned short* __fastcall WordWrap_FindNonWhiteSpaceBackward(const unsigned short* text, const unsigned short* start)
{
    while (text >= start && (WORDWRAP_IS_WHITE_SPACE(*text) || *text == 0x000A))
        --text;
    return text >= start ? text : 0;
}