// Behaviour test for 0x00C208E0 CSoundEvent::StopWaitingForMemory: state check for 4 first, the manager's
// waiting-for-memory list removes us, the virtual player reads the level into our record, final state word 1.
#include <cstdio>
struct SEventLevel { float m_gain; long m_muted; };
static int g_order = 0;
static int g_seenState = -1;
struct CSoundEvent;
struct CEventList {
    CSoundEvent* m_removed; int m_when;
    virtual void Remove(CSoundEvent* event) { m_removed = event; m_when = ++g_order; }
};
struct CSoundEventManager { CEventList m_waitingForMemory; };
struct CSoundPlayer {
    CSoundEventManager* m_manager; SEventLevel* m_level; int m_when;
    virtual void ReadLevel(CSoundEventManager* manager, SEventLevel* level) { m_manager = manager; m_level = level; m_when = ++g_order; }
};
struct CSoundEvent {
    SEventLevel m_level;
    CSoundPlayer m_virtualPlayer;
    unsigned short m_state;
    int m_checkWhen;
    void CheckState(long expected) { g_seenState = (int)expected; m_checkWhen = ++g_order; }
    void StopWaitingForMemory(CSoundEventManager* manager)
    {
        CheckState(4);
        manager->m_waitingForMemory.Remove(this);
        m_virtualPlayer.ReadLevel(manager, &m_level);
        m_state = 1;
    }
};
int main()
{
    CSoundEventManager manager; manager.m_waitingForMemory.m_removed = 0;
    CSoundEvent event; event.m_state = 4;
    event.StopWaitingForMemory(&manager);
    if (g_seenState != 4 || event.m_checkWhen != 1 || manager.m_waitingForMemory.m_removed != &event || manager.m_waitingForMemory.m_when != 2) { std::printf("00c208e0_TEST FAIL\n"); return 1; }
    if (event.m_virtualPlayer.m_manager != &manager || event.m_virtualPlayer.m_level != &event.m_level || event.m_virtualPlayer.m_when != 3 || event.m_state != 1) { std::printf("00c208e0_TEST FAIL\n"); return 1; }
    std::printf("00c208e0_TEST PASS\n");
    return 0;
}
