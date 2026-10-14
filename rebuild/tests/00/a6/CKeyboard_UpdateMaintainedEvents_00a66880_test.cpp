#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CInputEvent { unsigned char _body[0x28]; long m_kind; float m_time; unsigned char _pad[4]; long GetType() const { return m_kind; } };
struct CInputEventScanInfo { bool Started; long Index; };
struct CMaintainedEvent { long m_0; CInputEvent m_event; };
struct SNode { SNode* m_pNext; SNode* m_pPrev; CMaintainedEvent m_value; };
static unsigned long g_time = 0; static unsigned long timeGetTime() { return g_time; } static const float g_timeScale = 0.001f;
struct CKeyboard { CInputEvent EventStore[256]; long NoStoredEvents; long NoEventScansRunning; SNode* m_pMaintainedEvents;
    void UpdateMaintainedEvents() { for (SNode* node = m_pMaintainedEvents->m_pNext; node != m_pMaintainedEvents; node = node->m_pNext) { if (node->m_value.m_event.GetType() == 2) node->m_value.m_event.m_time = (float)timeGetTime() * g_timeScale; } } };

int main()
{
    CKeyboard* d = new CKeyboard; std::memset(d, 0, sizeof(*d)); SNode head, n1, n2; head.m_pNext = &n1; n1.m_pNext = &n2; n2.m_pNext = &head; d->m_pMaintainedEvents = &head;
    n1.m_value.m_event.m_kind = 2; n1.m_value.m_event.m_time = -1.0f; n2.m_value.m_event.m_kind = 3; n2.m_value.m_event.m_time = -1.0f;
    g_time = 3000; d->UpdateMaintainedEvents(); if (n1.m_value.m_event.m_time < 2.999f || n1.m_value.m_event.m_time > 3.001f || n2.m_value.m_event.m_time != -1.0f) { std::printf("00a66880_TEST FAIL\n"); return 1; }
    g_time = 0xFFFFFFFFul; d->UpdateMaintainedEvents(); if (n1.m_value.m_event.m_time < 4294967.0f || n1.m_value.m_event.m_time > 4294968.0f) { std::printf("00a66880_TEST FAIL\n"); return 1; }
    delete d;
    std::printf("00a66880_TEST PASS\n");
    return 0;
}