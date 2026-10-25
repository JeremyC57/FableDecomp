// CSoundEventManager::UpdateItems() -- walk the manager's event pool (+0x5c, count +0x60): every active event that is not audible (CSoundEvent::IsAudible 0x00C20C10) is stopped (CSoundEvent::Stop 0x00C21620)
// Leaf: ?UpdateItems@CSoundEventManager@@QAEXXZ
// Naming (sweep1, 2026-10-25): libpk class set CSoundEventManager (vtable 0x012B2748) / CSoundEventSet (its +0x2C
//   member) / CSoundEvent (0xB4 bytes, vtable 0x012B3674): the describer 0x00C21040 called by TraceEvents/
//   TracePlaybacks prints "EventId(", "Priority=", "State='Waiting for data'" / "Playing using physical player";
//   chunk 431/435's CPanel/CPanelItem names came from propagated UI labels (renamed from 00/c1/CPanel_UpdateItems_00c11cb0.cpp).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C11CB0, 72 bytes (`ret`). Descriptive names (no PDB covers this library): the
//   manifest's "CTCInventory_ProcessKeyPress" is propagated. Callers: OnKey 0x00C11F00 (in this chunk) and the two
//   lock-guarded refreshers 0x00C12720 / 0x00C127C0, which then walk the same array themselves.
// Callees: CSoundEventPool::operator[] @ 0x00C13620 (bounds-checked; element size 0xB4), CSoundEvent::IsActive
//   @ 0x00C20840, CSoundEvent::IsAudible @ 0x00C20C10, CSoundEvent::Stop @ 0x00C21620.
// Shape: the count is re-read every pass (`mov eax,[edi+60h] ; inc ebx ; cmp ebx,eax ; jb`), the array address kept
//   in ebp, the event in esi.
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
    void UpdateItems();
};

void CSoundEventManager::UpdateItems()
{
    for (unsigned long i = 0; i < m_eventSet.m_events.m_count; ++i) {
        CSoundEvent& event = m_eventSet.m_events[i];
        if (event.IsActive()) {
            if (!event.IsAudible(this))
                event.Stop(this);
        }
    }
}