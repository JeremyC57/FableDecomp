// CString::Release(CStringData* pData) -- MFC-style static CString::Release (PASCAL): unless pData is the shared empty block, assert it is referenced and drop one reference, freeing the block on the last
// Leaf: ?Release@CString@@SGXPAUCStringData@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0B9F0, 63 bytes (`ret 4`, __stdcall). Label correction: the manifest's
//   "CCharStringData_ReleaseWithRefcount" is propagated; the body is MFC's `void PASCAL CString::Release(CStringData*)`
//   ("pData->nRefs != 0\n" @ 0x012B1EEC); the landed GetBuffer 0x00C0C4B0 calls it as CString::Release(pOldData).
// Callees: the assertion hook through the global pointer @ 0x0143BB2C, CDebugBreak::CDebugBreak @ 0x00C16750,
//   InterlockedDecrement (IAT 0x0143FD38), CString::FreeData @ 0x00C0B900 (__fastcall, the header in ecx).
// Globals: _afxDataNil @ 0x0139C8FC (loaded before the frame push).
// Shape: pData lives in esi, so the empty CDebugBreak temporary is homed in its dead argument slot (`lea ecx,[esp+8]`).
#include <stddef.h>

extern "C" __declspec(dllimport) long __stdcall InterlockedDecrement(long* addend);   // IAT 0x0143FD38

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
extern CStringData* _afxDataNil;                // 0x0139C8FC

class CString {
public:
    char* m_pchData;                            // +0x00
    static void __fastcall FreeData(CStringData* pData);   // 0x00C0B900
    static void __stdcall Release(CStringData* pData);
};

void __stdcall CString::Release(CStringData* pData)
{
    if (pData != _afxDataNil) {
        LIB_ASSERT(pData->nRefs != 0)
        if (InterlockedDecrement(&pData->nRefs) <= 0)
            FreeData(pData);
    }
}