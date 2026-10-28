// CCharString::Truncate(unsigned long length) -- shorten the string to `length` characters when it is longer (to zero: assign the empty string; otherwise take a private copy of the data block and truncate it)
// Leaf: ?Truncate@CCharString@@QAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099F2A0, 58 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity (proven): both PDBs carry ?Truncate@CCharString@@QAEXK@Z, its callers (0x009982DB, 0x0099845E, 0x00E5ED75) already bind
//   that symbol to 0x0099F2A0 (rebuild/oracles/reloc_bindings.tsv), and it calls CBasicString<char>::Truncate on the data block.
// Callees: CCharString::operator=(const char*) 0x0099EFE0 (the "" literal 0x0122D70E), CCharString::MakeOwnCopyOfData 0x0099EAF0
//   (protected), CBasicString<char>::Truncate(unsigned long) 0x009A0080.
// Shape: `if (GetLength() > length)` with the inline null-safe GetLength (the explicit `PStringData && ...` form loads the
//   length into ecx and reloads `this`); the zero-length arm tail-duplicates its own return.
// ---- CCharString model (Ego_r: CCharString = one CCharStringData*; CCharStringData = CBasicString<char> Data + NoRefs) ----
template <class T> class CBasicString {
public:
    T*            PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    unsigned long UseFastExtend : 1;
    void Truncate(unsigned long length);                // 0x009A0080
    void Insert(unsigned long position, T* text);       // 0x009A06B0
};
class CCharStringData {
public:
    CBasicString<char> Data;                            // +0x00
    long               NoRefs;
};
class CCharString {
public:
    CCharStringData* PStringData;                       // +0x00
    const CCharString& operator=(const char* text);     // 0x0099EFE0
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->Data.StringLength; }
    void Truncate(unsigned long length);
protected:
    void MakeOwnCopyOfData();                           // 0x0099EAF0
};

void CCharString::Truncate(unsigned long length)
{
    if (GetLength() > length) {
        if (length == 0) {
            *this = "";
            return;
        }
        MakeOwnCopyOfData();
        PStringData->Data.Truncate(length);
    }
}