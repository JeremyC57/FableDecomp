// CScriptHost::ClearEvents() -- empty the host's event list at +0x60: take its first record, remove it from the list and free it, until the list is empty
// Leaf: ClearEvents
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0EA10, 52 bytes (`ret`). Descriptive names (no PDB covers this library): the
//   manifest's "CEngineVSConstantLayout_ClearAllEvents" is propagated. The function is slot 3 of the host vtable
//   0x012B25A8 (reference 0x012B25B4) and is called directly from the host destructor (0x00C0ED23); the records are the
//   12-byte POD events of QueueEvent 0x00C0E9E0.
// Callees: the typed list's GetFirst (virtual slot 11, +0x2c) and Remove (slot 8, +0x20), MSVCR71 scalar
//   `operator delete` (thunk 0x00BFE9BC; no null test: the record is a POD and the loop already tested it).
// Shape: the list address is kept in esi (`lea esi,[ecx+60h]`), the record in edi; the loop is rotated.
struct CQueuedEvent {                           // 12 bytes, no constructor
    long m_type;                                // +0
    long m_time;                                // +4
    long m_data;                                // +8
};

struct CEventList {                             // typed list over an implementation object (vtable 0x012B23A0)
    virtual long Compare0(void* a, void* b);
    virtual long Compare1(void* a, void* b);
    virtual void* Key2(void* item);
    virtual void* Key3(void* item);
    virtual void Slot4();
    virtual void Slot5();
    virtual void Slot6();
    virtual void Slot7();
    virtual void Remove(CQueuedEvent* item);    // slot 8 (+0x20)
    virtual void Add(CQueuedEvent* item);       // slot 9 (+0x24)
    virtual void Slot10();
    virtual CQueuedEvent* GetFirst();           // slot 11 (+0x2c)
    void* m_pImpl;                              // +4
    unsigned char m_impl[8];                    // +8
};

struct CScriptHost {
    void* m_vtable;                             // +0x00 (0x012B25A8)
    unsigned char _pad04[0x60 - 0x04];
    CEventList m_events;                        // +0x60
    void ClearEvents();
};

void CScriptHost::ClearEvents()
{
    CQueuedEvent* event = m_events.GetFirst();
    while (event) {
        m_events.Remove(event);
        delete event;
        event = m_events.GetFirst();
    }
}