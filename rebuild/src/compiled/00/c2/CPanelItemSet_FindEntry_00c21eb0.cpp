// CPanelItemSet::FindEntry(unsigned long key) -- peek the first entry of the set's second queue and return it when the key is "no limit" (-1) or the entry's +0x1C value is below it (else null)
// Leaf: FindEntry
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C21EB0, 38 bytes (`ret 4`). Descriptive names (no PDB covers this middleware
//   library): the manifest's "CTCInventoryBase_GetFirstAvailableIndex" is propagated. Its only caller 0x00C10C90 (a
//   panel member: `this` = the item set at panel +0x2C) hands the found entry's +4 item to 0x00C216D0 with the panel.
// Callees: the queue member object at +0x228 (panel +0x254), virtual slot 11 (+0x2C): its first entry or null.
// Shape: `cmp ecx,-1 ; jae` is an unsigned `key >= 0xFFFFFFFF` (written `== 0xFFFFFFFF` it becomes `je`), the entry compare `entry->m_key < key`
//   (`cmp [eax+1Ch],ecx ; jb`); both successful tests jump to the shared `ret 4` with the entry in eax.

struct CPanelItem;

struct CPanelEntry {
    unsigned char _pad00[4];
    CPanelItem*   m_pItem;                      // +0x04
    unsigned char _pad08[0x1c - 0x08];
    unsigned long m_key;                        // +0x1C
};

struct CPanelEntryQueue {                       // polymorphic queue member of the item set
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10();
    virtual CPanelEntry* GetFirst();            // slot 11 (+0x2C)
};

struct CPanelItemSet {                          // +0x2C of the panel
    unsigned char    _pad00[0x228];
    CPanelEntryQueue m_entries;                 // +0x228
    CPanelEntry* FindEntry(unsigned long key);
};

CPanelEntry* CPanelItemSet::FindEntry(unsigned long key)
{
    CPanelEntry* entry = m_entries.GetFirst();
    if (entry && (key >= 0xFFFFFFFF || entry->m_key < key))
        return entry;
    return 0;
}