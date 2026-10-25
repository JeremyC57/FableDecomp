// WordWrap_String::TokenizeA(char** tokens) -- the narrow twin of TokenizeW: widen the text through the code page (stack buffer, heap fallback), split it at the same word-wrap breaks, and count the tokens or narrow each back into the reallocated token buffer, storing each NUL-terminated token's start
// Grade: DIFFER(476v450) -- lane gap: LTCG ax-argument callees (IsEastAsianChar / IsNonBeginningChar / IsNonEndingChar receive the character in ax with no push; VC7.1 cannot emit that from C++ outside /GL). Reference body only.
// Leaf: ?TokenizeA@WordWrap_String@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A10B80, 450 bytes (+ int3 padding), `ret 4`, ebp frame (alloca). Ego_r.pdb
//   ?TokenizeA@WordWrap_String@@QAEHPAPAD@Z. Callees: operator delete 0x00BFE9BC, scalar operator new 0x00BFEA1A,
//   _alloca_probe 0x00BFEA30, MultiByteToWideChar / WideCharToMultiByte through the IAT (0x0143FE74 / 0x0143FD98) with the
//   code page global 0x01397098, wcslen 0x00BFED94 and the LTCG helpers IsEastAsianChar 0x00A10240,
//   IsNonBeginningChar 0x00A0FE80, IsNonEndingChar 0x00A10170 (character in ax).
// Layout (descriptive): text +0x00, extra token-buffer room +0x04, token buffer +0x08.
// Shape: the ATL-style conversion buffer (_alloca of the doubled wide length rounded to 4, heap fallback flagged in a
//   frame byte and freed at the end); strlen is the inline intrinsic.
#include <stddef.h>
#include <string.h>
#include <malloc.h>
extern "C" size_t __cdecl wcslen(const wchar_t* s);                              // 0x00BFED94
void* __cdecl operator new(unsigned int size);                                   // 0x00BFEA1A
void  __cdecl operator delete(void* block);                                      // 0x00BFE9BC
extern "C" __declspec(dllimport) int __stdcall MultiByteToWideChar(unsigned int codePage, unsigned long flags,
    const char* text, int length, wchar_t* wide, int wideLength);                // IAT 0x0143FE74
extern "C" __declspec(dllimport) int __stdcall WideCharToMultiByte(unsigned int codePage, unsigned long flags,
    const wchar_t* wide, int wideLength, char* text, int length, const char* defaultChar, int* usedDefault);   // IAT 0x0143FD98
extern unsigned int g_WordWrapCodePage;                                          // 0x01397098
extern "C" bool IsEastAsianChar(wchar_t c);                                      // 0x00A10240 (LTCG: c in ax)
extern "C" bool IsNonBeginningChar(wchar_t c);                                   // 0x00A0FE80 (LTCG: c in ax)
extern "C" bool IsNonEndingChar(wchar_t c);                                      // 0x00A10170 (LTCG: c in ax)

class WordWrap_String {
public:
    const char* m_text;                 // +0x00
    long        m_extra;                // +0x04
    char*       m_buffer;               // +0x08
    int TokenizeA(char** tokens);
};

int WordWrap_String::TokenizeA(char** tokens)
{
    if (tokens) {
        if (m_buffer)
            operator delete(m_buffer);
        m_buffer = (char*)operator new(strlen(m_text) + m_extra);
    }
    int count = 0;
    int wideLength = MultiByteToWideChar(g_WordWrapCodePage, 0, m_text, -1, 0, 0);
    wchar_t* wide = (wchar_t*)_alloca(((wideLength * 2) + 3) & ~3);
    bool onHeap = false;
    if (!wide) {
        onHeap = true;
        wide = (wchar_t*)operator new(wideLength * 2);
    }
    if (MultiByteToWideChar(g_WordWrapCodePage, 0, m_text, -1, wide, wideLength)) {
        char* out = m_buffer;
        const wchar_t* p = wide + 1;
        const wchar_t* start = wide;
        int len = (int)wcslen(wide);
        for (int n = len; n > 0 && n; --n, ++p) {
            if ((p && (*p == 9 || *p == 13 || *p == 0x20 || *p == 0x3000 || IsEastAsianChar(*p) || IsEastAsianChar(p[-1])) &&
                 !IsNonBeginningChar(*p) && !IsNonEndingChar(p[-1])) || *p == 0) {
                if (tokens) {
                    int length = (int)(p - start);
                    int bytes = WideCharToMultiByte(g_WordWrapCodePage, 0, start, length, 0, 0, 0, 0);
                    *tokens = out;
                    WideCharToMultiByte(g_WordWrapCodePage, 0, start, length, out, bytes, 0, 0);
                    (*tokens)[bytes] = 0;
                    ++tokens;
                    out += bytes + 1;
                } else
                    ++count;
                start = p;
            }
        }
    }
    if (onHeap)
        operator delete(wide);
    return count;
}
