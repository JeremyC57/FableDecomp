// Behaviour test (0x00c216d0): CSoundEvent::YieldPhysicalPlayer checks state 6, stops the physical channel and waits for the player with it
#include <cstdio>
struct CSoundEventManager { int Id; };
struct CSoundChannel { int Id; };
static int g_step = 0, g_check = 0, g_stop = 0, g_wait = 0; static long g_expected = -1;
static CSoundChannel g_channel; static CSoundChannel* g_waited = 0; static CSoundEventManager* g_mgrStop = 0; static CSoundEventManager* g_mgrWait = 0;
class CSoundEvent {
public:
    unsigned short m_state;
    void CheckState(long expected) { g_check = ++g_step; g_expected = expected; }
    CSoundChannel* StopPhysical(CSoundEventManager* manager) { g_stop = ++g_step; g_mgrStop = manager; return &g_channel; }
    void WaitForPhysicalPlayer(CSoundEventManager* manager, CSoundChannel* channel) { g_wait = ++g_step; g_mgrWait = manager; g_waited = channel; }
    void YieldPhysicalPlayer(CSoundEventManager* manager)
    {
        CheckState(6);
        WaitForPhysicalPlayer(manager, StopPhysical(manager));
    }
};
int main()
{
    CSoundEventManager mgr; mgr.Id = 1;
    CSoundEvent e; e.m_state = 6;
    e.YieldPhysicalPlayer(&mgr);
    if (g_check != 1 || g_expected != 6 || g_stop != 2 || g_wait != 3 || g_mgrStop != &mgr || g_mgrWait != &mgr || g_waited != &g_channel)
        { std::printf("00c216d0_TEST FAIL\n"); return 1; }
    std::printf("00c216d0_TEST PASS\n");
    return 0;
}