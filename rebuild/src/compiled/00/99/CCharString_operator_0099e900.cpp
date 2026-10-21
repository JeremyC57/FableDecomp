// CCharString::operator!=(const CCharString& other) const -- negation of the inline string equality (same block, or same length and equal characters)
// Leaf: ??9CCharString@@QBE_NABV0@@Z
// Retail: Fable.exe @ 0x0099E900, 83 bytes, `ret 4`; ~30 callers (0x00499CBB, 0x004AF7D1, ... 0x009987EF); Ego_r.pdb
// ??9CCharString@@QBE_NABV0@@Z. Catalog row label "std::less<CCharString>::operator()" is wrong (the ghidra label file
// already says CCharString::operator!=, chunk 227): every exit negates the equality result (`test al,al ; sete cl`).
// Body: return !(*this == other), operator== inlined: same data block -> true; both blocks non-null with equal length
// (+4) -> Compare(chars, other chars) == 0 (0x00411570, __fastcall); else false.
// Layout: CCharString {CCharStringData* PStringData}; data block {const char* chars +0, length +4} (the landed 0x0083CC10
// model; Ego_r.pdb CCharStringData = CBasicString<char> Data + NoRefs).
// Shape: /O2; the equality returns `Compare(...) == 0` (`neg ; sbb ; inc`).
struct CCharStringData {
    const char*   Chars;                                 // +0x00
    unsigned long Length;                                // +0x04
};
long __fastcall Compare(const char* a, const char* b);   // 0x00411570
class CCharString {
public:
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->Length == other.PStringData->Length)
            return Compare(PStringData->Chars, other.PStringData->Chars) == 0;
        return false;
    }
    bool operator!=(const CCharString& other) const;
};
bool CCharString::operator!=(const CCharString& other) const
{
    return !(*this == other);
}