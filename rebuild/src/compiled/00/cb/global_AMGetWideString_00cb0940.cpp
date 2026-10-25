// AMGetWideString(const unsigned short* psz, unsigned short** ppszReturn) -- DirectShow wxutil.cpp: CoTaskMemAlloc a copy of a wide string (E_POINTER / E_OUTOFMEMORY / NOERROR)
// Leaf: _AMGetWideString@8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB0940, 83 bytes (+ int3 padding). STDAPI = extern "C" __stdcall, `ret 8`.
// Library: the DirectShow base classes (strmbase, DX9 SDK wxutil.cpp) linked into Fable.exe next to the other
// landed strmbase rows (CMediaType 0x00CB19C0.., CBaseFilter 0x00CA7xxx). The manifest label
// "COM_AllocateWideString" is descriptive.
// Callees: kernel32 lstrlenW (IAT 0x0143FE48), ole32 CoTaskMemAlloc (IAT 0x01440638); CopyMemory is the memcpy
// intrinsic (`rep movsd` + `rep movsb`).
// Shape: the pre-2005 SDK body -- CheckPointer(ppszReturn, E_POINTER), ValidateReadWritePtr is empty in retail
// builds, nameLen = sizeof(WCHAR) * (lstrlenW(psz)+1) (`lea edi,[eax+eax+2]`), the allocation stored through
// ppszReturn before its null test.
#include <string.h>

extern "C" __declspec(dllimport) int __stdcall lstrlenW(const unsigned short* string);    // IAT 0x0143FE48
extern "C" __declspec(dllimport) void* __stdcall CoTaskMemAlloc(unsigned long cb);        // IAT 0x01440638

extern "C" long __stdcall AMGetWideString(const unsigned short* psz, unsigned short** ppszReturn)
{
    if (ppszReturn == 0)
        return 0x80004003L;                         // E_POINTER (CheckPointer)
    unsigned long nameLen = sizeof(unsigned short) * (lstrlenW(psz) + 1);
    *ppszReturn = (unsigned short*)CoTaskMemAlloc(nameLen);
    if (*ppszReturn == 0)
        return 0x8007000EL;                         // E_OUTOFMEMORY
    memcpy(*ppszReturn, psz, nameLen);
    return 0;                                       // NOERROR
}