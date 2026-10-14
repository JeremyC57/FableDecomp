#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CInputEvent { unsigned char _body[0x28]; long m_kind; float m_time; unsigned char _pad[4]; long GetType() const { return m_kind; } };
struct CInputEventScanInfo { bool Started; long Index; };
struct CMaintainedEvent { long m_0; CInputEvent m_event; };
struct SNode { SNode* m_pNext; SNode* m_pPrev; CMaintainedEvent m_value; };
struct CKeyboard { CInputEvent EventStore[256]; SNode* m_pMaintainedEvents; long NoStoredEvents; long NoEventScansRunning;
    bool EventScanStart(CInputEventScanInfo& scan, CInputEvent& event) { scan.Started = true; ++NoEventScansRunning; if (NoStoredEvents > 0) { scan.Index = 0; event = EventStore[0]; return true; } return false; }
    void AddMaintainedEventsToEventStore() { for (SNode* node = m_pMaintainedEvents->m_pNext; node != m_pMaintainedEvents; node = node->m_pNext) { if (NoStoredEvents >= 0 && NoStoredEvents < 256) { EventStore[NoStoredEvents] = node->m_value.m_event; ++NoStoredEvents; } } } };

int main()
{
    CKeyboard* d = new CKeyboard; std::memset(d, 0, sizeof(*d)); SNode head, n1, n2, n3; head.m_pNext = &n1; n1.m_pNext = &n2; n2.m_pNext = &n3; n3.m_pNext = &head;
    n1.m_value.m_event.m_kind = 11; n2.m_value.m_event.m_kind = 12; n3.m_value.m_event.m_kind = 13; d->m_pMaintainedEvents = &head;
    d->NoStoredEvents = 256 - 2; d->AddMaintainedEventsToEventStore();
    if (d->NoStoredEvents != 256 || d->EventStore[256 - 2].m_kind != 11 || d->EventStore[256 - 1].m_kind != 12) { std::printf("00a668e0_TEST FAIL\n"); return 1; }
    d->NoStoredEvents = 0; d->AddMaintainedEventsToEventStore(); if (d->NoStoredEvents != 3 || d->EventStore[2].m_kind != 13) { std::printf("00a668e0_TEST FAIL\n"); return 1; }
    d->NoStoredEvents = -1; d->AddMaintainedEventsToEventStore(); if (d->NoStoredEvents != -1) { std::printf("00a668e0_TEST FAIL\n"); return 1; }
    head.m_pNext = &head; d->NoStoredEvents = 0; d->AddMaintainedEventsToEventStore(); if (d->NoStoredEvents != 0) { std::printf("00a668e0_TEST FAIL\n"); return 1; }
    delete d;
    std::printf("00a668e0_TEST PASS\n");
    return 0;
}
