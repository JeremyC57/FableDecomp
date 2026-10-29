// _global::GetNumberOfCharactersOfTypeInString(const CWideString& text, wchar_t character) -- count how many times a character occurs in a wide string
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048DBF9, 67 bytes, plain `ret`.
// Callees: CWideString::GetAsCharArray() @ 0x0099AF10, CWideString::GetLength @ 0x0099B220.
// Shape: Ego_r.pdb `?GetNumberOfCharactersOfTypeInString@@YIJABVCWideString@@_W@Z` -- a `__fastcall` free
// function (text in ecx, the character in dx). The data pointer is fetched once before the loop, GetLength()
// is re-called for every test (top-tested `for (i = 0; i < GetLength(); ++i)`, rotated); with esi/edi/ebx/ebp
// holding text, index, character and data the count lives in the one-word frame (`push ecx`, `inc [esp+10h]`).
// `wchar_t` is the era `unsigned short` typedef (no /Zc:wchar_t in the retail build).
// Pragma: optimize("s") -- the CSubtitleRenderer TU (0x0048DB46..0x00490A22) is /Os; the speed build swaps the
// index/text registers (esi/edi) and no declaration order fixes that.
// Leaf: GetNumberOfCharactersOfTypeInString
#pragma optimize("s", on)

typedef unsigned short wchar_t16;

class CWideString {
public:
    void* PStringData;
    const wchar_t16* GetAsCharArray() const;          // 0x0099AF10
    long GetLength() const;                     // 0x0099B220
};

long __fastcall GetNumberOfCharactersOfTypeInString(const CWideString& text, wchar_t16 character)
{
    long count = 0;
    const wchar_t16* data = text.GetAsCharArray();
    for (long i = 0; i < text.GetLength(); ++i) {
        if (data[i] == character)
            ++count;
    }
    return count;
}