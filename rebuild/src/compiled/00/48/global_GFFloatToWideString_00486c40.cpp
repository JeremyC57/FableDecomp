// _global::GFFloatToWideString(double value) -- format a number with L"%f" and strip the trailing zeros after the decimal point (keeping one digit after a bare '.')
// Retail: Fable.exe (TLC, VC7.1) @ 0x00486C40, 224 bytes, `ret 8`.
// Callees: CWideString() @ 0x0099AED0, CWideString::Sprintf(const wchar_t*, ...) @ 0x0099BA70 (cdecl,
// format L"%f" @ 0x01237EF4), GetLength @ 0x0099B220, GetAsCharArray() @ 0x0099AF10, SetLength @ 0x0099BC50,
// operator+=(wchar_t) @ 0x0099B9A0, CWideString(const CWideString&) @ 0x0099B720, ~CWideString @ 0x0099B510.
// Shape: Ego_r.pdb `?GFFloatToWideString@@YI?AVCWideString@@N@Z` -- `__fastcall` with the hidden result in ecx (kept
// in edi) and the double on the stack. Scan for the '.' (GetLength re-called per test); once found, drop trailing
// '0's while the string is non-empty -- the last character is read as `text[GetLength() - 1]` (GetLength before
// the data pointer) -- and append a '0' when the '.' itself becomes last. The local is copied into the result
// (VC7.1 has no NRVO) and destroyed; no EH frame. Speed TU (aligned loop heads).
// Leaf: GFFloatToWideString

typedef unsigned short wchar_t16;

class CWideString {
public:
    void* PStringData;
    CWideString();                                          // 0x0099AED0
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510
    const wchar_t16* GetAsCharArray() const;                      // 0x0099AF10
    long GetLength() const;                                 // 0x0099B220
    void SetLength(long length);                            // 0x0099BC50
    const CWideString& operator+=(wchar_t16 c);             // 0x0099B9A0
    void __cdecl Sprintf(const wchar_t16* format, ...);   // 0x0099BA70
};


CWideString __fastcall GFFloatToWideString(double value)
{
    CWideString text;
    text.Sprintf(L"%f", value);
    for (long i = 0; i < text.GetLength(); ++i) {
        if (text.GetAsCharArray()[i] == '.') {
            while (text.GetLength() > 0) {
                wchar_t16 last = text.GetAsCharArray()[text.GetLength() - 1];
                if (last == '.') {
                    text += '0';
                    break;
                }
                if (last != '0')
                    break;
                text.SetLength(text.GetLength() - 1);
            }
            break;
        }
    }
    return text;
}