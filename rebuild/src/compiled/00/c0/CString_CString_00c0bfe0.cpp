// CString::CString(const char* lpsz) -- MFC-style CString constructor from a C string: start on the shared empty text, then allocate and copy the characters when the (null-safe) length is not zero
// Leaf: ??0CString
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0BFE0, 62 bytes (`ret 4`). Label correction: the manifest's
//   "Std_Allocator_Construct" is propagated; the body is MFC's CString(LPCTSTR) without the MAKEINTRESOURCE LoadString
//   arm (25 callers build strings from literals, e.g. the landed CAbilitySoundComponent ctor 0x00C0B200 "G"/"P").
// Callees: CString::SafeStrlen @ 0x00C0BCC0 (static __stdcall), CString::AllocBuffer @ 0x00C0BDA0; memcpy is the
//   intrinsic (rep movsd ; rep movsb).
// Globals: _afxPchNil @ 0x0139C900 (Init(), loaded first).
#include <stddef.h>
extern "C" void* __cdecl memcpy(void* dst, const void* src, size_t n);
#pragma intrinsic(memcpy)

extern char* _afxPchNil;                        // 0x0139C900

class CString {
public:
    char* m_pchData;                            // +0x00
    CString(const char* lpsz);
    void Init() { m_pchData = _afxPchNil; }
    void AllocBuffer(int nLen);                 // 0x00C0BDA0
    static int __stdcall SafeStrlen(const char* lpsz);   // 0x00C0BCC0
};

CString::CString(const char* lpsz)
{
    Init();
    int nLen = SafeStrlen(lpsz);
    if (nLen != 0) {
        AllocBuffer(nLen);
        memcpy(m_pchData, lpsz, nLen * sizeof(char));
    }
}