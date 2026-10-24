// CSoundEventManager_00c12680::CountPlayingEvents() -- under the module lock, counts the active events of the pool at +0x5c that are playing on a physical or a virtual player
// Leaf: CountPlayingEvents
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C12680, 159 bytes (+ int3 padding), __fastcall `ret`, EH-framed.
// Identity: the libpk sound-event manager (vtable 0x012B2748, no RTTI; destructor 0x00C13470 landed as "C3DMesh2", a
//   propagated label): its +0x2c member is the polymorphic event set whose PKCFrallocStatic event pool sits at +0x5c
//   (count +0x60, bounds-checked accessor @ 0x00C13620, 0xb4-byte events); an event's state word is +0xac (IsActive
//   @ 0x00C20840 = state != 0, IsPlayingPhysical @ 0x00C20F20 = state 6, IsPlayingVirtual @ 0x00C20F40 = state 5: the
//   describer @ 0x00C21040 prints "EventId(..), Priority=.., State='Playing using physical/virtual player'"). The
//   whole walk runs under the module's scoped lock over the critical section @ 0x0143BB14 (@ 0x00C0FFC0 /
//   0x00C0FFE0, EH state 0). No PDB name set covers libpk: the classes are named by address, names descriptive.
// Caller: vtable 0x012B2748 slot 31 (0x012B27C4).
// Shape: the walk goes through inline manager accessors (`GetNumEvents()`, `GetEvent(i)`) and an inline
//   `IsPlaying()` (physical || virtual): the plain `m_events.Get(i)` form swaps the index and event registers (esi/edi).
//   The count is a frame word (`mov [esp+8],esi` with the zero index, `inc dword ptr [esp+10h]` in the loop: all
//   four callee-saved registers hold this, the index, the pool and the event) read back after the lock destructor;
//   `a && (b || c)` gives the je/jne/je chain into the shared increment.
// Label correction: the catalog's `Inventory_CountEquippedItems` is propagated.
extern unsigned char g_section_0143bb14[];  // 0x0143BB14: the module's critical section

struct CScopedLock_00c12680 {                  // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    CScopedLock_00c12680(void* section);        // 0x00C0FFC0
    ~CScopedLock_00c12680();                    // 0x00C0FFE0
};

struct CSoundEvent_00c12680 {                  // 0xb4 bytes
    long IsActive();                       // 0x00C20840
    long IsPlayingPhysical();              // 0x00C20F20 (state 6)
    long IsPlayingVirtual();               // 0x00C20F40 (state 5)
    bool IsPlaying() { return IsPlayingPhysical() || IsPlayingVirtual(); }
};

struct CEventPool_00c12680 {                   // +0x5c: PKCFrallocStatic<CSoundEvent>
    long m_0;
    unsigned long m_count;                 // +4 (= +0x60)
    CSoundEvent_00c12680* Get(unsigned long index);   // 0x00C13620
};

struct CSoundEventManager_00c12680 {
    unsigned char _pad[0x5c];              // vtable 0x012B2748, the event set at +0x2c
    CEventPool_00c12680 m_events;               // +0x5c
    unsigned long GetNumEvents() { return m_events.m_count; }
    CSoundEvent_00c12680* GetEvent(unsigned long index) { return m_events.Get(index); }
    long CountPlayingEvents();
};

long CSoundEventManager_00c12680::CountPlayingEvents()
{
    CScopedLock_00c12680 lock(g_section_0143bb14);
    long count = 0;
    for (unsigned long i = 0; i < GetNumEvents(); ++i) {
        CSoundEvent_00c12680* event = GetEvent(i);
        if (event->IsActive() && event->IsPlaying())
            ++count;
    }
    return count;
}