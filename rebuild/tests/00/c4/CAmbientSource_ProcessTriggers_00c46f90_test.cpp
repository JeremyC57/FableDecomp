// Behaviour test for 00c46f90: CAmbientSource::ProcessTriggers -- fire every due trigger, each schedules itself again
#include <cstdio>

struct CAmbientSource;
static int g_fired = 0, g_scheduled = 0;
struct CAmbientTrigger {
    unsigned int m_time; CAmbientTrigger* m_next;
    void Fire(CAmbientSource*) { ++g_fired; }
    void Schedule(CAmbientSource*) { ++g_scheduled; }
};
struct CClock { unsigned int m_now; unsigned int GetTime() const { return m_now; } };
struct CSoundSystem { CClock m_clock; };
struct CSoundBank { CSoundSystem* m_pSystem; };
struct CAmbientSet { CSoundBank* m_pBank; };
struct CList {
    CAmbientTrigger* m_head;
    CAmbientTrigger* GetFirst() { return m_head; }
    void RemoveItem(CAmbientTrigger* t) { if (m_head == t) m_head = t->m_next; }
};
struct CAmbientSource {
    CAmbientSet* m_pSet;
    CList m_triggers;
    void ProcessTriggers()
    {
        CAmbientTrigger* trigger;
        while ((trigger = m_triggers.GetFirst()) != 0 && trigger->m_time <= m_pSet->m_pBank->m_pSystem->m_clock.GetTime()) {
            m_triggers.RemoveItem(trigger);
            trigger->Fire(this);
            trigger->Schedule(this);
        }
    }
};

int main()
{
    CSoundSystem system = { { 100 } }; CSoundBank bank = { &system }; CAmbientSet set = { &bank };
    CAmbientTrigger late = { 101, 0 }, due2 = { 100, &late }, due1 = { 5, &due2 };
    CAmbientSource s; s.m_pSet = &set; s.m_triggers.m_head = &due1;
    s.ProcessTriggers();
    int ok = g_fired == 2 && g_scheduled == 2 && s.m_triggers.m_head == &late;
    s.ProcessTriggers();
    ok &= g_fired == 2 && s.m_triggers.m_head == &late;
    system.m_clock.m_now = 101;
    s.ProcessTriggers();
    ok &= g_fired == 3 && g_scheduled == 3 && s.m_triggers.m_head == 0;
    if (!ok) { std::printf("00c46f90_TEST FAIL\n"); return 1; }
    std::printf("00c46f90_TEST PASS\n");
    return 0;
}