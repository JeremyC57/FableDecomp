// Behaviour test for CSoundEvent::Stop (0x00C21620): returns to idle when the state query says waiting/playing (2..6),
// leaves its group (if any) and the manager lists (the listening one only when flagged), resets the sample reference
// and fields, and goes back into the manager's event set -- in that order.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }
struct CSoundEvent;
struct CSoundEventList { const char* Name; virtual void Remove(CSoundEvent*) { Log(Name); } };
struct CSoundEventSet { void Add(CSoundEvent*) { Log("list"); } };
struct CSoundEventManager { CSoundEventSet m_eventSet; CSoundEventList m_list104, m_list1C4, m_list1F4; };
struct CSteeringGroup { void RemoveAgent(CSoundEventManager*, CSoundEvent*) { Log("registry"); } };
struct CSampleRef { void Reset() { Log("reset"); } };
struct CSoundEvent {
    long Index; CSampleRef m_sample; long Pending; CSteeringGroup* m_pGroup; unsigned short m_state; bool m_listening; long Event;
    void CheckState(long event)
    {
        switch (m_state) {
        case 0: Event = event; break; case 1: Event = 1; break; case 2: Event = 2; break; case 3: Event = 3; break;
        case 4: Event = 4; break; case 5: Event = 5; break; case 6: Event = 6; break;
        }
    }
    long IsWaitingOrPlayingPhysical() { CheckState(-1); long state = m_state; if (state >= 2 && (state <= 4 || state == 6)) return 1; return 0; }
    long IsPlayingVirtual_00c207a0() { CheckState(-1); return m_state == 5; }
    void ReturnToIdle(CSoundEventManager*) { Log("attach"); }
    void Stop(CSoundEventManager* manager);
};
void CSoundEvent::Stop(CSoundEventManager* manager)
{
    CheckState(-1);
    if (IsWaitingOrPlayingPhysical() || IsPlayingVirtual_00c207a0())
        ReturnToIdle(manager);
    CheckState(1);
    if (m_pGroup)
        m_pGroup->RemoveAgent(manager, this);
    if (m_listening)
        manager->m_list1F4.Remove(this);
    manager->m_list1C4.Remove(this);
    manager->m_list104.Remove(this);
    m_sample.Reset();
    Pending = 0;
    m_state = 0;
    Index = -1;
    manager->m_eventSet.Add(this);
}
int main()
{
    CSoundEventManager manager; manager.m_list104.Name = "A"; manager.m_list1C4.Name = "B"; manager.m_list1F4.Name = "C";
    CSteeringGroup registry;
    CSoundEvent event; event.Index = 3; event.Pending = 7; event.m_pGroup = 0; event.m_state = 1; event.m_listening = false;
    g_log[0] = 0; event.Stop(&manager);
    if (std::strcmp(g_log, "B,A,reset,list,") || event.Index != -1 || event.Pending != 0 || event.m_state != 0) { std::printf("00c21620_TEST FAIL\n"); return 1; }
    event.m_state = 5; event.m_pGroup = &registry; event.m_listening = true;
    g_log[0] = 0; event.Stop(&manager);
    if (std::strcmp(g_log, "attach,registry,C,B,A,reset,list,")) { std::printf("00c21620_TEST FAIL\n"); return 1; }
    event.m_state = 3; event.m_pGroup = 0; event.m_listening = false;
    g_log[0] = 0; event.Stop(&manager);
    if (std::strcmp(g_log, "attach,B,A,reset,list,")) { std::printf("00c21620_TEST FAIL\n"); return 1; }
    std::printf("00c21620_TEST PASS\n");
    return 0;
}