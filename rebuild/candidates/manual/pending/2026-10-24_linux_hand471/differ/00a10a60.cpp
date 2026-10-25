// WordWrap_String::TokenizeW(wchar_t** tokens) -- split the wide text into word-wrap tokens at breakable spaces (tab, CR, space, ideographic space) and around East Asian characters (unless the next character may not begin a line or the previous may not end one): count them, or, given a token array, copy each token NUL-terminated into the reallocated token buffer and store its start
// Grade: DIFFER(301v281) -- lane gap: LTCG ax-argument callees (IsEastAsianChar / IsNonBeginningChar / IsNonEndingChar receive the character in ax with no push and no ecx/edx convention; VC7.1 cannot emit that from C++ outside /GL). Reference body only.
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
extern "C" bool IsEastAsianChar(wchar_t c);                                      // 0x00A10240 (LTCG: c in ax)
extern "C" bool IsNonBeginningChar(wchar_t c);                                   // 0x00A0FE80 (LTCG: c in ax)
extern "C" bool IsNonEndingChar(wchar_t c);                                      // 0x00A10170 (LTCG: c in ax)

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
