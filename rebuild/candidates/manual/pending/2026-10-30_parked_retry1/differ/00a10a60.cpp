// WordWrap_String::TokenizeW(wchar_t** tokens) -- split the wide text into word-wrap tokens at breakable spaces (tab, CR, space, ideographic space) and around East Asian characters (unless the next character may not begin a line or the previous may not end one): count them, or, given a token array, copy each token NUL-terminated into the reallocated token buffer and store its start
// Grade: DIFFER(281v281) (retry1) -- with the three character classes as same-TU `static __declspec(noinline)` bodies VC7.1 passes the character in ax like retail (the length now agrees); left: register colouring of the prologue (retail keeps `tokens` in ebp from entry and homes count in the dead argument slot; ours loads tokens into eax and m_text into ebp).
// OldGrade: DIFFER(301v281) -- lane gap: LTCG ax-argument callees (IsEastAsianChar / IsNonBeginningChar / IsNonEndingChar receive the character in ax with no push and no ecx/edx convention; VC7.1 cannot emit that from C++ outside /GL). Reference body only.
// Leaf: ?TokenizeW@WordWrap_String@@
// Flags: /Zc:wchar_t
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A10A60, 281 bytes (+ int3 padding), `ret 4`. Ego_r.pdb
//   ?TokenizeW@WordWrap_String@@QAEHPAPA_W@Z (native wchar_t). Callees: operator delete 0x00BFE9BC, wcslen 0x00BFED94,
//   scalar operator new 0x00BFEA1A, wcsncpy 0x00BFEB04 and the LTCG helpers IsEastAsianChar 0x00A10240,
//   IsNonBeginningChar 0x00A0FE80, IsNonEndingChar 0x00A10170 (character in ax).
// Layout (descriptive): text +0x00, extra token-buffer room +0x04, token buffer +0x08.
// Shape: retail starts the scan one BYTE past the text (`lea edi,[eax+1]`, then 2-byte steps), modelled literally; the
//   token count is homed in the dead argument slot.
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


class WordWrap_String {
public:
    const char* m_text;                 // +0x00 (read as wide text)
    long        m_extra;                // +0x04
    wchar_t*    m_buffer;               // +0x08
    int TokenizeW(wchar_t** tokens);
};

int WordWrap_String::TokenizeW(wchar_t** tokens)
{
    if (tokens) {
        if (m_buffer)
            operator delete(m_buffer);
        m_buffer = (wchar_t*)operator new((wcslen((const wchar_t*)m_text) + m_extra) * sizeof(wchar_t));
    }
    const wchar_t* start = (const wchar_t*)m_text;
    wchar_t* out = m_buffer;
    int count = 0;
    const wchar_t* p = (const wchar_t*)(m_text + 1);
    int len = (int)wcslen((const wchar_t*)m_text);
    if (len <= 0)
        return count;
    for (int n = len; n; --n, ++p) {
        if ((p && (*p == 9 || *p == 13 || *p == 0x20 || *p == 0x3000 || IsEastAsianChar(*p) || IsEastAsianChar(p[-1])) &&
             !IsNonBeginningChar(*p) && !IsNonEndingChar(p[-1])) || *p == 0) {
            if (tokens) {
                int length = (int)(p - start);
                *tokens = out;
                wcsncpy(out, start, length);
                (*tokens)[length] = 0;
                ++tokens;
                out += length + 1;
            } else
                ++count;
            start = p;
        }
    }
    return count;
}
