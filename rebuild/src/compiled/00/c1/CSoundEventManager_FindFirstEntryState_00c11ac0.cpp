// CSoundEventManager::FindFirstEntryState(void* tag, const SObjectKey_00c196a0* key) -- vtable 0x012B2748 slot 17: under the module lock, resolve the key through the owner's object map, then walk the found object's entry table (group by group, entry by entry) and return the first non-negative state slot 18 reports for (tag, entry); -1 when the key resolves to nothing or no entry has a state
// Leaf: ?FindFirstEntryState@CSoundEventManager@@UAEJPAXPBUSObjectKey_00c196a0@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C11AC0, 320 bytes (no padding after: the next function starts at once), thiscall
//   `ret 8`, EH-framed (one state: the lock).
// Identity: the only reference is vtable 0x012B2748 slot 17 (0x012B278C) -- the libpk sound-event manager of the landed
//   sweep1 class set (CSoundEventManager: ~CSoundEventManager 0x00C13470 stores that vtable; DispatchToEntry slot 24,
//   CountWaitingEvents slot 30, CountPlayingEvents slot 31). libpk has no PDB or RTTI: the method name is descriptive. The
//   catalog's "Combat_IterateAnimationsForCombo" is propagated.
// Callees: PKCProtectionInstance ctor/dtor 0x00C0FFC0 / 0x00C0FFE0 over g_lock_0143bb14 (0x0143BB14),
//   CObjectMap_00c196a0::Find 0x00C196A0 (the landed spelling; the map pointer sits at +0x68 of the manager's +4 owner),
//   the found object's virtual slot 5 (its entry table) and the table's slots 4 / 5 / 6 (entry count of a group, group
//   count, entry (group, index)), and this manager's own slot 18 0x00C119D0 (state of (tag, entry); landed under the
//   descriptive CCombatStateTable_00c119d0::ProcessAnimationState).
// Shape: the getters are virtual and re-called every iteration (as retail); unsigned counters; three exits (none, a state,
//   exhausted), each destroying the lock. The entry is a named local: as a nested argument VC7.1 loads this manager's
//   vtable before the GetEntry call and keeps `this` only in the frame (321v320); the parameter order (tag, key) is retail's
//   (Find gets the second argument, slot 18 the first). Virtual slots whose meaning is unknown are named UnprovenSlotNN.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;          // 0x0143BB14: the module's critical section

class PKCProtectionInstance {
public:                                             // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                            // 0x00C0FFE0
};

struct CEntry_00c119d0;                             // the entry type slot 18 takes (landed 0x00C119D0)

struct CUnprovenEntryTable_00C11AC0 {               // the object's entry table (no evidence of its class)
    virtual void UnprovenSlot00();
    virtual void UnprovenSlot01();
    virtual void UnprovenSlot02();
    virtual void UnprovenSlot03();
    virtual unsigned long GetEntryCount(unsigned long group);                    // slot 4
    virtual unsigned long GetGroupCount();                                       // slot 5
    virtual CEntry_00c119d0* GetEntry(unsigned long group, unsigned long index); // slot 6
};

struct CObject_00c196a0 {                           // what CObjectMap_00c196a0::Find returns (landed spelling)
    virtual void UnprovenSlot00();
    virtual void UnprovenSlot01();
    virtual void UnprovenSlot02();
    virtual void UnprovenSlot03();
    virtual void UnprovenSlot04();
    virtual CUnprovenEntryTable_00C11AC0* GetEntryTable();                      // slot 5
};

struct SObjectKey_00c196a0;
struct CObjectMap_00c196a0 {
    CObject_00c196a0* Find(const SObjectKey_00c196a0* key);                     // 0x00C196A0
};

struct CUnprovenOwner_00C11AC0 {                    // the manager's +4 owner
    unsigned char _pad[0x68];
    CObjectMap_00c196a0* m_pObjectMap;              // +0x68
};

struct CSoundEventManager {                         // vtable 0x012B2748 (libpk, no RTTI)
    virtual void UnprovenSlot00();
    virtual void UnprovenSlot01();
    virtual void UnprovenSlot02();
    virtual void UnprovenSlot03();
    virtual void UnprovenSlot04();
    virtual void UnprovenSlot05();
    virtual void UnprovenSlot06();
    virtual void UnprovenSlot07();
    virtual void UnprovenSlot08();
    virtual void UnprovenSlot09();
    virtual void UnprovenSlot10();
    virtual void UnprovenSlot11();
    virtual void UnprovenSlot12();
    virtual void UnprovenSlot13();
    virtual void UnprovenSlot14();
    virtual void UnprovenSlot15();
    virtual void UnprovenSlot16();
    virtual long FindFirstEntryState(void* tag, const SObjectKey_00c196a0* key);   // slot 17 (this row)
    virtual long GetEntryState(void* tag, CEntry_00c119d0* entry);                 // slot 18 0x00C119D0
    CUnprovenOwner_00C11AC0* m_pOwner;              // +0x04
};

long CSoundEventManager::FindFirstEntryState(void* tag, const SObjectKey_00c196a0* key)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    CObject_00c196a0* object = m_pOwner->m_pObjectMap->Find(key);
    if (object == 0)
        return -1;
    for (unsigned long group = 0; group < object->GetEntryTable()->GetGroupCount(); ++group) {
        for (unsigned long index = 0; index < object->GetEntryTable()->GetEntryCount(group); ++index) {
            CEntry_00c119d0* entry = object->GetEntryTable()->GetEntry(group, index);
            long state = GetEntryState(tag, entry);
            if (state >= 0)
                return state;
        }
    }
    return -1;
}