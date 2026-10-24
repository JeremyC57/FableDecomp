// CSoundEventManager_00c123b0::Shutdown() -- under the module lock, stops every active event of the pool at +0x5c (handing it the manager), then detaches every listener the event set at +0x2c still yields (its virtual slot 11, re-queried after each detach)
// Leaf: Shutdown
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C123B0, 171 bytes (+ int3 padding), __fastcall `ret`, EH-framed.
// Identity: the libpk sound-event manager (vtable 0x012B2748, no RTTI; destructor 0x00C13470 landed as "C3DMesh2", a
//   propagated label): its +0x2c member is the polymorphic event set whose PKCFrallocStatic event pool sits at +0x5c
//   (count +0x60, bounds-checked accessor @ 0x00C13620, 0xb4-byte events); an event's state word is +0xac (IsActive
//   @ 0x00C20840 = state != 0, IsPlayingPhysical @ 0x00C20F20 = state 6, IsPlayingVirtual @ 0x00C20F40 = state 5: the
//   describer @ 0x00C21040 prints "EventId(..), Priority=.., State='Playing using physical/virtual player'"). The
//   whole walk runs under the module's scoped lock over the critical section @ 0x0143BB14 (@ 0x00C0FFC0 /
//   0x00C0FFE0, EH state 0). No PDB name set covers libpk: the classes are named by address, names descriptive.
// Callers: vtable 0x012B2748 slot 19 (0x012B2794) and the destructor 0x00C13470 (under its own lock). Callees: Stop
//   @ 0x00C21620, the listener's Detach @ 0x00C26580 (the landed UnbindKey 0x00C12520 uses the same pair).
// Shape: the walk goes through inline manager accessors (`GetNumEvents()`, `GetEvent(i)` over the pool): with them
//   VC7.1 gives `this` esi, the index edi and the event ebx (the plain `m_events.Get(i)` form, also with the Get body
//   in the TU, gives this edi / index ebx / event esi); the count is re-read each pass, the pool address sits in ebp,
//   ebx/ebp are pushed inside the guarded block, `lea esp,[esp]` aligns the loop; the listener loop loads the set's
//   vtable before forming its address (`mov eax,[esi+2Ch] ; lea edi,[esi+2Ch]`).
// Label correction: the catalog's `C3DMesh2_OnDestructorCleanup` is propagated.
extern unsigned char g_section_0143bb14[];  // 0x0143BB14: the module's critical section

struct CScopedLock_00c123b0 {                  // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    CScopedLock_00c123b0(void* section);        // 0x00C0FFC0
    ~CScopedLock_00c123b0();                    // 0x00C0FFE0
};

struct CSoundEventManager_00c123b0;

struct CSoundEvent_00c123b0 {                  // 0xb4 bytes
    long IsActive();                       // 0x00C20840
    void Stop(CSoundEventManager_00c123b0* manager);   // 0x00C21620
};

struct CEventPool_00c123b0 {                   // +0x5c: PKCFrallocStatic<CSoundEvent>
    long m_0;
    unsigned long m_count;                 // +4 (= +0x60)
    CSoundEvent_00c123b0* Get(unsigned long index);   // 0x00C13620
};

struct CListener_00c123b0 {
    void Detach(CSoundEventManager_00c123b0* manager);   // 0x00C26580
};

struct CEventSet_00c123b0 {                    // +0x2c (polymorphic)
    virtual void Slot00(); virtual void Slot01(); virtual void Slot02(); virtual void Slot03();
    virtual void Slot04(); virtual void Slot05(); virtual void Slot06(); virtual void Slot07();
    virtual void Slot08(); virtual void Slot09(); virtual void Slot10();
    virtual CListener_00c123b0* GetFirstListener();   // slot 11 (+0x2c)
    unsigned char _body[0x2c];
};

struct CSoundEventManager_00c123b0 {
    unsigned char _pad[0x2c];              // vtable 0x012B2748 and the first members
    CEventSet_00c123b0 m_eventSet;              // +0x2c
    CEventPool_00c123b0 m_events;               // +0x5c
    unsigned long GetNumEvents() { return m_events.m_count; }
    CSoundEvent_00c123b0* GetEvent(unsigned long index) { return m_events.Get(index); }
    void Shutdown();
};

void CSoundEventManager_00c123b0::Shutdown()
{
    CScopedLock_00c123b0 lock(g_section_0143bb14);
    for (unsigned long i = 0; i < GetNumEvents(); ++i) {
        CSoundEvent_00c123b0* event = GetEvent(i);
        if (event->IsActive())
            event->Stop(this);
    }
    for (CListener_00c123b0* listener = m_eventSet.GetFirstListener(); listener; listener = m_eventSet.GetFirstListener())
        listener->Detach(this);
}