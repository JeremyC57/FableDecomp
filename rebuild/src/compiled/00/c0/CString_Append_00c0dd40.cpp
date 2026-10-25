// CString::Append(const char* text) -- MFC-style append of a C string in place: ConcatInPlace with SafeStrlen(text) and the text
// Leaf: ?Append@CString@@QAEXPBD@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0DD40, 24 bytes, __thiscall, `ret 4`:
//   mov eax,[esp+4] ; push esi ; push eax ; push eax ; mov esi,ecx ; call 0xc0bcc0 ; push eax ; mov ecx,esi ;
//   call 0xc0c270 ; pop esi ; ret 4
// Naming (sweep1, 2026-10-25): was landed as CWorldMap::ApplyScriptedMapBrush (a propagated manifest label; its stand-in
//   called the callees ResolveMapBrush / ConsumeMapBrush); descriptive (no PDB covers this library). The callees are the
//   landed CString::SafeStrlen 0x00C0BCC0 (static __stdcall: 0 for null, else lstrlenA) and CString::ConcatInPlace
//   0x00C0C270 (the callee of the landed CString::Append(const CString&) 0x00C0DCF0, with the same (length, text)
//   arguments). Its five callers are the CString stream operator<< family: the text overload 0x00C0DEC4 and the
//   sprintf-based number/pointer helpers 0x00C0DD93 (double), 0x00C0DDDE (long), 0x00C0DE2E (unsigned long) and
//   0x00C0DE7E (pointer), each of which formats into a stack buffer and appends it here.
// Shape: arguments evaluated right to left: the text is pushed for ConcatInPlace first, then again for SafeStrlen.
class CString {
public:
    char* m_pchData;                            // +0x00
    static int __stdcall SafeStrlen(const char* lpsz);           // 0x00C0BCC0
    void ConcatInPlace(int nSrcLen, const char* lpszSrcData);   // 0x00C0C270
    void Append(const char* text);
};

void CString::Append(const char* text)
{
    ConcatInPlace(SafeStrlen(text), text);
}
