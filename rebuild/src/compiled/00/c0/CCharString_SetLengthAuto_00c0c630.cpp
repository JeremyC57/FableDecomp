// CCharString::SetLengthAuto(int) -- MFC-style CString::ReleaseBuffer: unshare, take the length from lstrlen when -1, assert it fits the allocation, store it and terminate the text
// Leaf: ReleaseBuffer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0C630, 137 bytes. The manifest's class is the engine CCharString; the body
// belongs to the MFC-style CString of the 0x00C0xxxx library (m_pchData points past a 12-byte CStringData header
// {nRefs, nDataLength, nAllocLength}), the class of the landed Right @ 0x00C0E0F0 and ConcatCopy @ 0x00C0C220.
// Callees: CString::CopyBeforeWrite @ 0x00C0BE20, lstrlenA (IAT 0x0143FD88), the assertion hook through the
//          global pointer @ 0x0143BB2C (__fastcall, the message in ecx), CDebugBreak::CDebugBreak @ 0x00C16750.
// Shape: each GetData() inlines ASSERT(m_pchData != NULL) (message "m_pchData != NULL\n"); the explicit
// ASSERT(nNewLength <= GetData()->nAllocLength) sits between the second and third. The empty CDebugBreak
// temporary is homed in the dead nNewLength argument slot (`lea ecx,[esp+0Ch]`: the value lives in edi).
#include <stddef.h>

extern "C" __declspec(dllimport) int __stdcall lstrlenA(const char* text);   // IAT 0x0143FD88

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
};

class CString {
public:
    char* m_pchData;                            // +0x00
    CStringData* GetData() const
    {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData) - 1;
    }
    void CopyBeforeWrite();                     // 0x00C0BE20
    void ReleaseBuffer(int nNewLength);
};

void CString::ReleaseBuffer(int nNewLength)
{
    CopyBeforeWrite();
    if (nNewLength == -1)
        nNewLength = lstrlenA(m_pchData);
    LIB_ASSERT(nNewLength <= GetData()->nAllocLength)
    GetData()->nDataLength = nNewLength;
    m_pchData[nNewLength] = '\0';
}