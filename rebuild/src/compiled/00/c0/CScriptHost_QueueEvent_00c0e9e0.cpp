// CScriptHost::QueueEvent(long type, long time, long data) -- allocate a 12-byte event record {type, time, data} and append it to the host's event list at +0x60 (virtual slot 9)
// Leaf: QueueEvent
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0E9E0, 46 bytes (`ret 0Ch`). Descriptive names (no PDB covers this library):
//   the manifest's "CEngineVSConstantLayout_QueueEventWithData" is propagated. The host is the class of vtable
//   0x012B25A8 (landed TryActivate 0x00C0E600 = its slot 5, the landed name CScriptHost_00c0e600); its destructor
//   (0x00C0ECE0..) builds the typed list at +0x60 with vtable 0x012B23A0, whose slot 9 forwards to the list
//   implementation at +4. The only caller (0x00C171FB) queues a table of 12-byte records with a time offset added.
// Callees: MSVCR71 scalar `operator new` (thunk 0x00BFEA1A), the list's Add (virtual slot 9, +0x24).
// Shape: a POD record: no null check after `new`. Retail stores +4 (ecx), +0 (ecx), +8 (edx, loaded first): VC7.1
//   stores the first source assignment first, holds the second in edx and stores it last, so the source order is
//   time, data, type (type, time, data gives +0, +8, +4).
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
    void QueueEvent(long type, long time, long data);
};

void CScriptHost::QueueEvent(long type, long time, long data)
{
    CQueuedEvent* event = new CQueuedEvent;
    event->m_time = time;
    event->m_data = data;
    event->m_type = type;
    m_events.Add(event);
}