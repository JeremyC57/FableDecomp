// WordWrap_TokenizeW(const unsigned short* text, unsigned short** tokens) -- split text at the line-break opportunities: copy each token into a new buffer, or count them
// Grade: DIFFER(149v222) -- LTCG: retail inlines WordWrap_CanBreakLineAt (cross-module inlining) and calls the helpers with the char in ax; ours calls 0x00A102B0 and the helpers through the stack
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A10720, length 222 bytes (+ int3 padding).
// Callees: MSVCR71 wcslen @ 0x00BFED94, operator new @ 0x00BFEA1A (scalar, the array new binds to it), wcsncpy @ 0x00BFEB04,
// IsEastAsianChar 0x00A10240, IsNonBeginningChar 0x00A0FE80, IsNonEndingChar 0x00A10170 (inlined CanBreakLineAt).
// Shape: `__fastcall` (text in ecx, token array in edx -> ebx, plain `ret`); the count lives at [esp+10h], the
// remaining length at [esp+14h]; CanBreakLineAt(cursor, 0) is inlined (`test edi,edi`); a token is cut at a break
// opportunity or at the terminator; the count is returned (only incremented when tokens is null).
// Lane gap: retail was linked with /GL + /LTCG, and the linker gave these file-static word-wrap helpers a custom
// register convention: the character arrives in ax (callers `mov eax,edx` / `mov ax,[ecx-2]` then `call`), the
// callees read ax directly and the callers keep ecx/edx live across the calls (the linker knows the callees leave
// them alone). A plain .obj can only show the default convention (argument on the stack or in ecx), so the grader,
// which reads .obj text, cannot match it (GOTCHAS: the lane needs a link-and-extract mode for /GL + /LTCG).
// Leaf: WordWrap_TokenizeW
typedef unsigned int size_t;
extern "C" size_t __cdecl wcslen(const unsigned short* text);                                          // 0x00BFED94
extern "C" unsigned short* __cdecl wcsncpy(unsigned short* dest, const unsigned short* src, size_t n);  // 0x00BFEB04
#define WORDWRAP_IS_WHITE_SPACE(c) ((c) == 0x0009 || (c) == 0x000D || (c) == 0x0020 || (c) == 0x3000)
bool IsEastAsianChar(unsigned short c);       // 0x00A10240
bool IsNonBeginningChar(unsigned short c);    // 0x00A0FE80
bool IsNonEndingChar(unsigned short c);       // 0x00A10170

inline bool WordWrap_CanBreakLineAt(const unsigned short* text, const unsigned short* start)   // 0x00A102B0, inlined here
{
    if (text == start)
        return false;
    if (!WORDWRAP_IS_WHITE_SPACE(*text) && !IsEastAsianChar(*text) && !IsEastAsianChar(text[-1]))
        return false;
    return !IsNonBeginningChar(*text) && !IsNonEndingChar(text[-1]);
}

long __fastcall WordWrap_TokenizeW(const unsigned short* text, unsigned short** tokens)
{
    if (text == 0)
        return 0;
    long count = 0;
    const unsigned short* tokenStart = text;
    const unsigned short* cursor = text + 1;
    for (long remaining = wcslen(text); remaining > 0; --remaining, ++cursor) {
        if (WordWrap_CanBreakLineAt(cursor, 0) || *cursor == 0) {
            if (tokens) {
                long length = cursor - tokenStart;
                *tokens = new unsigned short[length + 1];
                wcsncpy(*tokens, tokenStart, length);
                (*tokens)[length] = 0;
                ++tokens;
            }
            else
                ++count;
            tokenStart = cursor;
        }
    }
    return count;
}
