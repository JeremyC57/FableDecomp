// IsEastAsianChar(unsigned short c) -- whether a character is East Asian (breakable anywhere); with the Korean option on, Hangul (jamo, compatibility jamo, syllables) is not
// Grade: DIFFER(110v105) -- LTCG custom convention: retail compares the char already in ax; ours adds the one stack load `mov ax,[esp+4]` (+5 bytes) and every other instruction matches (36 of 36 mnemonics)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A10240, length 105 bytes (+ int3 padding). No callees.
// Global: 0x013CE760 the word-wrap option bits (bit 2 = Korean: Hangul breaks only at spaces).
// Callers: WordWrap_CanBreakLineAt 0x00A102B0 (0x00A102D5, 0x00A102E2) and its inlined copies in the tokenizers.
// Ranges: Hangul jamo 1100-11FF, compatibility jamo 3130-318F, syllables AC00-D7A3 (excluded under bit 2);
// East Asian 1100-11FF, 3000-D7AF, F900-FAFF, FF00-FFDC.
// Lane gap: retail was linked with /GL + /LTCG, and the linker gave these file-static word-wrap helpers a custom
// register convention: the character arrives in ax (callers `mov eax,edx` / `mov ax,[ecx-2]` then `call`), the
// callees read ax directly and the callers keep ecx/edx live across the calls (the linker knows the callees leave
// them alone). A plain .obj can only show the default convention (argument on the stack or in ecx), so the grader,
// which reads .obj text, cannot match it (GOTCHAS: the lane needs a link-and-extract mode for /GL + /LTCG).
// Leaf: IsEastAsianChar
extern unsigned long g_WordWrapOptions;   // 0x013CE760

bool IsEastAsianChar(unsigned short c)
{
    if (g_WordWrapOptions & 4) {
        if ((c >= 0x1100 && c <= 0x11FF) || (c >= 0x3130 && c <= 0x318F) || (c >= 0xAC00 && c <= 0xD7A3))
            return false;
    }
    return (c >= 0x1100 && c <= 0x11FF) || (c >= 0x3000 && c <= 0xD7AF) || (c >= 0xF900 && c <= 0xFAFF) || (c >= 0xFF00 && c <= 0xFFDC);
}
