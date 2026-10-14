// CEntryOwner_00c12000::ApplyEntries() -- for every entry of the checked array at +0x224 (count at +0x228): if the entry's +4 pointer is set, apply the entry to this
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C12000, length 61 bytes.
// Callees: CCheckedArray::Get(unsigned long) @ 0x00C13700 (bounds-checked element access, EH-framed);
// CEntry::Apply(owner) @ 0x00C27540 (an incremental-link jump thunk to 0x00C271E0).
// The manifest label (C3DMesh2::CalculateTextureChangeFlags) is propagated and does not fit
// (C3DMesh2 is 0xf4 bytes); the callee labels are propagated too. The array object's address
// is formed once outside the loop (`lea ebx,[edi+0x224]`) and its count re-read each pass.
struct CEntryOwner_00c12000;

struct CEntry {
    void* m_vptr;
    void* m_pTarget;                       // +4
    void Apply(CEntryOwner_00c12000* owner);   // 0x00C27540
};

struct CCheckedArray {
    CEntry* m_pData;                       // +0
    unsigned long m_count;                 // +4
    CEntry* Get(unsigned long index);      // 0x00C13700
};

struct CEntryOwner_00c12000 {
    unsigned char _pad[0x224];
    CCheckedArray m_entries;               // +0x224
    void ApplyEntries();
};

void CEntryOwner_00c12000::ApplyEntries()
{
    for (unsigned long i = 0; i < m_entries.m_count; ++i) {
        CEntry* entry = m_entries.Get(i);
        if (entry->m_pTarget)
            entry->Apply(this);
    }
}