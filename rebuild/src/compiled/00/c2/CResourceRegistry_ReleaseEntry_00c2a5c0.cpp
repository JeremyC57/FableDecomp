// CResourceRegistry::ReleaseEntry(CResourceEntry* entry) -- destroys an entry, then destroys its owning resource map when the map's entry list reports itself empty
// Leaf: ?ReleaseEntry@CResourceRegistry@@QAEXPAUCResourceEntry@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A5C0, length 58 bytes. `this` is never read (`ret 4`).
// Naming (sweep1, 2026-10-25): was landed as CTCParticleAttacher::HandleMessage (a propagated manifest label);
//   descriptive (no PDB covers libpk). FINDINGS chunk 436: both callers are CResourceRegistry members -- the landed
//   CResourceRegistry::ReleaseOwnerEntries 0x00C2A640 (0x00C2A66A: ReleaseEntry on every entry its owner list yields)
//   and 0x00C2A6E8 -- beside SetEntryValue 0x00C2A300 / HasEntry 0x00C2A320.
// Callees: the entry's destructor @ 0x00C36590 and the owner's destructor @ 0x00C37130 (labels "Fastcall_ResourceMapClear" /
// "Fastcall_ResourceMapDestructor"), operator delete @ 0x00BFE9BC (twice), the owner's embedded interface at +0x10, virtual
// slot 15 (+0x3c, long).
// Shape: the owner (the entry's first word) is read into edi before the entry dies; each `delete` is written out as the
// destructor call plus operator delete (no null test, `add esp,4` after each).
void __cdecl operator delete(void* block);          // 0x00BFE9BC

struct CEntryList {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual long IsInUse();                         // slot 15 (+0x3c)
};

struct CResourceMap {
    unsigned char _pad00[0x10];
    CEntryList    m_entries;                        // +0x10
    ~CResourceMap();                                // 0x00C37130
};

struct CResourceEntry {
    CResourceMap* m_pOwner;                         // +0x00
    ~CResourceEntry();                              // 0x00C36590
};

class CResourceRegistry {
public:
    void ReleaseEntry(CResourceEntry* entry);
};

void CResourceRegistry::ReleaseEntry(CResourceEntry* entry)
{
    CResourceMap* owner = entry->m_pOwner;
    entry->~CResourceEntry();
    operator delete(entry);
    if (owner->m_entries.IsInUse() == 0) {
        owner->~CResourceMap();
        operator delete(owner);
    }
}
