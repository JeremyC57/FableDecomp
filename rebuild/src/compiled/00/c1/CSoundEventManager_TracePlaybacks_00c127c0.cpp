// CSoundEventManager::TracePlaybacks() -- under the module lock, refreshes the events and then, for every playback slot of the pool at +0x224 that holds an event, builds the event's description and the slot's status strings for the (compiled-out) trace output
// Leaf: ?TracePlaybacks@CSoundEventManager@@QAEXXZ
// Flags: /EHsc
// Naming (sweep1, 2026-10-25): libpk class set CSoundEventManager (vtable 0x012B2748) / CSoundEventSet (its +0x2C
//   member) / CSoundEvent (0xB4 bytes, vtable 0x012B3674): the describer 0x00C21040 called by TraceEvents/
//   TracePlaybacks prints "EventId(", "Priority=", "State='Waiting for data'" / "Playing using physical player";
//   chunk 431/435's CPanel/CPanelItem names came from propagated UI labels; this row drops the address suffix of its
//   class names (renamed from 00/c1/CSoundEventManager00c127c0_TracePlaybacks_00c127c0.cpp).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C127C0, 186 bytes (+ int3 padding), __fastcall `ret`, EH-framed.
// Identity: the libpk sound-event manager (vtable 0x012B2748, no RTTI; destructor 0x00C13470 landed as "C3DMesh2", a
//   propagated label): its +0x2c member is the polymorphic event set whose PKCFrallocStatic event pool sits at +0x5c
//   (count +0x60, bounds-checked accessor @ 0x00C13620, 0xb4-byte events; IsActive @ 0x00C20840 = state word +0xac
//   != 0); the describer @ 0x00C21040 returns an MFC-style string by value ("EventId(..), Priority=.., State='..'").
//   The walk runs under the module's scoped lock over the critical section @ 0x0143BB14 (@ 0x00C0FFC0 / 0x00C0FFE0,
//   EH state 0) after the manager's event refresh @ 0x00C11CB0 (landed as CSoundEventManager::UpdateItems). No PDB name set
//   covers libpk: the classes are named by address, names descriptive.
//   The playback pool is a second PKCFrallocStatic at +0x224 (count +0x228, accessor @ 0x00C13700, 0x2c-byte slots:
//   the event at +4; the slot's status string @ 0x00C27760, landed as CPlayback::LogStatus: "CODA reports source IS
//   playing(pos/len)").
// Shape: the two-argument trace evaluates right to left: the event description first (temporary 1, EH state 1 while
//   the status call runs), then the slot status (temporary 2, no state of its own); they die in reverse order.
// Label correction: the catalog's `CTCInventory_DumpCombatActionDebug_c127c0` is propagated.
extern unsigned char g_section_0143bb14[];  // 0x0143BB14: the module's critical section
extern char* g_pchNil_0139c900;            // 0x0139C900

struct CScopedLock_00c127c0 {                  // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    CScopedLock_00c127c0(void* section);        // 0x00C0FFC0
    ~CScopedLock_00c127c0();                    // 0x00C0FFE0
};

struct CString_00c127c0 {                      // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c127c0() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c127c0();                        // 0x00C0BA40
};

inline void TraceOutput(const CString_00c127c0&, const CString_00c127c0&) { }   // the release build's trace sink: compiled out, its argument still built

struct CSoundEventManager;

struct CSoundEvent {                  // 0xb4 bytes
    CString_00c127c0 Describe(CSoundEventManager* manager);   // 0x00C21040
};

struct CPlaybackSlot_00c127c0 {                // 0x2c bytes
    long m_0;
    CSoundEvent* m_pEvent;             // +0x04
    unsigned char _body[0x24];
    CString_00c127c0 GetStatus();               // 0x00C27760
};

struct CPlaybackPool_00c127c0 {                // +0x224: PKCFrallocStatic<CPlaybackSlot>
    long m_0;
    unsigned long m_count;                 // +4 (= +0x228)
    CPlaybackSlot_00c127c0* Get(unsigned long index);   // 0x00C13700
};

struct CSoundEventManager {
    unsigned char _pad[0x224];             // vtable 0x012B2748, the event set at +0x2c, the event pool at +0x5c
    CPlaybackPool_00c127c0 m_playbacks;         // +0x224
    unsigned long GetNumPlaybacks() { return m_playbacks.m_count; }
    CPlaybackSlot_00c127c0* GetPlayback(unsigned long index) { return m_playbacks.Get(index); }
    void UpdateEvents();                   // 0x00C11CB0
    void TracePlaybacks();
};

void CSoundEventManager::TracePlaybacks()
{
    CScopedLock_00c127c0 lock(g_section_0143bb14);
    UpdateEvents();
    for (unsigned long i = 0; i < GetNumPlaybacks(); ++i) {
        CPlaybackSlot_00c127c0* slot = GetPlayback(i);
        if (slot->m_pEvent)
            TraceOutput(slot->GetStatus(), slot->m_pEvent->Describe(this));
    }
}