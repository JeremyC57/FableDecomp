// CString::ReverseFind(char ch) const -- MFC-style CString::ReverseFind: the index of the last occurrence of ch (_mbsrchr, DBCS aware), -1 when absent
// Leaf: ReverseFind
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0BC20, 36 bytes (`ret 4`). Label correction: the manifest's
//   "CCharString_ReverseFind" names the engine string; the body belongs to the MFC-style CString of the 0x00C0xxxx
//   library (callers 0x00C0E385 / 0x00C0E3B7 are the landed GetFileNameWithoutExtension 0x00C0E360).
// Callees: MSVCR71!_mbsrchr through the import slot 0x0143FFD4 (the character zero-extended: `movzx ecx,byte`).
// Shape: the MFC text through tchar.h's _MBCS inline wrapper `_tcsrchr(const char*, unsigned int)` (calling _mbsrchr
//   directly loads the character into eax and the text into ecx; the wrapper gives retail's ecx/eax); then
//   `(lpsz == NULL) ? -1 : (int)(lpsz - m_pchData)`, the -1 arm `or eax,-1` with its own ret.
#include <stddef.h>

extern "C" __declspec(dllimport) unsigned char* __cdecl _mbsrchr(const unsigned char* string, unsigned int c);   // IAT 0x0143FFD4

// tchar.h (_MBCS, no _MB_MAP_DIRECT): _tcsrchr is an inline wrapper over _mbsrchr
static __inline char* __cdecl _tcsrchr(const char* s, unsigned int c)
{
    return (char*)_mbsrchr((const unsigned char*)s, c);
}

class CString {
public:
    char* m_pchData;                            // +0x00
    int ReverseFind(char ch) const;
};

int CString::ReverseFind(char ch) const
{
    char* lpsz = _tcsrchr(m_pchData, (unsigned char)ch);
    return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}