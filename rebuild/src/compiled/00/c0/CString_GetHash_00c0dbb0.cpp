// CString::GetHash() const -- the Jenkins hash (seed 0) of the string's text (m_pchData at +0x00)
// Leaf: ?GetHash@CString@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0DBB0, length 24 bytes.
// Naming (sweep1, 2026-10-25): was landed as CNamedRecord_00c0dbb0::JenkinsHashOfName; descriptive (no PDB covers this
//   library). `this` is the library's MFC-style CString: callers 0x00C101F9 (CNamedSet::Find), 0x00C1EF3F / 0x00C1EF70
//   (CItemAction::CItemAction, landed, which declares `unsigned long CString::GetHash() const // 0x00C0DBB0` and calls
//   it on its by-value CString name), 0x00C2E6EE and 0x00C2E7CE; the length callee 0x00C0BCC0 is the landed
//   CString::SafeStrlen (static __stdcall: 0 for null, else lstrlenA through 0x0143FD88).
// Callees: CString::SafeStrlen @ 0x00C0BCC0, JenkinsHashBuffer(const char*, unsigned long, unsigned long) @ 0x00C1D500
// (__fastcall: data in ecx, length in edx, seed on the stack -- the `push 0` before the length call is its
// third argument, consumed by the hash's own `ret`).
// Shape: arguments evaluated right to left: the seed is pushed, the length computed by the call, the string
// re-read from `this` (esi) for ecx. The manifest's "CAnimationSet::BeginLoading" label is propagated.
unsigned long __fastcall JenkinsHashBuffer(const char* data, unsigned long length, unsigned long seed);   // 0x00C1D500

class CString {
public:
    char* m_pchData;                            // +0x00
    static int __stdcall SafeStrlen(const char* lpsz);   // 0x00C0BCC0
    unsigned long GetHash() const;
};

unsigned long CString::GetHash() const
{
    return JenkinsHashBuffer(m_pchData, SafeStrlen(m_pchData), 0);
}
