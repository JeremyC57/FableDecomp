// CSoundEventManager::OnKey(unsigned long key) -- unless the manager's event set is busy: update the events, and if it is still not busy, stop the event the set finds below the key (CSoundEventSet::FindItem), then poll the set again
// Leaf: ?OnKey@CSoundEventManager@@QAEXK@Z
// Naming (sweep1, 2026-10-25): libpk class set CSoundEventManager (vtable 0x012B2748) / CSoundEventSet (its +0x2C
//   member) / CSoundEvent (0xB4 bytes, vtable 0x012B3674): the describer 0x00C21040 called by TraceEvents/
//   TracePlaybacks prints "EventId(", "Priority=", "State='Waiting for data'" / "Playing using physical player";
//   chunk 431/435's CPanel/CPanelItem names came from propagated UI labels (renamed from 00/c1/CPanel_OnKey_00c11f00.cpp).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C11F00, 72 bytes (`ret 4`). Descriptive names (no PDB covers this library):
//   the manifest's "CTCHero_OnInventoryKeyPressed" is propagated. The only caller is 0x00C12A84.
// Callees: CSoundEventSet::IsBusy @ 0x00C21DF0 (three times; the last result is dropped), CSoundEventManager::UpdateItems
//   @ 0x00C11CB0, CSoundEventSet::FindItem @ 0x00C21E60, CSoundEvent::Stop @ 0x00C21620.
// Shape: the event set address is kept in esi, `this` in edi; both busy exits share the epilogue.
struct CSoundEventManager;

struct CSoundEvent {                             // 0xb4-byte element (state word at +0xac)
    long IsActive() const;                      // 0x00C20840 (`state != 0`, returned as a full register)
    bool IsAudible(CSoundEventManager* manager);  // 0x00C20C10 (bool: retail tests al, though the callee sets eax)
    void Stop(CSoundEventManager* manager);               // 0x00C21620
};

struct CSoundEventPool {                        // polymorphic array: count at +4, elements at +0x10
    void* m_vtable;                             // +0x00
    unsigned long m_count;                      // +0x04
    unsigned char _pad08[0x10 - 0x08];
    CSoundEvent* m_pEvents;                       // +0x10
    CSoundEvent& operator[](unsigned long index);   // 0x00C13620 (bounds-checked, EH-framed)
};

struct CSoundEventSet {                          // +0x2c of the manager
    unsigned char _pad00[0x30];
    CSoundEventPool m_events;                    // +0x30 (manager +0x5c)
    long IsBusy();                              // 0x00C21DF0 (forwards to m_events virtual slot 1)
    CSoundEvent* FindItem(unsigned long key);    // 0x00C21E60
};

struct CSoundEventManager {
    unsigned char _pad00[0x2c];
    CSoundEventSet m_eventSet;                    // +0x2c
    void UpdateItems();                         // 0x00C11CB0
    void OnKey(unsigned long key);
};

void CSoundEventManager::OnKey(unsigned long key)
{
    if (!m_eventSet.IsBusy()) {
        UpdateItems();
        if (!m_eventSet.IsBusy()) {
            CSoundEvent* event = m_eventSet.FindItem(key);
            if (event)
                event->Stop(this);
            m_eventSet.IsBusy();
        }
    }
}