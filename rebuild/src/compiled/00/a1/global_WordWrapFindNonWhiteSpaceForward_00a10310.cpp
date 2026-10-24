// WordWrap_FindNonWhiteSpaceForward(const unsigned short* text) -- skip tabs, carriage returns, spaces and ideographic spaces, then one line feed; return the position, or null at the end of the text
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A10310, length 52 bytes (+ int3 padding). No callees.
// Module: the DirectX-sample word-wrap code (WordWrap_SetOption 0x00A0FDB0 .. WordWrap_TokenizeA 0x00A10800); called by
// WordWrap_FindNextLineW/A (0x00A103F6, 0x00A1053D). wchar_t is not a built-in type under the era flags: WCHAR is
// `unsigned short`.
// Shape: `__fastcall` (text in ecx, plain `ret`); the white-space test is the sample's four-way macro on the loaded
// character (`cmp ax,9/0Dh/20h/3000h`, a backward `jmp` loop); the line-feed test and the final null test re-read
// `*text` from memory (`cmp word ptr [ecx],0Ah`, `mov ax,[ecx]`), and `*text ? text : 0` is the branchless
// `neg ax ; sbb eax,eax ; and eax,ecx`.
// Leaf: ?WordWrap_FindNonWhiteSpaceForward@@YIPBGPBG@Z
#define WORDWRAP_IS_WHITE_SPACE(c) ((c) == 0x0009 || (c) == 0x000D || (c) == 0x0020 || (c) == 0x3000)

const unsigned short* __fastcall WordWrap_FindNonWhiteSpaceForward(const unsigned short* text)
{
    while (WORDWRAP_IS_WHITE_SPACE(*text))
        ++text;
    if (*text == 0x000A)
        ++text;
    return *text ? text : 0;
}