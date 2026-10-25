// CString::operator=(const char* lpsz) -- MFC-style assignment from a C string: AssignCopy(SafeStrlen(lpsz), lpsz), return *this
// Leaf: ??4CString@@QAEABV0@PBD@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0C200, 28 bytes, __thiscall, `ret 4`:
//   mov eax,[esp+4] ; push esi ; push eax ; push eax ; mov esi,ecx ; call 0xc0bcc0 ; push eax ; mov ecx,esi ;
//   call 0xc0c060 ; mov eax,esi ; pop esi ; ret 4
// Naming (sweep1, 2026-10-25): was landed as CShaderResource::GetPixelShader (a propagated manifest label; its stand-in
//   called the callees helper_c0bcc0 / consume_c0c060); descriptive (no PDB covers this library). The callees are the
//   landed CString::SafeStrlen 0x00C0BCC0 (static __stdcall: 0 for null, else lstrlenA) and CString::AssignCopy
//   0x00C0C060 (the copy path of the landed operator=(const CString&) 0x00C0C0C0), which is MFC's
//   operator=(LPCTSTR) body. The landed caller CString::CString 0x00C0DAB0 already binds it as
//   ??4CString@@QAEABV0@PBD@Z; other callers 0x00C0DBA3, 0x00C0DC33, 0x00C277C5, 0x00C2DB88, 0x00C31269 (9 in all).
// Shape: arguments evaluated right to left: the text is pushed for AssignCopy first, then again for SafeStrlen.
class CString {
public:
    char* m_pchData;                            // +0x00
    static int __stdcall SafeStrlen(const char* lpsz);           // 0x00C0BCC0
    void AssignCopy(int nSrcLen, const char* lpszSrcData);      // 0x00C0C060
    const CString& operator=(const char* lpsz);
};

const CString& CString::operator=(const char* lpsz)
{
    AssignCopy(SafeStrlen(lpsz), lpsz);
    return *this;
}
