// _global::StripSubAnimationSuffix(CString& name) -- cuts the last five characters (the `_SUBn` suffix) off a name longer than five characters by writing a NUL at length - 5
// Leaf: StripSubAnimationSuffix
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C2ED20, 39 bytes, `ret` with the string in ecx. Descriptive name (no PDB covers
//   this library): the catalog's `PairStringAllocator_Construct` is propagated; no direct caller is left in the image
//   (the companion of IsSubAnimationName 0x00C2ECD0).
// Callees: CString::SafeStrlen @ 0x00C0BCC0 (static __stdcall, called twice), CString::SetAt @ 0x00C0C970.
// Shape: `__fastcall` with the string by reference in ecx (kept in esi); the signed `> 5` gives `jle`.
class CString {
public:
    char* m_pchData;                            // +0x00
    static int __stdcall SafeStrlen(const char* lpsz);   // 0x00C0BCC0
    void SetAt(int nIndex, char ch);                     // 0x00C0C970
};

void __fastcall StripSubAnimationSuffix(CString& name)
{
    if (CString::SafeStrlen(name.m_pchData) > 5)
        name.SetAt(CString::SafeStrlen(name.m_pchData) - 5, 0);
}