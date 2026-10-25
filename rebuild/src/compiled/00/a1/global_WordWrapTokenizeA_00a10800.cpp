// WordWrap_TokenizeA(const char* text, char** tokens) -- split multibyte text into word-wrap tokens: convert it to a wide stack copy, cut wherever WordWrap_CanBreakAt allows a break (and at the end), and either store each token as a new multibyte string in tokens[] or, with no array, count them
// Leaf: ?WordWrap_TokenizeA@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A10800, 338 bytes, __fastcall (ecx text, edx tokens), plain `ret`, ebp frame
//   (the _alloca). No direct callers (reached through a pointer). Name: the chunk label (descriptive; the module is the
//   word-wrap helper set at 0x00A0FE80..0x00A10800).
// Globals: the code page 0x01397098.
// Callees: MultiByteToWideChar (IAT 0x0143FE74), WideCharToMultiByte (IAT 0x0143FD98), _alloca probe 0x00BFEA30, operator new
//   0x00BFEA1A, operator delete 0x00BFE9BC, wcslen 0x00BFED94, WordWrap_CanBreakAt @ 0x00A102B0 (__fastcall p, start).
// Shape: the wide copy comes from _alloca with an operator-new fallback (and a bool to free it); the token loop is a
//   do / while over wcslen characters starting one character in; the count is only kept when no array is given. The
//   loop counter is a second local copied from the length INSIDE the `> 0` test: counting down the length itself stores
//   it before the `jle` (retail stores it after).
typedef unsigned short wchar_t;                     // VC7.1 default /Zc:wchar_t-

extern "C" __declspec(dllimport) int __stdcall MultiByteToWideChar(unsigned int codePage, unsigned long flags,
                                                                   const char* multiByte, int multiByteCount,
                                                                   wchar_t* wide, int wideCount);   // IAT 0x0143FE74
extern "C" __declspec(dllimport) int __stdcall WideCharToMultiByte(unsigned int codePage, unsigned long flags,
                                                                   const wchar_t* wide, int wideCount, char* multiByte,
                                                                   int multiByteCount, const char* defaultChar,
                                                                   int* usedDefaultChar);           // IAT 0x0143FD98
extern "C" void* __cdecl _alloca(unsigned int size);
#pragma intrinsic(_alloca)
extern "C" unsigned int __cdecl wcslen(const wchar_t* text);                                         // 0x00BFED94
void* __cdecl operator new(unsigned int size);                                                      // 0x00BFEA1A
void __cdecl operator delete(void* block);                                                          // 0x00BFE9BC

extern unsigned int g_uWordWrapCodePage;            // 0x01397098

bool __fastcall WordWrap_CanBreakAt(const wchar_t* p, const wchar_t* start);   // 0x00A102B0

long __fastcall WordWrap_TokenizeA(const char* text, char** tokens)
{
    if (text == 0)
        return 0;
    long count = 0;
    int length = MultiByteToWideChar(g_uWordWrapCodePage, 0, text, -1, 0, 0);
    int bytes = length * (int)sizeof(wchar_t);
    wchar_t* wide = (wchar_t*)_alloca(bytes);
    bool onHeap = false;
    if (wide == 0) {
        onHeap = true;
        wide = (wchar_t*)operator new(bytes);
    }
    if (MultiByteToWideChar(g_uWordWrapCodePage, 0, text, -1, wide, length) != 0) {
        const wchar_t* p = wide + 1;
        const wchar_t* start = wide;
        int wideLength = (int)wcslen(wide);
        if (wideLength > 0) {
            int remaining = wideLength;
            do {
                if (WordWrap_CanBreakAt(p, 0) || *p == 0) {
                    if (tokens != 0) {
                        int tokenLength = (int)(p - start);
                        int size = WideCharToMultiByte(g_uWordWrapCodePage, 0, start, tokenLength, 0, 0, 0, 0);
                        *tokens = (char*)operator new(size + 1);
                        WideCharToMultiByte(g_uWordWrapCodePage, 0, start, tokenLength, *tokens, size, 0, 0);
                        (*tokens)[size] = 0;
                        ++tokens;
                    } else
                        ++count;
                    start = p;
                }
                ++p;
            } while (--remaining != 0);
        }
    }
    if (onHeap)
        operator delete(wide);
    return count;
}