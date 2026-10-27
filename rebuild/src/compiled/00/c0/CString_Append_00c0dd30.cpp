// CString::Append(char ch) -- libpk string: append one character in place (ConcatInPlace(1, &ch))
// Leaf: ?Append@CString@@QAEXD@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0DD30, 15 bytes, thiscall, `ret 4`: `lea eax,[esp+4] ; push eax ; push 1 ;
//   call 0x00C0C270`, the character's own argument slot is the one-byte source.
// Identity (probable): the char overload next to the landed Append(const char*) 0x00C0DD40 and Append(double)
//   0x00C0DD60 of the 0x00C0xxxx MFC-style CString (the MFC operator+=(TCHAR) body `ConcatInPlace(1, &ch)`); the
//   descriptive Append name follows the landed siblings.
// Callee: CString::ConcatInPlace(int nSrcLen, const char* lpszSrcData) 0x00C0C270.
class CString {
public:
    char* m_pchData;                            // +0x00
    void ConcatInPlace(int nSrcLen, const char* lpszSrcData);   // 0x00C0C270
    void Append(char ch);
};

void CString::Append(char ch)
{
    ConcatInPlace(1, &ch);
}