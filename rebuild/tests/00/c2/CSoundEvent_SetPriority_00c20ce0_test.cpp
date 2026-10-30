// behaviour test for 0x00C20CE0 CSoundEvent::SetPriority (copy of the logic over logging stubs)
#include <cstdio>
#include <cstring>
struct CSoundEvent;
static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CEventList {
    const char* m_name;
    void Remove(CSoundEvent*) { char b[32]; std::sprintf(b, "%s-", m_name); Log(b); }
    void Add(CSoundEvent*) { char b[32]; std::sprintf(b, "%s+", m_name); Log(b); }
};
struct CSoundEventManager { CEventList m_waitingForPlayer, m_waitingForMemory, m_byPriority; };
struct CSoundChannel {
    void Start(CSoundEventManager*, long priority) { char b[32]; std::sprintf(b, "ch+%ld", priority); Log(b); }
    void Stop(CSoundEventManager*, long priority) { char b[32]; std::sprintf(b, "ch-%ld", priority); Log(b); }
};
static int g_checks = 0;
struct CSoundEvent {
    long m_priority;
    CSoundChannel* m_pChannel;
    unsigned short m_state;
    void CheckState(long expected) { if (expected == -1) ++g_checks; }
    void SetPriority(CSoundEventManager* manager, long priority);
};
void CSoundEvent::SetPriority(CSoundEventManager* manager, long priority)
{
    CheckState(-1);
    if (priority == m_priority)
        return;
    if (m_state == 3) {
        m_pChannel->Stop(manager, m_priority);
        manager->m_waitingForPlayer.Remove(this);
    } else if (m_state == 4)
        manager->m_waitingForMemory.Remove(this);
    else if (m_state == 6)
        m_pChannel->Stop(manager, m_priority * 2);
    manager->m_byPriority.Remove(this);
    m_priority = priority;
    manager->m_byPriority.Add(this);
    if (m_state == 3) {
        m_pChannel->Start(manager, m_priority);
        manager->m_waitingForPlayer.Add(this);
    } else if (m_state == 4)
        manager->m_waitingForMemory.Add(this);
    else if (m_state == 6)
        m_pChannel->Start(manager, m_priority * 2);
}
static bool Check(CSoundEvent& e, CSoundEventManager& m, unsigned short state, long from, long to, const char* want)
{
    g_log[0] = 0; e.m_state = state; e.m_priority = from;
    e.SetPriority(&m, to);
    return std::strcmp(g_log, want) == 0 && e.m_priority == to;
}
int main()
{
    CSoundEventManager m; m.m_waitingForPlayer.m_name = "P"; m.m_waitingForMemory.m_name = "M"; m.m_byPriority.m_name = "Q";
    CSoundChannel ch; CSoundEvent e; e.m_pChannel = &ch;
    if (!Check(e, m, 3, 2, 5, "ch-2;P-;Q-;Q+;ch+5;P+;") ||
        !Check(e, m, 4, 2, 5, "M-;Q-;Q+;M+;") ||
        !Check(e, m, 6, 2, 5, "ch-4;Q-;Q+;ch+10;") ||
        !Check(e, m, 5, 2, 5, "Q-;Q+;") ||
        !Check(e, m, 3, 5, 5, "") || g_checks != 5)
    { std::printf("00c20ce0_TEST FAIL\n"); return 1; }
    std::printf("00c20ce0_TEST PASS\n");
    return 0;
}