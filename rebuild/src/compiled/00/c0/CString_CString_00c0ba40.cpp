// CString::~CString() -- MFC-style CString destructor: unless the string uses the shared empty block, drop one reference and free the block when it was the last
// Leaf: ??1CString@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0BA40, 137 bytes. Naming (sweep1, 2026-10-25): was landed as
// CCharStringData::Release (the manifest label, propagated) although this source already built ~CString:
// the body is the destructor of the MFC-style CString of the 0x00C0xxxx library (m_pchData points past the 12-byte
// CStringData header {nRefs, nDataLength, nAllocLength}); the landed Right @ 0x00C0E0F0 calls it as ~CString.
// Callees: the assertion hook through the global pointer @ 0x0143BB2C (__fastcall, message in ecx),
//          CDebugBreak::CDebugBreak @ 0x00C16750, InterlockedDecrement (IAT 0x0143FD38), CString::FreeData @ 0x00C0B900
//          (__fastcall, the header in ecx: `operator delete`; manifest "operator_new" is propagated).
// Globals: the shared empty-string header pointer @ 0x0139C8FC (_afxDataNil).
// Shape: three inlined GetData() calls, each with ASSERT(m_pchData != NULL); the empty CDebugBreak temporary lands on
// the dead high byte of the `push ecx` frame word (`lea ecx,[esp+7]`).
#include <stddef.h>

extern "C" __declspec(dllimport) long __stdcall InterlockedDecrement(long* addend);   // IAT 0x0143FD38 (same _InterlockedDecrement@4 import)

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
    CStringData* GetData() const
    {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData) - 1;
    }
    static void __fastcall FreeData(CStringData* data);   // 0x00C0B900
    ~CString();
};

CString::~CString()
{
    if (GetData() != _afxDataNil) {
        if (InterlockedDecrement(&GetData()->nRefs) <= 0)
            FreeData(GetData());
    }
}