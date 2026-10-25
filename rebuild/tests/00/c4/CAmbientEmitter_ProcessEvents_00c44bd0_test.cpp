// Behaviour test for 00c44bd0: CAmbientEmitter::ProcessEvents -- fire due events (unless muted), each reschedules itself
#include <cstdio>

struct CAmbientEmitter;
static int g_triggered = 0, g_rescheduled = 0;
struct CAmbientEvent {
    unsigned int m_time; CAmbientEvent* m_next;
    void Trigger(CAmbientEmitter*) { ++g_triggered; }
    void Reschedule(CAmbientEmitter*) { ++g_rescheduled; }
};
struct CClock { unsigned int m_now; unsigned int GetTime() const { return m_now; } };
struct CSoundSystem { CClock m_clock; };
struct CSoundBank { CSoundSystem* m_pSystem; bool m_bMuted; };
struct CAmbientScene { CSoundBank* m_pBank; };
struct CAmbientZone { CAmbientScene* m_pScene; };
struct CList {
    CAmbientEvent* m_head;
    CAmbientEvent* GetFirst() { return m_head; }
    void RemoveItem(CAmbientEvent* e) { if (m_head == e) m_head = e->m_next; }
};
struct CAmbientEmitter {
    CAmbientZone* m_pZone;
    CList m_events;
    CSoundBank* GetBank() const { return m_pZone->m_pScene->m_pBank; }
    void ProcessEvents()
    {
        CAmbientEvent* event;
        while ((event = m_events.GetFirst()) != 0 && event->m_time <= GetBank()->m_pSystem->m_clock.GetTime()) {
            m_events.RemoveItem(event);
            if (!GetBank()->m_bMuted)
                event->Trigger(this);
            event->Reschedule(this);
        }
    }
};

int main()
{
    CSoundSystem system = { { 100 } }; CSoundBank bank = { &system, false };
    CAmbientScene scene = { &bank }; CAmbientZone zone = { &scene };
    CAmbientEvent late = { 150, 0 }, due2 = { 100, &late }, due1 = { 40, &due2 };
    CAmbientEmitter e; e.m_pZone = &zone; e.m_events.m_head = &due1;
    e.ProcessEvents();
    int ok = g_triggered == 2 && g_rescheduled == 2 && e.m_events.m_head == &late;
    bank.m_bMuted = true; system.m_clock.m_now = 200;
    e.ProcessEvents();
    ok &= g_triggered == 2 && g_rescheduled == 3 && e.m_events.m_head == 0;
    if (!ok) { std::printf("00c44bd0_TEST FAIL\n"); return 1; }
    std::printf("00c44bd0_TEST PASS\n");
    return 0;
}