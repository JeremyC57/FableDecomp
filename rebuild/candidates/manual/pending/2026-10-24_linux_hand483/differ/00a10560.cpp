// WordWrap_FindNextLineA(const char* text, unsigned int maxWidth, const char** lineEnd) -- multibyte front end of the word-wrap line finder: convert text to the shared wide buffer, mark characters the code page could not map with a placeholder (L'?' on 1252, U+4E9E elsewhere), run the wide line finder, then walk the multibyte text with CharNextExA to map the line end and the next line start back
// Grade: DIFFER(433v433) -- one instruction: retail tests the MultiByteToWideChar result with `cmp eax,esi` (esi = the zero register), ours emits `test eax,eax` (same length, everything else byte-identical after relocation)
// Leaf: WordWrap_FindNextLineA
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A10560, 431 bytes, __fastcall `ret 4` (ecx text, edx maxWidth, stack next).
//   No direct callers (reached through a pointer). Name: the chunk label (descriptive; the module is the word-wrap helper
//   set beside IsNonBeginningChar 0x00A0FE80 and the wide finder 0x00A10390).
// Globals: the character-width callback 0x013CE758 (null = word wrap disabled), code page 0x01397098, flags 0x013CE760,
//   0x013CE764, the code page's default wide char 0x013CE768 and its multibyte bytes 0x013CE76C..0x013CE76E, the wide
//   buffer 0x013CE770 (0x2000 characters, last one forced to 0).
// Callees: MultiByteToWideChar (IAT 0x0143FE74), GetLastError (IAT 0x0143FD58), CharNextExA (IAT 0x0144039C, cached in ebx),
//   WordWrap_FindNextLineW @ 0x00A10390 (__fastcall).
// Shape: the wide finder returns the next line's start and stores the line's last character through its out parameter;
//   when it returns 0 but the conversion was truncated (ERROR_INSUFFICIENT_BUFFER) the buffer's last character is taken as
//   the next start; *lineEnd is only written when the wide finder produced a line end.
typedef unsigned short wchar_t;                     // VC7.1 default /Zc:wchar_t-

extern "C" __declspec(dllimport) int __stdcall MultiByteToWideChar(unsigned int codePage, unsigned long flags,
                                                                   const char* multiByte, int multiByteCount,
                                                                   wchar_t* wide, int wideCount);   // IAT 0x0143FE74
extern "C" __declspec(dllimport) unsigned long __stdcall GetLastError();                            // IAT 0x0143FD58
extern "C" __declspec(dllimport) char* __stdcall CharNextExA(unsigned short codePage, const char* current,
                                                             unsigned long flags);                  // IAT 0x0144039C

extern int (__fastcall* g_pfnWordWrapGetCharWidth)(wchar_t ch);   // 0x013CE758
extern unsigned int g_uWordWrapCodePage;                          // 0x01397098
extern unsigned long g_dwWordWrapFlags;                           // 0x013CE760
extern unsigned long g_dwWordWrapNoDefaultCharCheck;              // 0x013CE764
extern wchar_t g_wcWordWrapDefaultChar;                           // 0x013CE768
extern char g_szWordWrapDefaultChar[3];                           // 0x013CE76C
extern wchar_t g_wszWordWrapBuffer[0x2000];                       // 0x013CE770

const wchar_t* __fastcall WordWrap_FindNextLineW(const wchar_t* text, unsigned int maxWidth,
                                                 const wchar_t** lineEnd);   // 0x00A10390

const char* __fastcall WordWrap_FindNextLineA(const char* text, unsigned int maxWidth, const char** lineEnd)
{
    if (g_pfnWordWrapGetCharWidth == 0 || text == 0 || *text == 0) {
        *lineEnd = 0;
        return 0;
    }
    int truncated = 0;
    if (MultiByteToWideChar(g_uWordWrapCodePage, 0, text, -1, g_wszWordWrapBuffer, 0x2000) == 0) {
        if (GetLastError() != 122)                  // ERROR_INSUFFICIENT_BUFFER
        {
            *lineEnd = 0;
            return 0;
        }
        truncated = 1;
    }
    g_wszWordWrapBuffer[0x1FFF] = 0;
    if (!(g_dwWordWrapFlags & 2) && g_dwWordWrapNoDefaultCharCheck == 0 && g_wcWordWrapDefaultChar != 0) {
        const char* source = text;
        for (int i = 0; g_wszWordWrapBuffer[i] != 0; ++i) {
            if (g_wszWordWrapBuffer[i] == g_wcWordWrapDefaultChar &&
                (source[0] != g_szWordWrapDefaultChar[0] ||
                 (g_szWordWrapDefaultChar[1] != 0 &&
                  (source[1] != g_szWordWrapDefaultChar[1] || g_szWordWrapDefaultChar[2] != 0))))
                g_wszWordWrapBuffer[i] = g_uWordWrapCodePage == 1252 ? 0x003F : 0x4E9E;
            source = CharNextExA((unsigned short)g_uWordWrapCodePage, source, 0);
        }
    }
    const wchar_t* lineEndWide = 0;
    const wchar_t* nextLineWide = WordWrap_FindNextLineW(g_wszWordWrapBuffer, maxWidth, &lineEndWide);
    const char* nextLine = 0;
    if (nextLineWide != 0 || truncated) {
        if (nextLineWide == 0)
            nextLineWide = &g_wszWordWrapBuffer[0x1FFF];
        int count = (int)(nextLineWide - g_wszWordWrapBuffer);
        nextLine = text;
        for (; count != 0; --count)
            nextLine = CharNextExA((unsigned short)g_uWordWrapCodePage, nextLine, 0);
    }
    if (lineEndWide != 0) {
        int count = (int)(lineEndWide - g_wszWordWrapBuffer);
        *lineEnd = text;
        for (; count != 0; --count)
            *lineEnd = CharNextExA((unsigned short)g_uWordWrapCodePage, *lineEnd, 0);
    }
    return nextLine;
}
