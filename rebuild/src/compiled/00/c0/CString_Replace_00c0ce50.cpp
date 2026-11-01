// CString::Replace(const char* lpszOld, const char* lpszNew) -- MFC-style CString::Replace: count the occurrences of lpszOld, then (copy-before-write, reallocating when too small or shared) replace each in place with lpszNew; returns the count
// Leaf: ?Replace@CString@@QAEHPBD0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0CE50, 681 bytes, thiscall `ret 8`, returns the replacement count. No callers
//   (an unreferenced library member kept by the linker).
// Identity (two sources): (1) the body is MFC's CString::Replace(LPCTSTR lpszOld, LPCTSTR lpszNew) (strex.cpp) line for line --
//   the SafeStrlen pair with the early `return 0`, the strstr / lstrlen counting loop, CopyBeforeWrite, the
//   `nAllocLength < nNewLength || nRefs > 1` reallocation (AllocBuffer, memcpy, Release(pOldData)), the memmove / memcpy
//   replacement loop and its assertion text "m_pchData[nNewLength] == '\0'\n" (0x012B20CC); (2) the row sits in the libpk
//   MFC-style CString TU between the landed CString::Insert 0x00C0CB50 and the other strex members, calling the landed
//   CString::SafeStrlen 0x00C0BCC0, CopyBeforeWrite 0x00C0BE20, GetData 0x00C0B8C0, AllocBuffer 0x00C0BDA0 and
//   Release(CStringData*) 0x00C0B9F0. The catalog label "CCharString_ReplaceAll" is a descriptive guess (libpk has no PDB);
//   the CString member name is MFC's. Spelling as the landed CString rows (public members, `int`, PASCAL statics).
// Callees: SafeStrlen x2 (__stdcall), strstr (IAT 0x0143FFDC), lstrlenA (IAT 0x0143FD88), CopyBeforeWrite, GetData (VC7.1
//   expands the inline GetData everywhere but the `nRefs > 1` test, where it calls the out-of-line COMDAT 0x00C0B8C0 -- the
//   inline budget, reproduced from the in-class body), AllocBuffer, Release, memmove (IAT 0x01440144), memcpy intrinsic,
//   the assertion hook through 0x0143BB2C ("m_pchData != NULL\n" 0x012B1EC0) and CDebugBreak::CDebugBreak 0x00C16750.
#include <stddef.h>
extern "C" void* __cdecl memcpy(void* dst, const void* src, size_t n);
#pragma intrinsic(memcpy)
extern "C" __declspec(dllimport) void* __cdecl memmove(void* dst, const void* src, size_t n);   // IAT 0x01440144
extern "C" __declspec(dllimport) char* __cdecl strstr(const char* s, const char* sub);          // IAT 0x0143FFDC
extern "C" __declspec(dllimport) int __stdcall lstrlenA(const char* text);                      // IAT 0x0143FD88

typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;           // 0x0143BB2C

struct CDebugBreak {                            // empty temporary whose constructor calls DebugBreak
    CDebugBreak();                              // 0x00C16750
};

#define LIB_ASSERT(cond) \
    if (!(cond)) { if (g_pfnAssertHook) g_pfnAssertHook(#cond "\n"); CDebugBreak(); }

struct CStringData {                            // the 12-byte header in front of the characters
    long nRefs;
    long nDataLength;
    long nAllocLength;
    char* data() { return (char*)(this + 1); }
};

class CString {
public:
    char* m_pchData;                            // +0x00
    CStringData* GetData() const
    {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData) - 1;
    }
    static void __stdcall Release(CStringData* pData);   // 0x00C0B9F0
    static int __stdcall SafeStrlen(const char* lpsz);   // 0x00C0BCC0
    void AllocBuffer(int nLen);                 // 0x00C0BDA0
    void CopyBeforeWrite();                     // 0x00C0BE20
    int Replace(const char* lpszOld, const char* lpszNew);
};

int CString::Replace(const char* lpszOld, const char* lpszNew)
{
    int nSourceLen = SafeStrlen(lpszOld);
    if (nSourceLen == 0)
        return 0;
    int nReplacementLen = SafeStrlen(lpszNew);

    int nCount = 0;
    char* lpszStart = m_pchData;
    char* lpszEnd = m_pchData + GetData()->nDataLength;
    char* lpszTarget;
    while (lpszStart < lpszEnd) {
        while ((lpszTarget = strstr(lpszStart, lpszOld)) != NULL) {
            nCount++;
            lpszStart = lpszTarget + nSourceLen;
        }
        lpszStart += lstrlenA(lpszStart) + 1;
    }

    if (nCount > 0) {
        CopyBeforeWrite();

        int nOldLength = GetData()->nDataLength;
        int nNewLength = nOldLength + (nReplacementLen - nSourceLen) * nCount;
        if (GetData()->nAllocLength < nNewLength || GetData()->nRefs > 1) {
            CStringData* pOldData = GetData();
            char* pstr = m_pchData;
            AllocBuffer(nNewLength);
            memcpy(m_pchData, pstr, pOldData->nDataLength * sizeof(char));
            CString::Release(pOldData);
        }
        lpszStart = m_pchData;
        lpszEnd = m_pchData + GetData()->nDataLength;

        while (lpszStart < lpszEnd) {
            while ((lpszTarget = strstr(lpszStart, lpszOld)) != NULL) {
                int nBalance = nOldLength - (lpszTarget - m_pchData + nSourceLen);
                memmove(lpszTarget + nReplacementLen, lpszTarget + nSourceLen, nBalance * sizeof(char));
                memcpy(lpszTarget, lpszNew, nReplacementLen * sizeof(char));
                lpszStart = lpszTarget + nReplacementLen;
                lpszStart[nBalance] = '\0';
                nOldLength += (nReplacementLen - nSourceLen);
            }
            lpszStart += lstrlenA(lpszStart) + 1;
        }
        LIB_ASSERT(m_pchData[nNewLength] == '\0')
        GetData()->nDataLength = nNewLength;
    }
    return nCount;
}