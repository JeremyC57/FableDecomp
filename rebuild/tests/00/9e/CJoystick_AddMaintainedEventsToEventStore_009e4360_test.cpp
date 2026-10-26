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
    CJoystick* d = new CJoystick; std::memset(d, 0, sizeof(*d)); SNode head, n1, n2, n3; head.m_pNext = &n1; n1.m_pNext = &n2; n2.m_pNext = &n3; n3.m_pNext = &head;
    n1.m_value.m_event.m_kind = 11; n2.m_value.m_event.m_kind = 12; n3.m_value.m_event.m_kind = 13; d->m_pMaintainedEvents = &head;
    d->NoStoredEvents = 64 - 2; d->AddMaintainedEventsToEventStore();
    if (d->NoStoredEvents != 64 || d->EventStore[64 - 2].m_kind != 11 || d->EventStore[64 - 1].m_kind != 12) { std::printf("009e4360_TEST FAIL\n"); return 1; }
    d->NoStoredEvents = 0; d->AddMaintainedEventsToEventStore(); if (d->NoStoredEvents != 3 || d->EventStore[2].m_kind != 13) { std::printf("009e4360_TEST FAIL\n"); return 1; }
    d->NoStoredEvents = -1; d->AddMaintainedEventsToEventStore(); if (d->NoStoredEvents != -1) { std::printf("009e4360_TEST FAIL\n"); return 1; }
    head.m_pNext = &head; d->NoStoredEvents = 0; d->AddMaintainedEventsToEventStore(); if (d->NoStoredEvents != 0) { std::printf("009e4360_TEST FAIL\n"); return 1; }
    delete d;
    std::printf("009e4360_TEST PASS\n");
    return 0;
}