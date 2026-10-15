// CThingComponentSet::Add(const CEntry& entry) -- grow the entry vector by one default entry and overwrite it with the given one
// Retail: Fable.exe (TLC, VC7.1) @ 0x00870480, 82 bytes (+ int3 padding).
// Callees: CSmallVector<CThingComponentSet::CEntry, 8>::resize(long n, const CEntry& fill) @ 0x00870370 (the manifest label
//          names the CANavQuadTreeNode instantiation; propagated).
// Layout: PDB CThingComponentSet = Entries (CSmallVector<CEntry,8>: Start pointer +0, signed char Size +4);
//   CEntry is 9 bytes (Name CDefString +0, DriverType +4, Override +8; the element stride is 9: `lea edx,[eax+eax*8]`),
//   so it is declared packed as in the PDB.
// Shape: the fill value is a default CEntry temporary in the frame (DriverType/Override zeroed before the argument
//   pushes, Name's -1 after them); the new last element (`Start + Size*9 - 9`) is assigned memberwise.
#pragma pack(push, 1)
struct CDefString {
    long TablePos;
    CDefString() : TablePos(-1) { }
};

struct CEntry {                         // CThingComponentSet::CEntry, 9 bytes (PDB: packed)
    CDefString Name;                    // +0x00
    unsigned long DriverType;           // +0x04
    bool Override;                      // +0x08
    CEntry() : DriverType(0), Override(false) { }
};
#pragma pack(pop)

template<class T, int N> struct CSmallVector {
    T* Start;                           // +0x00
    signed char Size;                   // +0x04
    void resize(long n, const T& fill);     // 0x00870370
    T& back() { return Start[Size - 1]; }
};

struct CThingComponentSet {
    CSmallVector<CEntry, 8> Entries;    // +0x00
    void Add(const CEntry& entry);
};

void CThingComponentSet::Add(const CEntry& entry)
{
    Entries.resize(Entries.Size + 1, CEntry());
    Entries.back() = entry;
}