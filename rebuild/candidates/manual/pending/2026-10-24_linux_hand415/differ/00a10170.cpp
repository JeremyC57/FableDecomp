// IsNonEndingChar(unsigned short c) -- with the East Asian line-breaking option on, whether a character must not end a line (opening brackets, opening quotes, currency signs)
// Grade: DIFFER(202v197) -- LTCG custom convention: retail tests the char already in ax; ours adds the one stack load `mov ax,[esp+4]` (+5 bytes) and every other instruction matches (60 of 60 mnemonics)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A10170, length 197 bytes (+ int3 padding). No callees.
// Global: 0x013CE760 the word-wrap option bits (bit 0 = East Asian line breaking), set by WordWrap_SetOption 0x00A0FDB0.
// Callers: WordWrap_CanBreakLineAt 0x00A102B0 and its inlined copies in WordWrap_TokenizeW/A (0x00A1079D ...).
// Body: an option test then the `||` chain of 26 characters in retail's compare order (verified against the
// retail compare sequence); the result is `bool` (`xor al,al` early out, `mov al,1` / `xor al,al` tails).
// Lane gap: retail was linked with /GL + /LTCG, and the linker gave these file-static word-wrap helpers a custom
// register convention: the character arrives in ax (callers `mov eax,edx` / `mov ax,[ecx-2]` then `call`), the
// callees read ax directly and the callers keep ecx/edx live across the calls (the linker knows the callees leave
// them alone). A plain .obj can only show the default convention (argument on the stack or in ecx), so the grader,
// which reads .obj text, cannot match it (GOTCHAS: the lane needs a link-and-extract mode for /GL + /LTCG).
// Leaf: IsNonEndingChar
extern unsigned long g_WordWrapOptions;   // 0x013CE760

bool IsNonEndingChar(unsigned short c)
{
    if (!(g_WordWrapOptions & 1))
        return false;
    return c == 0x3010 || c == 0x300C || c == 0xFE5D || c == 0x301D || c == 0x005B || c == 0x3014 || c == 0x3008
        || c == 0xFE59 || c == 0x2018 || c == 0x2035 || c == 0x007B || c == 0xFF5B || c == 0x300E || c == 0xFE5B
        || c == 0x201C || c == 0x005C || c == 0x0024 || c == 0xFFE1 || c == 0x300A || c == 0xFF04 || c == 0xFF3B
        || c == 0xFFE6 || c == 0xFFE5 || c == 0x0022 || c == 0x0028 || c == 0xFF08;
}
