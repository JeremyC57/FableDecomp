// CTCParticleAttacher::HandleMessage(CResourceEntry*) -- destroys an entry, then destroys its owning map when the map reports itself empty (catalogue label, class unknown)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A5C0, length 58 bytes. The label is propagated; callers @ 0x00C2A66A /
// 0x00C2A6E8. `this` is never read (`ret 4`).
// Callees: the entry's destructor @ 0x00C36590 and the owner's destructor @ 0x00C37130 (labels "Fastcall_ResourceMapClear" /
// "Fastcall_ResourceMapDestructor"), operator delete @ 0x00BFE9BC (twice), the owner's embedded interface at +0x10, virtual
// slot 15 (+0x3c, long).
// Shape: the owner (the entry's first word) is read into edi before the entry dies; each `delete` is written out as the
// destructor call plus operator delete (no null test, `add esp,4` after each).
void __cdecl operator delete(void* block);          // 0x00BFE9BC

struct CEntryList_00c2a5c0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual long IsInUse();                         // slot 15 (+0x3c)
};

struct CResourceOwner_00c2a5c0 {
    unsigned char       _pad00[0x10];
    CEntryList_00c2a5c0 m_entries;                  // +0x10
    ~CResourceOwner_00c2a5c0();                     // 0x00C37130
};

struct CResourceEntry_00c2a5c0 {
    CResourceOwner_00c2a5c0* m_pOwner;              // +0x00
    ~CResourceEntry_00c2a5c0();                     // 0x00C36590
};

struct CTCParticleAttacher {
    void HandleMessage(CResourceEntry_00c2a5c0* entry);
};

void CTCParticleAttacher::HandleMessage(CResourceEntry_00c2a5c0* entry)
{
    CResourceOwner_00c2a5c0* owner = entry->m_pOwner;
    entry->~CResourceEntry_00c2a5c0();
    operator delete(entry);
    if (owner->m_entries.IsInUse() == 0) {
        owner->~CResourceOwner_00c2a5c0();
        operator delete(owner);
    }
}