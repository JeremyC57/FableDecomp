// CSoundEventSet::FindEntry(unsigned long key) -- peek the first entry of the set's second queue and return it when the key is "no limit" (-1) or the entry's +0x1C value is below it (else null)
// Leaf: ?FindEntry@CSoundEventSet@@QAEPAUCSoundEventEntry@@K@Z
// Naming (sweep1, 2026-10-25): libpk class set CSoundEventManager (vtable 0x012B2748) / CSoundEventSet (its +0x2C
//   member) / CSoundEvent (0xB4 bytes, vtable 0x012B3674): the describer 0x00C21040 called by TraceEvents/
//   TracePlaybacks prints "EventId(", "Priority=", "State='Waiting for data'" / "Playing using physical player";
//   chunk 431/435's CPanel/CPanelItem names came from propagated UI labels (renamed from 00/c2/CPanelItemSet_FindEntry_00c21eb0.cpp).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C21EB0, 38 bytes (`ret 4`). Descriptive names (no PDB covers this middleware
//   library): the manifest's "CTCInventoryBase_GetFirstAvailableIndex" is propagated. Its only caller 0x00C10C90 (a
//   manager member: `this` = the event set at manager +0x2C) hands the found entry's +4 event to 0x00C216D0 with the manager.
// Callees: the queue member object at +0x228 (manager +0x254), virtual slot 11 (+0x2C): its first entry or null.
// Shape: `cmp ecx,-1 ; jae` is an unsigned `key >= 0xFFFFFFFF` (written `== 0xFFFFFFFF` it becomes `je`), the entry compare `entry->m_key < key`
//   (`cmp [eax+1Ch],ecx ; jb`); both successful tests jump to the shared `ret 4` with the entry in eax.

struct CSoundEvent;

struct CSoundEventEntry {
    unsigned char _pad00[4];
    CSoundEvent*   m_pEvent;                      // +0x04
    unsigned char _pad08[0x1c - 0x08];
    unsigned long m_key;                        // +0x1C
};

struct CSoundEventEntryQueue {                       // polymorphic queue member of the event set
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10();
    virtual CSoundEventEntry* GetFirst();            // slot 11 (+0x2C)
};

struct CSoundEventSet {                          // +0x2C of the manager
    unsigned char    _pad00[0x228];
    CSoundEventEntryQueue m_entries;                 // +0x228
    CSoundEventEntry* FindEntry(unsigned long key);
};

CSoundEventEntry* CSoundEventSet::FindEntry(unsigned long key)
{
    CSoundEventEntry* entry = m_entries.GetFirst();
    if (entry && (key >= 0xFFFFFFFF || entry->m_key < key))
        return entry;
    return 0;
}