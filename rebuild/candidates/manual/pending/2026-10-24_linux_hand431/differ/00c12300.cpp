// CPanel::GetItemStatistics(CPanelItemStats* stats) -- under the library lock, zero the five per-state counters, let every active item of the item array count itself into them, and copy the panel's +0x370 value into the sixth field
// Grade: DIFFER(165v165) -- pass 3: the walk through inline owner accessors GetNumItems()/GetItem(i) (the landed 0x00C123B0/0x00C12720 recipe) gives retail's edi index / late ebx item; left: retail reads the +0x370 value into the dead `this` register ebp (`mov ebp,[ebp+370h] ; mov [esi+14h],ebp`), the lane into eax (float type tried, same)
// Flags: /EHsc
// Leaf: GetItemStatistics
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C12300, 165 bytes (`ret 4`). Built under /EHsc. Descriptive names (no PDB
//   covers this library): the manifest's "Inventory_PopulateItemArray" is propagated. The panel and its item array
//   (+0x5c, count +0x60, 0xB4-byte items) are the ones of UpdateItems 0x00C11CB0 and OnKey 0x00C11F00.
// Callees: CScopedLock::CScopedLock @ 0x00C0FFC0 / ~CScopedLock @ 0x00C0FFE0 over the global lock @ 0x0143BB14,
//   CPanelItemArray::operator[] @ 0x00C13620, CPanelItem::IsActive @ 0x00C20840, CPanelItem::CountInto @ 0x00C20F60
//   (a jump table over the item state 2..6 incrementing one of the five counters).
// Tried: a pointer local for the item, a named local for the +0x370 value, an inline Reset() for the five zero
//   stores (all identical output), operator[] at each use (170 bytes). The retail sibling 0x00C12720 has the same
//   edi-index / late-ebx colouring.
// Shape: the zero register edi doubles as the loop index; the five counters are cleared in retail order (+0xc, +0x10,
//   +4, +8, +0); all callee-saved registers are busy, so the array address is re-formed (`lea ecx,[ebp+5Ch]`) per pass.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;       // 0x0143BB14

struct CScopedLock {                            // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);     // 0x00C0FFC0
    ~CScopedLock();                             // 0x00C0FFE0
};

struct CPanelItemStats {                        // 0x18 bytes
    long m_count0;                              // +0x00
    long m_count1;                              // +0x04
    long m_count2;                              // +0x08
    long m_count3;                              // +0x0c
    long m_count4;                              // +0x10
    long m_total;                               // +0x14
};

struct CPanelItem {                             // 0xb4-byte element (state word at +0xac)
    long IsActive();                            // 0x00C20840
    void CountInto(CPanelItemStats* stats);     // 0x00C20F60
};
struct CPanelItemArray {                        // +0x5c: the item pool, count at +4
    long m_0;
    unsigned long m_count;                      // +4 (= +0x60)
    CPanelItem* Get(unsigned long index);       // 0x00C13620 (bounds-checked, EH-framed)
};
struct CPanel {
    unsigned char _pad00[0x5c];
    CPanelItemArray m_items;                    // +0x5c
    unsigned char _pad64[0x370 - 0x64];
    long m_total;                               // +0x370
    unsigned long GetNumItems() { return m_items.m_count; }
    CPanelItem* GetItem(unsigned long index) { return m_items.Get(index); }
    void GetItemStatistics(CPanelItemStats* stats);
};
void CPanel::GetItemStatistics(CPanelItemStats* stats)
{
    CScopedLock lock(&g_lock_0143bb14);
    stats->m_count3 = 0;
    stats->m_count4 = 0;
    stats->m_count1 = 0;
    stats->m_count2 = 0;
    stats->m_count0 = 0;
    for (unsigned long i = 0; i < GetNumItems(); ++i) {
        CPanelItem* item = GetItem(i);
        if (item->IsActive())
            item->CountInto(stats);
    }
    stats->m_total = m_total;
}
