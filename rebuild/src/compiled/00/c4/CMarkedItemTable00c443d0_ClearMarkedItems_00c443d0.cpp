// CMarkedItemTable_00c443d0::ClearMarkedItems() -- releases every marked entry (dropping its mark count), clears its slot and paired record, then resets the table's bookkeeping words
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C443D0, length 83 bytes (the 96-byte catalog row ends in int3 padding).
// Callees: the entry's mark release @ 0x00C3C5F0 (`if (m_marks) --m_marks;`, manifest label
//          "CItem::SetQuestPersistentFlag" is propagated).
// Label: the manifest's "CTCInventory::ClearMarkedItems" module is propagated (this is the no-RTTI 0x00C4xxxx
//        module; callers 0x00C437D1, 0x00C43821, 0x00C4455A); the leaf name is kept, the class is descriptive.
// Layout: +0x00 entry pointer array, +0x04 8-byte records (one __int64 each), +0x08 count, +0x0C..+0x20
//         bookkeeping words with the entry being released published at +0x14.
// Shape: the slot is indexed at every use (`m_ppEntries[i]`): VC CSEs the slot address into eax
//        (`lea eax,[eax+edi*4]`) and re-reads it for the call after the m_pCurrent store; a reference
//        local strength-reduces `i*4` into a fourth register instead. The record is cleared by ONE
//        __int64 store (base loaded once); two 32-bit member stores re-read m_pRecords in between.

struct CMarkedEntry {
    void ReleaseMark();                           // 0x00C3C5F0
};

struct CMarkedItemTable_00c443d0 {
    CMarkedEntry** m_ppEntries;                   // +0x00
    __int64*       m_pRecords;                    // +0x04
    unsigned long  m_count;                       // +0x08
    long           m_markedCount;                 // +0x0C
    long           m_markedBytes;                 // +0x10
    CMarkedEntry*  m_pCurrent;                    // +0x14
    long           m_first;                       // +0x18
    long           m_last;                        // +0x1C
    long           m_cursor;                      // +0x20
    void ClearMarkedItems();
};

void CMarkedItemTable_00c443d0::ClearMarkedItems()
{
    for (unsigned long i = 0; i < m_count; ++i) {
        if (m_ppEntries[i] != 0) {
            m_pCurrent = m_ppEntries[i];
            m_ppEntries[i]->ReleaseMark();
            m_ppEntries[i] = 0;
            m_pRecords[i] = 0;
        }
    }
    m_markedCount = 0;
    m_markedBytes = 0;
    m_first = 0;
    m_last = 0;
    m_cursor = 0;
    m_pCurrent = 0;
}