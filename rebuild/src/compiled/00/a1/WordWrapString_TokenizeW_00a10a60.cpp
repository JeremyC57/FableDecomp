// WordWrap_String::TokenizeW(wchar_t** tokens) -- split the wide text into word-wrap tokens at breakable positions (tab, CR, space, ideographic space, around East Asian characters unless the next may not begin or the previous may not end a line): count them, or, given a token array, copy each token NUL-terminated into the reallocated token buffer and store its start
// Leaf: ?TokenizeW@WordWrap_String@@QAEHPAPA_W@Z
// Flags: /Zc:wchar_t
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A10A60, 281 bytes (+ int3 padding), `ret 4`. Ego_r.pdb
//   ?TokenizeW@WordWrap_String@@QAEHPAPA_W@Z (native wchar_t). Callees: operator delete 0x00BFE9BC, wcslen 0x00BFED94,
//   scalar operator new 0x00BFEA1A, wcsncpy 0x00BFEB04 and the word-wrap character classes IsEastAsianChar 0x00A10240,
//   IsNonBeginningChar 0x00A0FE80, IsNonEndingChar 0x00A10170 (PDB C names; character in ax).
// Layout: Ego_r.pdb WordWrap_String { char* m_pszSource; int m_iTokens; char* m_pszFirstToken; } (no shared header).
// Shape (wave 25): the loop is the landed free-function sibling WordWrap_TokenizeW 0x00A10720 verbatim -- the break test is the
//   inlined PDB helper `bool __fastcall WordWrap_CanBreakLineAt(const wchar_t*, const wchar_t*)` called with a null start
//   (its `text == start` test is retail's `test edi,edi`), and `for (remaining = wcslen(text); remaining > 0; ...)` gives
//   retail's count-in-esi zero (`xor esi,esi`, returned by the skipped-loop exit), `tokens` in ebp and the count homed in the
//   dead argument slot. The three character classes are same-TU `static __declspec(noinline)` bodies, so VC7.1 passes the
//   character in ax as retail does; the scan starts one BYTE past the text (`lea edi,[eax+1]`), modelled literally.
#include <stddef.h>
extern "C" size_t __cdecl wcslen(const wchar_t* s);                              // 0x00BFED94
extern "C" wchar_t* __cdecl wcsncpy(wchar_t* d, const wchar_t* s, size_t n);     // 0x00BFEB04
void* __cdecl operator new(unsigned int size);                                   // 0x00BFEA1A
void  __cdecl operator delete(void* block);                                      // 0x00BFE9BC
extern unsigned long g_WordWrapOptions;                                          // 0x013CE760

static __declspec(noinline) bool IsNonBeginningChar(wchar_t ch)   // 0x00A0FE80
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

static __declspec(noinline) bool IsNonEndingChar(wchar_t c)        // 0x00A10170
{
    if (!(g_WordWrapOptions & 1))
        return false;
    return c == 0x3010 || c == 0x300C || c == 0xFE5D || c == 0x301D || c == 0x005B || c == 0x3014 || c == 0x3008
        || c == 0xFE59 || c == 0x2018 || c == 0x2035 || c == 0x007B || c == 0xFF5B || c == 0x300E || c == 0xFE5B
        || c == 0x201C || c == 0x005C || c == 0x0024 || c == 0xFFE1 || c == 0x300A || c == 0xFF04 || c == 0xFF3B
        || c == 0xFFE6 || c == 0xFFE5 || c == 0x0022 || c == 0x0028 || c == 0xFF08;
}

static __declspec(noinline) bool IsEastAsianChar(wchar_t c)        // 0x00A10240
{
    if (g_WordWrapOptions & 4) {
        if ((c >= 0x1100 && c <= 0x11FF) || (c >= 0x3130 && c <= 0x318F) || (c >= 0xAC00 && c <= 0xD7A3))
            return false;
    }
    return (c >= 0x1100 && c <= 0x11FF) || (c >= 0x3000 && c <= 0xD7AF) || (c >= 0xF900 && c <= 0xFAFF) || (c >= 0xFF00 && c <= 0xFFDC);
}


#define WORDWRAP_IS_WHITE_SPACE(c) ((c) == 0x0009 || (c) == 0x000D || (c) == 0x0020 || (c) == 0x3000)
inline bool __fastcall WordWrap_CanBreakLineAt(const wchar_t* text, const wchar_t* start)
{
    if (text == start)
        return false;
    return (WORDWRAP_IS_WHITE_SPACE(*text) || IsEastAsianChar(*text) || IsEastAsianChar(text[-1]))
        && !IsNonBeginningChar(*text) && !IsNonEndingChar(text[-1]);
}

class WordWrap_String {                 // Ego_r.pdb, 12 bytes
public:
    char* m_pszSource;                  // +0x00 (the TokenizeW path reads it as wide text)
    int   m_iTokens;                    // +0x04 extra token-buffer room
    char* m_pszFirstToken;              // +0x08 token buffer
    int TokenizeW(wchar_t** tokens);
};

int WordWrap_String::TokenizeW(wchar_t** tokens)
{
    if (tokens) {
        if (m_pszFirstToken)
            operator delete(m_pszFirstToken);
        m_pszFirstToken = (char*)operator new((wcslen((const wchar_t*)m_pszSource) + m_iTokens) * sizeof(wchar_t));
    }
    wchar_t* out = (wchar_t*)m_pszFirstToken;
    const wchar_t* cursor = (const wchar_t*)(m_pszSource + 1);
    const wchar_t* tokenStart = (const wchar_t*)m_pszSource;
    int count = 0;
    for (int remaining = (int)wcslen((const wchar_t*)m_pszSource); remaining > 0; --remaining, ++cursor) {
        if (WordWrap_CanBreakLineAt(cursor, 0) || *cursor == 0) {
            if (tokens) {
                int length = (int)(cursor - tokenStart);
                *tokens = out;
                wcsncpy(out, tokenStart, length);
                (*tokens)[length] = 0;
                ++tokens;
                out += length + 1;
            }
            else
                ++count;
            tokenStart = cursor;
        }
    }
    return count;
}