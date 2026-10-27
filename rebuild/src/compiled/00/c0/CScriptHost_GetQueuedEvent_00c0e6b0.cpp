// CScriptHost::GetQueuedEvent(long type, long* time, long* data) -- look up the queued event of a type in the host's event list at +0x60: copy its time and data out and return 0, or return -1 when none is queued
// Leaf: ?GetQueuedEvent@CScriptHost@@UAEJJPAJ0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0E6B0, 47 bytes, thiscall, `ret 0Ch`.
// Identity (descriptive): virtual slot 4 of the script host vtable 0x012B25A8 (landed slot 5 TryActivate 0x00C0E600,
//   QueueEvent 0x00C0E9E0 that fills the same 12-byte records {type, time, data}). libpk has no PDB or RTTI.
// Callee: the typed event list's keyed Find (virtual slot 17, +0x44), the key passed by reference to the type's own
//   argument slot (`lea edx,[esp+4]`), as in the landed DestroyOwner 0x00C0EC10.
struct CQueuedEvent {                           // 12 bytes (landed QueueEvent 0x00C0E9E0)
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
    virtual void Slot12();
    virtual void Slot13();
    virtual void Slot14();
    virtual void Slot15();
    virtual void Slot16();
    virtual CQueuedEvent* Find(const long& type);   // slot 17 (+0x44)
    void* m_pImpl;                              // +4
    unsigned char m_impl[8];                    // +8
};

struct CScriptHost {                            // vtable 0x012B25A8
    virtual void Post();                        // slot 0 (0x00C0E970)
    virtual void Slot1();
    virtual void Slot2();
    virtual void ClearEvents();                 // slot 3 (landed 0x00C0EA10)
    virtual long GetQueuedEvent(long type, long* time, long* data);   // slot 4
    unsigned char _pad04[0x60 - 0x04];
    CEventList m_events;                        // +0x60
};

long CScriptHost::GetQueuedEvent(long type, long* time, long* data)
{
    CQueuedEvent* event = m_events.Find(type);
    if (!event)
        return -1;
    *time = event->m_time;
    *data = event->m_data;
    return 0;
}