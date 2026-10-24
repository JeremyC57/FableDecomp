// CEntryOwner_00c14ad0::ReleaseTarget(void* target) -- removes every entry the registry at +0x40 still finds for the target from its own list (the entry's +4 owner), re-querying after each removal
// Leaf: ReleaseTarget
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C14AD0, 67 bytes (+ int3 padding), __fastcall `ret 4`.
// Identity: a libpk manager; its owner calls it right before the sound-event manager's ReleaseTarget (0x00C12140) with
//   the same target (0x00C0897E: `mov ecx,[edi+64h] ; test ecx,ecx ; je ; push esi ; call`). The registry at +0x40
//   is polymorphic: slot 17 (+0x44) finds an entry by the address of a key word -- the interface the landed
//   0x00C12140 uses at +0x284. Callee: the entry list's Remove @ 0x00C2C8F0 (lock-guarded).
// Shape: the finder is an inline helper taking the key BY VALUE (`FindEntry(target)`): each inlined call copies the
//   key into a fresh frame word whose address goes to slot 17 -- the first into the dead argument slot, the second into
//   the `push ecx` word, re-stored from edi every pass; the registry address stays in esi.
// Label correction: the catalog's `CTCHero_ProcessAllCombatBlockParryEvents` is propagated; descriptive names.
struct CEntry_00c14ad0;

struct CEntryList_00c14ad0 {
    void Remove(CEntry_00c14ad0* entry);                       // 0x00C2C8F0
};

struct CEntry_00c14ad0 {
    long m_0;
    CEntryList_00c14ad0* m_pList;                              // +0x04
};

class CRegistry_00c14ad0 {                                     // +0x40: slot 17 finds an entry by key
public:
    virtual void Slot00(); virtual void Slot04(); virtual void Slot08(); virtual void Slot0C();
    virtual void Slot10(); virtual void Slot14(); virtual void Slot18(); virtual void Slot1C();
    virtual void Slot20(); virtual void Slot24(); virtual void Slot28(); virtual void Slot2C();
    virtual void Slot30(); virtual void Slot34(); virtual void Slot38(); virtual void Slot3C();
    virtual void Slot40();
    virtual CEntry_00c14ad0* Find(void* const* key);           // slot 17 (+0x44)
    CEntry_00c14ad0* FindEntry(void* key) { return Find(&key); }
};

struct CEntryOwner_00c14ad0 {
    unsigned char _pad[0x40];
    CRegistry_00c14ad0 m_registry;                             // +0x40
    void ReleaseTarget(void* target);
};

void CEntryOwner_00c14ad0::ReleaseTarget(void* target)
{
    for (CEntry_00c14ad0* entry = m_registry.FindEntry(target); entry; entry = m_registry.FindEntry(target))
        entry->m_pList->Remove(entry);
}