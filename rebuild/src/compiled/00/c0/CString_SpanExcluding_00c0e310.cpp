// CString::SpanExcluding(const char* lpszCharSet) -- MFC strcore.cpp: the leading run of characters that are all outside lpszCharSet, as a new string (Left of _tcscspn)
// Leaf: ?SpanExcluding@CString@@QBE?AV1@PBD@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0E310, 80 bytes, thiscall `ret 8` (hidden result + lpszCharSet).
// Identity: libpk's copy of MFC 6 strcore.cpp (the sibling of the landed Left 0x00C0E1F0, which it calls). The ASSERT string
//   "IsValidString(lpszCharSet)\n" (0x012B1F18) is MFC's `ASSERT(AfxIsValidString(lpszCharSet))` under the libpk
//   IsValidString null test; _tcscspn is tchar.h's _MBCS inline wrapper over MSVCR71!_mbscspn (IAT 0x0143FFF4).
// Shape: the CDebugBreak temporary lands on the dead lpszCharSet argument slot (`lea ecx,[esp+14h]`), m_pchData is read
//   directly (no GetData ASSERT), and the result pointer is reloaded after the cdecl call for `push / return`.
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_CSTRING_ALLOCCOPY
#define FABLE_RETAIL_CSTRING_LEFT
#define FABLE_RETAIL_CSTRING_SPAN
#include "engine/CString.h"

#ifdef _MSC_VER
#define FABLE_CRT_IMPORT __declspec(dllimport)
#else
#define FABLE_CRT_IMPORT
#endif
extern "C" FABLE_CRT_IMPORT size_t __cdecl _mbscspn(const unsigned char* string, const unsigned char* control);   // IAT 0x0143FFF4

// tchar.h (_MBCS, no _MB_MAP_DIRECT): _tcscspn is an inline wrapper over _mbscspn
static __inline size_t __cdecl _tcscspn(const char* string, const char* control)
{
    return _mbscspn((const unsigned char*)string, (const unsigned char*)control);
}

CString CString::SpanExcluding(const char* lpszCharSet) const
{
    LIB_ASSERT(IsValidString(lpszCharSet))
    return Left((int)_tcscspn(m_pchData, lpszCharSet));
}