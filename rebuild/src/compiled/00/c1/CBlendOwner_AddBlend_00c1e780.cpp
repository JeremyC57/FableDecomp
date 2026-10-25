// CBlendOwner::AddBlend(CBlendEntry* entry) -- files the entry at the end of the unkeyed list (+8) when its key resolves to -1, else of the keyed list (+0x14), growing the list by one slot when it is full
// Leaf: AddBlend
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1E780, 66 bytes (`ret 4`). Descriptive name (no PDB covers this library): the
//   manifest's "CUIEventQueue_EnqueueKeyInput" is propagated. The add counterpart of RemoveBlend 0x00C1E7D0 (same key
//   test, same two lists); caller 0x00C2E5FE.
// Callees: CBlendKey::GetIndex @ 0x00C1BA90 (on &entry->m_key), the array's Grow @ 0x00C1E640 (thiscall, slot count).
// Shape: written as if/else with the array's inline Add in both arms: VC7.1 merges the two inlined copies behind a
//   selected list pointer (`add esi,8` / `add esi,14h`), and only the merged form gives retail's `mov ecx,esi ; push 1`
//   and the data-before-count loads (a `?:` list pointer or reference schedules both the other way). The full test is
//   `capacity - count == 0` (`mov ; sub ; jne`); the count is re-read for the increment.
struct CBlendKey {
    long GetIndex();                        // 0x00C1BA90
};

struct CBlendEntry {
    unsigned char _pad[0x8];
    CBlendKey m_key;                        // +8
};

template <class T>
struct CPtrArray {
    T* m_data;                              // +0
    unsigned long m_capacity;               // +4
    unsigned long m_count;                  // +8
    void Grow(unsigned long slots);         // 0x00C1E640
    void Add(T value)
    {
        if (m_capacity - m_count == 0)
            Grow(1);
        m_data[m_count] = value;
        ++m_count;
    }
};

struct CBlendOwner {
    void* m_pOwner;                         // +0x00
    long m_4;                               // +0x04
    CPtrArray<CBlendEntry*> m_unkeyed;      // +0x08
    CPtrArray<CBlendEntry*> m_keyed;        // +0x14
    void AddBlend(CBlendEntry* entry);
};

void CBlendOwner::AddBlend(CBlendEntry* entry)
{
    if (entry->m_key.GetIndex() == -1)
        m_unkeyed.Add(entry);
    else
        m_keyed.Add(entry);
}