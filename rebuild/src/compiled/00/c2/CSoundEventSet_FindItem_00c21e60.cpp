// CSoundEventSet::FindItem(unsigned long key) -- peek the first event of the set's pending queue and return it only when its +0x14 value is below the key (else null)
// Leaf: ?FindItem@CSoundEventSet@@QAEPAUCSoundEvent@@K@Z
// Naming (sweep1, 2026-10-25): libpk class set CSoundEventManager (vtable 0x012B2748) / CSoundEventSet (its +0x2C
//   member) / CSoundEvent (0xB4 bytes, vtable 0x012B3674): the describer 0x00C21040 called by TraceEvents/
//   TracePlaybacks prints "EventId(", "Priority=", "State='Waiting for data'" / "Playing using physical player";
//   chunk 431/435's CPanel/CPanelItem names came from propagated UI labels (renamed from 00/c2/CPanelItemSet_FindItem_00c21e60.cpp).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C21E60, 33 bytes (`ret 4`). Descriptive names (no PDB covers this middleware
//   library): the manifest's "CTCInventoryItem_GetFirstAvailableIndex" is propagated. Its only caller is
//   CSoundEventManager::OnKey 0x00C11F00 (chunk 431: `event = m_eventSet.FindItem(key); if (event) event->Activate(this);`), so
//   `this` is the manager's event set at +0x2C and the result is a CSoundEvent.
// Callees: the pending queue member object at +0x198 (manager +0x1C4), virtual slot 11 (+0x2C): its first event or null.
// Shape: the queue is a member OBJECT (`mov eax,[ecx+198h] ; add ecx,198h ; call [eax+2Ch]`); the unsigned compare
//   `event->m_key < key` jumps straight to the shared `ret 4` with the event still in eax, the null and the
//   not-below cases share the `xor eax,eax`.

struct CSoundEvent {                             // 0xB4-byte state-machine event
    unsigned char _pad00[0x14];
    unsigned long m_key;                        // +0x14
};

struct CSoundEventQueue {                        // polymorphic queue member of the event set
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10();
    virtual CSoundEvent* GetFirst();             // slot 11 (+0x2C)
};

struct CSoundEventSet {                          // +0x2C of the manager
    unsigned char   _pad00[0x198];
    CSoundEventQueue m_pending;                  // +0x198
    CSoundEvent* FindItem(unsigned long key);
};

CSoundEvent* CSoundEventSet::FindItem(unsigned long key)
{
    CSoundEvent* event = m_pending.GetFirst();
    if (event && event->m_key < key)
        return event;
    return 0;
}