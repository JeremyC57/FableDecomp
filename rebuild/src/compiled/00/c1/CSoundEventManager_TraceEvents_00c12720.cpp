// CSoundEventManager::TraceEvents() -- under the module lock, refreshes the events and then builds the description string of every active event of the pool at +0x5c for the (compiled-out) trace output
// Leaf: ?TraceEvents@CSoundEventManager@@QAEXXZ
// Flags: /EHsc
// Naming (sweep1, 2026-10-25): libpk class set CSoundEventManager (vtable 0x012B2748) / CSoundEventSet (its +0x2C
//   member) / CSoundEvent (0xB4 bytes, vtable 0x012B3674): the describer 0x00C21040 called by TraceEvents/
//   TracePlaybacks prints "EventId(", "Priority=", "State='Waiting for data'" / "Playing using physical player";
//   chunk 431/435's CPanel/CPanelItem names came from propagated UI labels; this row drops the address suffix of its
//   class names (renamed from 00/c1/CSoundEventManager00c12720_TraceEvents_00c12720.cpp).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C12720, 152 bytes (+ int3 padding), __fastcall `ret`, EH-framed.
// Identity: the libpk sound-event manager (vtable 0x012B2748, no RTTI; destructor 0x00C13470 landed as "C3DMesh2", a
//   propagated label): its +0x2c member is the polymorphic event set whose PKCFrallocStatic event pool sits at +0x5c
//   (count +0x60, bounds-checked accessor @ 0x00C13620, 0xb4-byte events; IsActive @ 0x00C20840 = state word +0xac
//   != 0); the describer @ 0x00C21040 returns an MFC-style string by value ("EventId(..), Priority=.., State='..'").
//   The walk runs under the module's scoped lock over the critical section @ 0x0143BB14 (@ 0x00C0FFC0 / 0x00C0FFE0,
//   EH state 0) after the manager's event refresh @ 0x00C11CB0 (landed as CSoundEventManager::UpdateItems). No PDB name set
//   covers libpk: the classes are named by address, names descriptive.
// Shape: the release trace macro leaves the describer call and the temporary's destruction (`push esi ; lea eax,[esp+14h]
//   ; push eax ; call Describe ; lea ecx,[esp+10h] ; call ~CString`, no EH state for the temporary: nothing can throw
//   before its destructor); the walk uses the inline accessors that give retail's esi/edi/ebx assignment (0x00C123B0);
//   the zero index is the state word and the first count test is a memory compare after the refresh call.
// Label correction: the catalog's `Inventory_DebugLogState` is propagated.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;  // 0x0143BB14: the module's critical section
extern char* g_pchNil_0139c900;            // 0x0139C900

class PKCProtectionInstance { public:                  // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    PKCProtectionInstance(PKCCriticalSection* section);        // 0x00C0FFC0
    ~PKCProtectionInstance();                    // 0x00C0FFE0
};

struct CString_00c12720 {                      // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c12720() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c12720();                        // 0x00C0BA40
};

inline void TraceOutput(const CString_00c12720&) { }   // the release build's trace sink: compiled out, its argument still built

struct CSoundEventManager;

struct CSoundEvent {                  // 0xb4 bytes
    long IsActive();                       // 0x00C20840
    CString_00c12720 Describe(CSoundEventManager* manager);   // 0x00C21040
};

struct CSoundEventPool {                   // +0x5c: PKCFrallocStatic<CSoundEvent>
    long m_0;
    unsigned long m_count;                 // +4 (= +0x60)
    CSoundEvent* Get(unsigned long index);   // 0x00C13620
};

struct CSoundEventManager {
    unsigned char _pad[0x5c];              // vtable 0x012B2748, the event set at +0x2c
    CSoundEventPool m_events;               // +0x5c
    unsigned long GetNumEvents() { return m_events.m_count; }
    CSoundEvent* GetEvent(unsigned long index) { return m_events.Get(index); }
    void UpdateEvents();                   // 0x00C11CB0
    void TraceEvents();
};

void CSoundEventManager::TraceEvents()
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    UpdateEvents();
    for (unsigned long i = 0; i < GetNumEvents(); ++i) {
        CSoundEvent* event = GetEvent(i);
        if (event->IsActive())
            TraceOutput(event->Describe(this));
    }
}