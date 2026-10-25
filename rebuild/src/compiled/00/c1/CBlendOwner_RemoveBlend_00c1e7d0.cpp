// CBlendOwner::RemoveBlend(CBlendEntry* entry) -- removes the entry from the unkeyed list (+8) when its key (+8) resolves to -1, else from the keyed list (+0x14)
// Leaf: ?RemoveBlend@CBlendOwner@@QAEXPAUCBlendEntry@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1E7D0, length 38 bytes (`ret 4`). Descriptive name (no PDB covers
//   this library); the remove counterpart of AddBlend 0x00C1E780 (same key test, same two lists). The manifest label
//   (C3DAnimationBlendState::AddCombinationBlend) is propagated and does not fit the bytes.
// Naming (sweep1, 2026-10-25): was landed as CBlendOwner_00c1e7d0::AddCombinationBlend. The callee 0x00C1E900 is the
//   landed CPtrArray<CBlendEntry*>::Remove (find, shift down, --count), which the old comment called CBlendList::Add;
//   RemoveAllBlends 0x00C1E800 calls this function until both counts reach zero.
// Callees: CBlendKey::GetIndex @ 0x00C1BA90 (on &entry->m_key, returns long); CPtrArray<CBlendEntry*>::Remove
//   @ 0x00C1E900 (thiscall, one arg).
// Shape: written as if/else: VC hoists the common `push` of the argument above the branch and merges the two Remove
//   calls, leaving only the `lea` of the list selected (a conditional expression on the list reference pushes after
//   the select).
struct CBlendKey {
    long GetIndex();                       // 0x00C1BA90
};

struct CBlendEntry {
    unsigned char _pad[0x8];
    CBlendKey m_key;                       // +8
};

template <class T>
struct CPtrArray {
    T* m_data;                             // +0
    unsigned long m_capacity;              // +4
    unsigned long m_count;                 // +8
    void Remove(T value);                  // 0x00C1E900 for CBlendEntry*
};

struct CBlendOwner {
    void* m_pOwner;                        // +0x00
    long m_4;                              // +0x04
    CPtrArray<CBlendEntry*> m_unkeyed;     // +0x08
    CPtrArray<CBlendEntry*> m_keyed;       // +0x14
    void RemoveBlend(CBlendEntry* entry);
};

void CBlendOwner::RemoveBlend(CBlendEntry* entry)
{
    if (entry->m_key.GetIndex() == -1)
        m_unkeyed.Remove(entry);
    else
        m_keyed.Remove(entry);
}
