// WordWrap_String::TokenizeA(char** tokens) -- the narrow twin of TokenizeW: widen the text through the code page (stack buffer, heap fallback), split it at the same word-wrap breaks, and count the tokens or narrow each back into the reallocated token buffer, storing each NUL-terminated token's start
// Grade: DIFFER(458v450) (retry1) -- with the three character classes as same-TU `static __declspec(noinline)` bodies the helper calls pass the character in ax like retail (was 476v450); left: register colouring (retail this in ebx, the wide length in esi; ours this in esi, length in ebx, which also folds the alloca rounding into one lea).
// OldGrade: DIFFER(476v450) -- lane gap: LTCG ax-argument callees (IsEastAsianChar / IsNonBeginningChar / IsNonEndingChar receive the character in ax with no push; VC7.1 cannot emit that from C++ outside /GL). Reference body only.
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
