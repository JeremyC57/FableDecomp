// CKeyboard::UpdateMaintainedEvents() -- walk the maintained-event list (+0x340c) and restamp every maintained event of kind 2 (GetType @ 0x00A03B40) with the current time: GetTickCount (import 0x0143FDFC, unsigned) scaled by 0.001f (0x01266300) into the event's +0x2c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A66880, length 96 bytes.
// Callees: CInputEvent::GetType @ 0x00A03B40 (on the node's event at +0xc), GetTickCount through the import table (held in ebx across the loop).
// Shape: a one-word frame (`push ecx`) for the unsigned-to-float conversion (`fild` plus 2^32 when negative), `this` in edi, the node in esi, `fstp [esi+38h]`, `ret`.
class CInputEvent { public:                                // 0x34 bytes (PDB): the kind word at +0x28, the time at +0x2c
    unsigned char _body[0x28];
    long          m_kind;                          // +0x28
    float         m_time;                          // +0x2c
    unsigned char _pad30[4];
    long GetType() const;                          // 0x00A03B40 (labelled "CWorld::DrawGetEnvironment")
};
class CInputEventScanInfo { public:                       // PDB: bool Started +0, long Index +4
    bool Started;
    long Index;
};
class CMaintainedEvent { public:                          // the list element: a word then the event
    long        m_0;
    CInputEvent m_event;                           // +0x04
};
struct SMaintainedEventNode {                      // retail (VC6-era Dinkumware) list node: next, prev, value
    SMaintainedEventNode* m_pNext;
    SMaintainedEventNode* m_pPrev;
    CMaintainedEvent      m_value;                 // +0x08 (the event at +0x0c)
};
class CInputManager;
class CKeyboard { public:                                 // retail layout: 256 stored events, the one-word list head at +0x340c
    virtual void v00();
    CInputEvent           EventStore[256];         // +0x04
    long                  NoStoredEvents;          // +0x3404
    long                  NoEventScansRunning;     // +0x3408
    SMaintainedEventNode* m_pMaintainedEvents;     // +0x340c list<CMaintainedEvent> head
    bool EventScanStart(CInputEventScanInfo& scan, CInputEvent& event);
    void AddMaintainedEventsToEventStore();
    void UpdateMaintainedEvents();
};
extern "C" __declspec(dllimport) unsigned long __stdcall GetTickCount(void);   // import 0x0143FDFC = KERNEL32 GetTickCount (Fable.exe import table), not timeGetTime
extern const float g_timeScale_01266300;           // 0x01266300 = 0.001f (milliseconds to seconds)

void CKeyboard::UpdateMaintainedEvents()
{
    for (SMaintainedEventNode* node = m_pMaintainedEvents->m_pNext; node != m_pMaintainedEvents; node = node->m_pNext) {
        if (node->m_value.m_event.GetType() == 2)
            node->m_value.m_event.m_time = (float)GetTickCount() * g_timeScale_01266300;
    }
}