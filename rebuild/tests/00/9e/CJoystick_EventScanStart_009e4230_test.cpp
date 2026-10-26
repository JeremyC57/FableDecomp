#include <cstdio>
#include <cstdlib>
#include <cstring>
class CInputEvent { public: unsigned char _body[0x28]; long m_kind; float m_time; unsigned char _pad[4]; long GetType() const { return m_kind; } };
class CInputEventScanInfo { public: bool Started; long Index; };
class CMaintainedEvent { public: long m_0; CInputEvent m_event; };
struct SNode { SNode* m_pNext; SNode* m_pPrev; CMaintainedEvent m_value; };
class CJoystick { public: CInputEvent EventStore[64]; SNode* m_pMaintainedEvents; long NoStoredEvents; long NoEventScansRunning;
    bool EventScanStart(CInputEventScanInfo& scan, CInputEvent& event) { scan.Started = true; ++NoEventScansRunning; if (NoStoredEvents > 0) { scan.Index = 0; event = EventStore[0]; return true; } return false; }
    void AddMaintainedEventsToEventStore() { for (SNode* node = m_pMaintainedEvents->m_pNext; node != m_pMaintainedEvents; node = node->m_pNext) { if (NoStoredEvents >= 0 && NoStoredEvents < 64) { EventStore[NoStoredEvents] = node->m_value.m_event; ++NoStoredEvents; } } } };

int main()
{
    CJoystick* d = new CJoystick; std::memset(d, 0, sizeof(*d)); d->EventStore[0].m_kind = 9; d->EventStore[0].m_time = 2.5f; d->NoStoredEvents = 3;
    CInputEventScanInfo scan; scan.Started = false; scan.Index = 77; CInputEvent e; std::memset(&e, 0, sizeof(e));
    if (!d->EventScanStart(scan, e) || !scan.Started || scan.Index != 0 || e.m_kind != 9 || e.m_time != 2.5f || d->NoEventScansRunning != 1) { std::printf("009e4230_TEST FAIL\n"); return 1; }
    d->NoStoredEvents = 0; scan.Started = false; scan.Index = 77;
    if (d->EventScanStart(scan, e) || !scan.Started || scan.Index != 77 || d->NoEventScansRunning != 2) { std::printf("009e4230_TEST FAIL\n"); return 1; }
    delete d;
    std::printf("009e4230_TEST PASS\n");
    return 0;
}