// CString::Release() -- MFC-style CString::Release: unless the string uses the shared empty block, assert the block is referenced, drop one reference (freeing the block on the last) and point the string at the shared empty text
// Leaf: ?Release@CString@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0B910, 213 bytes (`ret`). Label correction: the manifest's
//   "CCharString_ReleaseRefcount" is propagated; the body is MFC's CString::Release() of the 0x00C0xxxx library
//   (assert texts "m_pchData != NULL\n" 0x012B1EC0 and "GetData()->nRefs != 0\n" 0x012B1ED4 are MFC's own); the
//   landed AllocBeforeWrite 0x00C0BEE0 calls it as Release().
// Callees: the assertion hook through the global pointer @ 0x0143BB2C (__fastcall, message in ecx),
//   CDebugBreak::CDebugBreak @ 0x00C16750, InterlockedDecrement (IAT 0x0143FD38), CString::FreeData @ 0x00C0B900
//   (__fastcall, the header in ecx: `operator delete[]`).
// Globals: _afxDataNil (the shared empty header) @ 0x0139C8FC, _afxPchNil (its text) @ 0x0139C900.
// Shape: five inlined GetData() calls, each with ASSERT(m_pchData != NULL); every empty CDebugBreak temporary lands
//   on the dead high byte of the `push ecx` frame word (`lea ecx,[esp+7]`); Init() is the final store.
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
extern char* _afxPchNil;                        // 0x0139C900

class CString {
public:
    char* m_pchData;                            // +0x00
    CStringData* GetData() const
    {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData) - 1;
    }
    void Init() { m_pchData = _afxPchNil; }
    static void __fastcall FreeData(CStringData* pData);   // 0x00C0B900
    void Release();
};

void CString::Release()
{
    if (GetData() != _afxDataNil) {
        LIB_ASSERT(GetData()->nRefs != 0)
        if (InterlockedDecrement(&GetData()->nRefs) <= 0)
            FreeData(GetData());
        Init();
    }
}