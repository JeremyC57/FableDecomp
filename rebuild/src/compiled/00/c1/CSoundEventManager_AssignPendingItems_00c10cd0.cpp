// CSoundEventManager::AssignPendingItems() -- hand each event of the list at +0x164 (events waiting for a physical player) a free slot of the manager's event set until either runs out, then place the events of the list at +0x194 (waiting for memory) until one refuses (negative result)
// Leaf: ?AssignPendingItems@CSoundEventManager@@QAEXXZ
// Naming (sweep1, 2026-10-25): libpk class set CSoundEventManager (vtable 0x012B2748) / CSoundEventSet (its +0x2C
//   member) / CSoundEvent (0xB4 bytes, vtable 0x012B3674): the describer 0x00C21040 called by TraceEvents/
//   TracePlaybacks prints "EventId(", "Priority=", "State='Waiting for data'" / "Playing using physical player";
//   chunk 431/435's CPanel/CPanelItem names came from propagated UI labels (renamed from 00/c1/CPanel_AssignPendingItems_00c10cd0.cpp).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C10CD0, 112 bytes (`ret`). Descriptive names (no PDB covers this library): the
//   manifest's "NUISystem_CSpellContainerList_PopulateSpells" is propagated. Callers 0x00C1137C, 0x00C11FD0, 0x00C128FA;
//   the event set at +0x2c is the one of OnKey 0x00C11F00 / UpdateItems 0x00C11CB0.
// Callees: the typed lists' GetNext (virtual slot 12, +0x30), CSoundEventSet::GetFreeSlot @ 0x00C21E50 (forwards to
//   the set's +0x1f8 object, virtual slot 1), CSoundEvent::Assign @ 0x00C21750, CSoundEvent::Place @ 0x00C219F0.
// Shape: the first loop is rotated (entry in esi, list in edi, set in ebp pushed only around it); the second keeps the
//   entry in eax and jumps back to one shared `test eax,eax`: a plain `while (placed)`, `for (;;)` or `for` with a
//   `return` all rotate it; the top-tested form needs the `__forceinline` early-return condition helper IsEntry.
struct CSoundEventManager;

struct CSoundEvent {                               // 0xB4 bytes (vtable 0x012B3674)
    void Assign(CSoundEventManager* manager, void* slot);     // 0x00C21750 (events waiting for a physical player)
    long Place(CSoundEventManager* manager);                  // 0x00C219F0 (events waiting for memory)
};

struct CSoundEventList {                           // typed event list over an implementation object (+0x164 waiting
                                                   //   for a physical player, +0x194 waiting for memory: agent434)
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3(); virtual void Slot4();
    virtual void Slot5(); virtual void Slot6(); virtual void Slot7(); virtual void Slot8(); virtual void Slot9();
    virtual void Slot10(); virtual void Slot11();
    virtual CSoundEvent* GetNext();           // slot 12 (+0x30)
    void* m_pImpl;                              // +4
    unsigned char m_impl[8];                    // +8
};

static __forceinline bool IsEntry(const CSoundEvent* entry)
{
    if (entry == 0)
        return false;
    return true;
}

struct CSoundEventSet {                          // +0x2c of the manager
    void* GetFreeSlot();                        // 0x00C21E50
};

struct CSoundEventManager {
    unsigned char _pad00[0x2c];
    CSoundEventSet m_eventSet;                    // +0x2c
    unsigned char _pad2d[0x164 - 0x2d];
    CSoundEventList m_pending;                     // +0x164
    unsigned char _pad174[0x194 - 0x174];
    CSoundEventList m_placed;                       // +0x194
    void AssignPendingItems();
};

void CSoundEventManager::AssignPendingItems()
{
    CSoundEvent* pending = m_pending.GetNext();
    while (pending) {
        void* slot = m_eventSet.GetFreeSlot();
        if (!slot)
            break;
        pending->Assign(this, slot);
        pending = m_pending.GetNext();
    }
    CSoundEvent* placed = m_placed.GetNext();
    while (IsEntry(placed)) {
        if (placed->Place(this) < 0)
            break;
        placed = m_placed.GetNext();
    }
}