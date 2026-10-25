// Behaviour test for 00c12680: CountPlayingEvents -- under the module lock, counts the active events that are playing physically (state 6) or virtually (state 5)
#include <cstdio>

static int g_lockDepth = 0, g_virtualQueries = 0;
struct CScopedLock {
    CScopedLock(void*) { ++g_lockDepth; }
    ~CScopedLock() { --g_lockDepth; }
};
static unsigned char g_section[0x18];

struct CSoundEvent {
    long m_active;
    long m_state;
    long IsActive() { return m_active; }
    long IsPlayingPhysical() { return m_state == 6; }
    long IsPlayingVirtual() { ++g_virtualQueries; return m_state == 5; }
    bool IsPlaying() { return IsPlayingPhysical() || IsPlayingVirtual(); }
};

struct CEventPool {
    CSoundEvent m_items[6];
    unsigned long m_count;
    CSoundEvent* Get(unsigned long index) { return &m_items[index]; }
};

struct CSoundEventManager {
    CEventPool m_events;
    unsigned long GetNumEvents() { return m_events.m_count; }
    CSoundEvent* GetEvent(unsigned long index) { return m_events.Get(index); }
    long CountPlayingEvents()
    {
        CScopedLock lock(g_section);
        long count = 0;
        for (unsigned long i = 0; i < GetNumEvents(); ++i) {
            CSoundEvent* event = GetEvent(i);
            if (event->IsActive() && event->IsPlaying())
                ++count;
        }
        return count;
    }
};

int main()
{
    CSoundEventManager m;
    long active[6] = { 1, 1, 1, 0, 1, 1 };
    long state[6]  = { 6, 5, 2, 6, 6, 5 };
    for (int i = 0; i < 6; ++i) { m.m_events.m_items[i].m_active = active[i]; m.m_events.m_items[i].m_state = state[i]; }
    m.m_events.m_count = 5;
    if (m.CountPlayingEvents() != 3 || g_lockDepth != 0) { std::printf("00c12680_TEST FAIL\n"); return 1; }
    if (g_virtualQueries != 2) { std::printf("00c12680_TEST FAIL\n"); return 1; }   // short-circuit: only the non-physical active events ask
    m.m_events.m_count = 0;
    if (m.CountPlayingEvents() != 0) { std::printf("00c12680_TEST FAIL\n"); return 1; }
    std::printf("00c12680_TEST PASS\n");
    return 0;
}