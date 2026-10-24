// RetrieveStringByLanguage(HINSTANCE hInstance, unsigned long id, char* buffer, unsigned long bufferSize, unsigned short language) -- load string-table entry `id` in one language as ANSI text
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099D3A0, 142 bytes. FableWin.pdb ?RetrieveStringByLanguage@@YIHPAUHINSTANCE__@@KPADKG@Z:
// `__fastcall` (hInstance in ecx, id in edx, the rest on the stack, `ret 0Ch`).
// Callees (KERNEL32 imports): FindResourceExA @ [0x0143FD8C], LoadResource @ [0x0143FD90], LockResource @ [0x0143FD94],
// WideCharToMultiByte @ [0x0143FD98].
// Shape: the string-table block is resource (id >> 4) + 1 of type RT_STRING (6); its 16 entries are counted UTF-16
// strings, walked with an unsigned index (`jb`) until entry id & 15 (hoisted into edi before the first call) is
// found non-empty. The converted length is the result; the terminator goes at min(length, bufferSize - 1) compared
// signed (`jge`). Every failure shares the one `xor eax,eax` exit.
// Leaf: ?RetrieveStringByLanguage@@
struct HINSTANCE__ { int unused; };
typedef HINSTANCE__* HINSTANCE;
typedef HINSTANCE HMODULE;
struct HRSRC__ { int unused; };
typedef HRSRC__* HRSRC;
typedef void* HGLOBAL;
typedef unsigned short WCHAR;   // wchar_t is not a built-in type under the retail flags

extern "C" __declspec(dllimport) HRSRC __stdcall FindResourceExA(HMODULE module, const char* type, const char* name,
                                                                 unsigned short language);   // [0x0143FD8C]
extern "C" __declspec(dllimport) HGLOBAL __stdcall LoadResource(HMODULE module, HRSRC resource);        // [0x0143FD90]
extern "C" __declspec(dllimport) void* __stdcall LockResource(HGLOBAL data);                            // [0x0143FD94]
extern "C" __declspec(dllimport) int __stdcall WideCharToMultiByte(unsigned int codePage, unsigned long flags,
    const WCHAR* wideText, int wideCount, char* text, int textSize, const char* defaultChar,
    int* usedDefaultChar);                                                                              // [0x0143FD98]

#define RT_STRING_ID          ((const char*)6)
#define MAKEINTRESOURCE_ID(i) ((const char*)(unsigned long)(unsigned short)(i))
#define MIN_OF(a, b)          (((a) < (b)) ? (a) : (b))

int __fastcall RetrieveStringByLanguage(HINSTANCE hInstance, unsigned long id, char* buffer,
                                        unsigned long bufferSize, unsigned short language)
{
    unsigned long entry = id & 15;
    HRSRC resource = FindResourceExA(hInstance, RT_STRING_ID, MAKEINTRESOURCE_ID((id >> 4) + 1), language);
    if (resource != 0) {
        HGLOBAL data = LoadResource(hInstance, resource);
        if (data != 0) {
            const WCHAR* text = (const WCHAR*)LockResource(data);
            if (text != 0) {
                for (unsigned long i = 0; i < 16; i++) {
                    unsigned int length = *text++;
                    if (length != 0 && i == entry) {
                        int converted = WideCharToMultiByte(0, 0, text, length, buffer, bufferSize, 0, 0);
                        if (converted == 0)
                            return 0;
                        buffer[MIN_OF(converted, (int)bufferSize - 1)] = 0;
                        return converted;
                    }
                    text += length;
                }
            }
        }
    }
    return 0;
}