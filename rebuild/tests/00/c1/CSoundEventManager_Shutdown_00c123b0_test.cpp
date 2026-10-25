// Behaviour test for 00c123b0: sound-event manager Shutdown -- under the module lock, stops every active event of the pool, then detaches listeners from the event set until it reports none
#include <cstdio>

static int g_lockDepth = 0, g_maxDepth = 0;
struct CScopedLock {
    CScopedLock(void*) { if (++g_lockDepth > g_maxDepth) g_maxDepth = g_lockDepth; }
    ~CScopedLock() { --g_lockDepth; }
};
static unsigned char g_section[0x18];

struct CSoundEventManager;

struct CSoundEvent {
    long m_active;
    int m_stops;
    long IsActive() { return m_active; }
    void Stop(CSoundEventManager*) { if (g_lockDepth != 1) m_stops = 100; ++m_stops; m_active = 0; }
};

struct CEventPool {
    CSoundEvent m_items[5];
    unsigned long m_count;
    CSoundEvent* Get(unsigned long index) { return &m_items[index]; }
};

struct CListener {
    CListener* m_pNext;
    int m_detached;
};

struct CEventSet {
    CListener* m_pHead;
    CListener* GetFirstListener() { return m_pHead; }
};

struct CSoundEventManager {
    CEventSet m_eventSet;
    CEventPool m_events;
    void Detach(CListener* l) { ++l->m_detached; if (m_eventSet.m_pHead == l) m_eventSet.m_pHead = l->m_pNext; }
    unsigned long GetNumEvents() { return m_events.m_count; }
    CSoundEvent* GetEvent(unsigned long index) { return m_events.Get(index); }
    void Shutdown()
    {
        CScopedLock lock(g_section);
        for (unsigned long i = 0; i < GetNumEvents(); ++i) {
            CSoundEvent* event = GetEvent(i);
            if (event->IsActive())
                event->Stop(this);
        }
        for (CListener* listener = m_eventSet.GetFirstListener(); listener; listener = m_eventSet.GetFirstListener())
            Detach(listener);              // retail: listener->Detach(manager) unlinks it from the set
    }
};

int main()
{
    CSoundEventManager m;
    long active[5] = { 1, 0, 1, 1, 0 };
    for (int i = 0; i < 5; ++i) { m.m_events.m_items[i].m_active = active[i]; m.m_events.m_items[i].m_stops = 0; }
    m.m_events.m_count = 4;                // the fifth slot is beyond the live count
    m.m_events.m_items[4].m_active = 1;
    CListener l3 = { 0, 0 }, l2 = { &l3, 0 }, l1 = { &l2, 0 };
    m.m_eventSet.m_pHead = &l1;
    m.Shutdown();
    int expect[5] = { 1, 0, 1, 1, 0 };
    for (int i = 0; i < 5; ++i)
        if (m.m_events.m_items[i].m_stops != expect[i]) { std::printf("00c123b0_TEST FAIL\n"); return 1; }
    if (l1.m_detached != 1 || l2.m_detached != 1 || l3.m_detached != 1 || m.m_eventSet.m_pHead != 0) { std::printf("00c123b0_TEST FAIL\n"); return 1; }
    if (g_lockDepth != 0 || g_maxDepth != 1) { std::printf("00c123b0_TEST FAIL\n"); return 1; }
    std::printf("00c123b0_TEST PASS\n");
    return 0;
}