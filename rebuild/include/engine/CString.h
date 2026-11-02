#ifndef FABLE_PASS11_CSTRING_H
#define FABLE_PASS11_CSTRING_H
#include <stddef.h>
#include <stdarg.h>
// Retail opt-in (Codex pass 11): no pre-existing engine/CString.h was supplied.
// Layout: real libpk MFC-derived CString has its sole character pointer at +0;
// CStringData is the preceding three native long words at -12, -8 and -4.
// Native SetAt/Replace/FormatV and original MFC CString declarations corroborate.
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;
struct CDebugBreak { CDebugBreak(); };
#define IsValidString(lpsz) ((lpsz) != NULL)
#define LIB_ASSERT(cond) if (!(cond)) { if (g_pfnAssertHook) g_pfnAssertHook(#cond "\n"); CDebugBreak(); }
struct CStringData {
    long nRefs;
    long nDataLength;
    long nAllocLength;
    char* data() { return (char*)(this + 1); }
};
class CString {
public:
    char* m_pchData;
    CStringData* GetData() const {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData)-1;
    }
    int GetAllocLength() const { return GetData()->nAllocLength; }
    char* GetBuffer(int nMinBufLength);
    void ReleaseBuffer(int nNewLength = -1);
    void FormatV(const char* lpszFormat, va_list argList);
};
#endif
