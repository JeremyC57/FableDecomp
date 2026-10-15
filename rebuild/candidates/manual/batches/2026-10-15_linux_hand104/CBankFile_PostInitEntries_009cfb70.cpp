// CBankFile::PostInitEntries() -- sorts the symbol-CRC index if it is dirty, trims its storage, and packs the runtime data of a read-only bank
// Leaf: PostInitEntries
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CFB70, length 69 bytes.
// Label: the catalog name "CTCVillage::OnInitialActivate" is propagated. The body sits inside the CBankFile code
// (0x009CD140..0x009CFBC0) and works on CBankFile members: Ego_r.pdb SymbolCRCIndices (CVectorMap<unsigned long,
// unsigned long, CKeyPairCompareLess>, PDB +0xEC) lands at retail +0xD0 once the four CArrays (16 -> 12 bytes) and the
// three maps (16 -> 12) before it shrink, and the flag at +0x8D is ReadOnly (PDB +0x9D). The method name is inferred
// from the PDB's private no-argument CBankFile methods (`?PostInitEntries@CBankFile@@AAEXXZ`); verify at landing.
// Callees: the /Gr STL sort of 8-byte pairs @ 0x009B85A0 (`__fastcall`: first in ecx, last in edx, the 1-byte
// comparator by value on the stack; manifest "std::introsort_partial<CNavigationPosition>" is propagated), the
// vector-map's storage trim @ 0x009B7B10 (manifest "CArray<pair<CCharString,CSymbolInfo>>::DeallocateUnusedMemory" is
// propagated), CBankFile::PackRuntimeData() @ 0x009CD740 (tail jump).
// Shape: CVectorMap::Sort() inline (`if (NeedsSort) { sort(begin, end, Compare); NeedsSort = false; }`), the map's
// address kept in esi; the comparator byte is zero-extended into eax for the push.
struct SCRCPair {
    unsigned long Key;
    unsigned long Value;
};

struct CKeyPairCompareLess {
    unsigned char _empty;
};

void __fastcall SortCRCPairs(SCRCPair* first, SCRCPair* last, CKeyPairCompareLess compare);   // 0x009B85A0

struct CCRCVectorMap {                      // CVectorMap<unsigned long, unsigned long, CKeyPairCompareLess>
    SCRCPair*           _M_start;           // +0x00
    SCRCPair*           _M_finish;          // +0x04
    SCRCPair*           _M_end_of_storage;  // +0x08
    CKeyPairCompareLess Compare;            // +0x0C
    bool                NeedsSort;          // +0x0D
    void Sort()
    {
        if (NeedsSort) {
            SortCRCPairs(_M_start, _M_finish, Compare);
            NeedsSort = false;
        }
    }
    void DeallocateUnusedMemory();          // 0x009B7B10
};

struct CBankFile {
    void*         __vftable;                // +0x00
    unsigned char _pad_0x04[0x88];
    bool          RetailMode;               // +0x8C
    bool          ReadOnly;                 // +0x8D
    unsigned char _pad_0x8e[0x42];
    CCRCVectorMap SymbolCRCIndices;         // +0xD0
    void PackRuntimeData();                 // 0x009CD740
    void PostInitEntries();
};

void CBankFile::PostInitEntries()
{
    SymbolCRCIndices.Sort();
    SymbolCRCIndices.DeallocateUnusedMemory();
    if (ReadOnly)
        PackRuntimeData();
}
