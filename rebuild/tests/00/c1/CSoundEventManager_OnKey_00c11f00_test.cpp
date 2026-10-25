// behaviour test for 0x00C11F00 CSoundEventManager::OnKey: when the event set is idle, update the events and, still idle, activate the event bound to the key
#include <cstdio>

struct CSoundEventManager;
static int g_updates = 0, g_busyQueries = 0;

struct CSoundEvent {
    unsigned long m_key;
    int m_activations;
    void Stop(CSoundEventManager*) { ++m_activations; }                  // stand-in for 0x00C21620
};

struct CSoundEventSet {
    long m_busyAt;                                                // query number from which the set reports busy (0 = never)
    CSoundEvent* m_events;
    int m_count;
    long IsBusy() { ++g_busyQueries; return m_busyAt != 0 && g_busyQueries >= m_busyAt; }   // stand-in for 0x00C21DF0
    CSoundEvent* FindItem(unsigned long key)                      // stand-in for 0x00C21E60
    {
        for (int i = 0; i < m_count; ++i) if (m_events[i].m_key == key) return &m_events[i];
        return 0;
    }
};

struct CSoundEventManager {
    CSoundEventSet m_eventSet;
    void UpdateItems() { ++g_updates; }                          // stand-in for 0x00C11CB0
    void OnKey(unsigned long key);
};

void CSoundEventManager::OnKey(unsigned long key)
{
    if (!m_eventSet.IsBusy()) {
        UpdateItems();
        if (!m_eventSet.IsBusy()) {
            CSoundEvent* event = m_eventSet.FindItem(key);
            if (event)
                event->Stop(this);
            m_eventSet.IsBusy();
        }
    }
}

int main()
{
    CSoundEvent events[2] = { { 7, 0 }, { 9, 0 } };
    CSoundEventManager manager; manager.m_eventSet.m_events = events; manager.m_eventSet.m_count = 2; manager.m_eventSet.m_busyAt = 0;
    manager.OnKey(9);
    if (events[1].m_activations != 1 || events[0].m_activations != 0 || g_updates != 1 || g_busyQueries != 3) { std::printf("00c11f00_TEST FAIL\n"); return 1; }
    manager.OnKey(5);                                               // unbound key: update only
    if (g_updates != 2 || events[0].m_activations + events[1].m_activations != 1) { std::printf("00c11f00_TEST FAIL\n"); return 1; }
    g_busyQueries = 0; manager.m_eventSet.m_busyAt = 2;              // busy after the update
    manager.OnKey(7);
    if (g_updates != 3 || events[0].m_activations != 0 || g_busyQueries != 2) { std::printf("00c11f00_TEST FAIL\n"); return 1; }
    g_busyQueries = 0; manager.m_eventSet.m_busyAt = 1;              // busy from the start
    manager.OnKey(7);
    if (g_updates != 3 || g_busyQueries != 1) { std::printf("00c11f00_TEST FAIL\n"); return 1; }
    std::printf("00c11f00_TEST PASS\n");
    return 0;
}