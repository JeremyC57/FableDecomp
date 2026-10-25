// behaviour test for 0x00C11CB0 CSoundEventManager::UpdateItems: activate every active event the manager has not handled yet
#include <cstdio>

struct CSoundEventManager;

struct CSoundEvent {
    long m_state;
    bool m_handled;
    int m_activations;
    long IsActive() const { return m_state != 0; }               // stand-in for 0x00C20840
    bool IsAudible(CSoundEventManager*) { return m_handled; }                // stand-in for 0x00C20C10
    void Stop(CSoundEventManager*) { ++m_activations; }                  // stand-in for 0x00C21620
};

struct CSoundEventPool {
    unsigned long m_count;
    CSoundEvent* m_pEvents;
    CSoundEvent& operator[](unsigned long index) { return m_pEvents[index]; }   // 0x00C13620
};

struct CSoundEventSet {
    CSoundEventPool m_events;
};

struct CSoundEventManager {
    CSoundEventSet m_eventSet;
    void UpdateItems();
};

void CSoundEventManager::UpdateItems()
{
    for (unsigned long i = 0; i < m_eventSet.m_events.m_count; ++i) {
        CSoundEvent& event = m_eventSet.m_events[i];
        if (event.IsActive()) {
            if (!event.IsAudible(this))
                event.Stop(this);
        }
    }
}

int main()
{
    CSoundEvent events[4] = { { 1, false, 0 }, { 0, false, 0 }, { 2, true, 0 }, { 3, false, 0 } };
    CSoundEventManager manager; manager.m_eventSet.m_events.m_count = 4; manager.m_eventSet.m_events.m_pEvents = events;
    manager.UpdateItems();
    if (events[0].m_activations != 1 || events[1].m_activations != 0 || events[2].m_activations != 0 || events[3].m_activations != 1) { std::printf("00c11cb0_TEST FAIL\n"); return 1; }
    manager.m_eventSet.m_events.m_count = 0;
    manager.UpdateItems();
    if (events[0].m_activations != 1) { std::printf("00c11cb0_TEST FAIL\n"); return 1; }
    std::printf("00c11cb0_TEST PASS\n");
    return 0;
}