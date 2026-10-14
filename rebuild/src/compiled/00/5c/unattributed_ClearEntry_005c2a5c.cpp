// CSoundPairTable_005c2a5c::ClearEntry(long index) -- stores a default (all-zero) {id, CSoundPair} entry at `index` of the owned array
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C2A5C, length 48 bytes.
// Callees: CArray<std::pair<unsigned long, CSoundPair>>::SetAt @ 0x005C2A13 (manifest "push_back":
// (index, const entry&) -- it overwrites in range and grows otherwise, `ret 8`), and the
// CCountedPointer destructor @ 0x005C04BF on the entry's +8 (releases the block, nulls both words).
// The manifest's "CAC_HighScorePost::CAC_HighScorePost" label is a constructor name on a body
// that returns nothing (`leave ; ret 4`, no `mov eax,esi`), so it is rejected; the class is
// named by what the body does. Frame-pointer body with `push dword ptr [ebp+8]`, `leave` and no
// spills, but with the entry's constructors inlined: a size-optimised TU with /Oy off only
// (optimize("y",off) then ("s",on); the "gsy" form of GOTCHAS 114 disables inlining and spills
// `this`). The 0x14-byte entry is zeroed through one register by its inline constructors
// (pair -> CSoundPair -> CCountedPointer), passed by reference after the index, and only the
// counted pointer sub-object has a destructor to run.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCountedPointer {                    // 8 bytes: object, count block
    void* m_pObject;
    void* m_pBlock;
    CCountedPointer() : m_pObject(0), m_pBlock(0) { }
    ~CCountedPointer();                     // 0x005C04BF
};

struct CSoundPair {                         // 0x10 bytes
    long            m_sound;                // +0x00
    CCountedPointer m_pSample;              // +0x04
    long            m_volume;               // +0x0c
    CSoundPair() : m_sound(0), m_pSample(), m_volume(0) { }
};

struct SSoundPairEntry {                    // std::pair<unsigned long, CSoundPair>, 0x14 bytes
    unsigned long first;
    CSoundPair    second;
    SSoundPairEntry() : first(0), second() { }
};

struct CSoundPairArray {
    SSoundPairEntry* first;
    SSoundPairEntry* last;
    void SetAt(long index, const SSoundPairEntry& entry);   // 0x005C2A13
};

struct CSoundPairTable_005c2a5c {
    void*           __vftable;              // +0x00
    CSoundPairArray* m_pEntries;            // +0x04
    void ClearEntry(long index);
};

void CSoundPairTable_005c2a5c::ClearEntry(long index)
{
    SSoundPairEntry entry;
    m_pEntries->SetAt(index, entry);
}