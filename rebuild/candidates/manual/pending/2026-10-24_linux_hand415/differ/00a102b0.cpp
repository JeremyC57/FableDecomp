// WordWrap_CanBreakLineAt(const unsigned short* text, const unsigned short* start) -- whether a line may break before `text`
// Grade: DIFFER(118v92) -- LTCG custom convention: retail passes each char to the helpers in ax (`mov eax,edx ; call` / `mov ax,[ecx-2] ; call`) and keeps ecx/edx live across the calls; ours pushes the char, saves registers and pops the arguments
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A102B0, length 92 bytes (+ int3 padding).
// Callees: IsEastAsianChar 0x00A10240 (twice), IsNonBeginningChar 0x00A0FE80, IsNonEndingChar 0x00A10170 (all ax-convention).
// Shape: `__fastcall` (text in ecx, start in edx, plain `ret`); `text == start` early out (`xor al,al`); the
// white-space macro on the loaded char (dx); the tail returns `mov eax,1` / `xor eax,eax`.
// Lane gap: retail was linked with /GL + /LTCG, and the linker gave these file-static word-wrap helpers a custom
// register convention: the character arrives in ax (callers `mov eax,edx` / `mov ax,[ecx-2]` then `call`), the
// callees read ax directly and the callers keep ecx/edx live across the calls (the linker knows the callees leave
// them alone). A plain .obj can only show the default convention (argument on the stack or in ecx), so the grader,
// which reads .obj text, cannot match it (GOTCHAS: the lane needs a link-and-extract mode for /GL + /LTCG).
// Leaf: WordWrap_CanBreakLineAt
#define WORDWRAP_IS_WHITE_SPACE(c) ((c) == 0x0009 || (c) == 0x000D || (c) == 0x0020 || (c) == 0x3000)
bool IsEastAsianChar(unsigned short c);       // 0x00A10240
bool IsNonBeginningChar(unsigned short c);    // 0x00A0FE80
bool IsNonEndingChar(unsigned short c);       // 0x00A10170

bool __fastcall WordWrap_CanBreakLineAt(const unsigned short* text, const unsigned short* start)
{
    if (text == start)
        return false;
    if (!WORDWRAP_IS_WHITE_SPACE(*text) && !IsEastAsianChar(*text) && !IsEastAsianChar(text[-1]))
        return false;
    return !IsNonBeginningChar(*text) && !IsNonEndingChar(text[-1]);
}
