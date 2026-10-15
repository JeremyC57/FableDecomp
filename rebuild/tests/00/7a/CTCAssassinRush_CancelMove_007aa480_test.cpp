// Behaviour test for 0x007AA480 CTCAssassinRush::CancelMove: a locked main player only schedules the
// cancel; otherwise the effects are cleaned up, TC 0xA9 is removed from the parent and the flag cleared.
#include <cstdio>

static int g_log[8];
static int g_n;
static long g_removed;
static void* g_removedFrom;

struct CThing { void RemoveTC(long type) { g_log[g_n++] = 2; g_removed = type; g_removedFrom = this; } };
struct CPlayer { bool m_bCancelLocked; };
struct CPlayerManager { CPlayer* m_pPlayer; CPlayer* GetMainPlayer() { return m_pPlayer; } };
static CPlayerManager g_manager;
static CPlayerManager* GetPlayerManager() { return &g_manager; }

struct CTCAssassinRush {
    CThing* ParentThing;
    bool    ScheduleForCancel;
    void CleanUpEffects() { g_log[g_n++] = 1; }
    void CancelMove();
};

void CTCAssassinRush::CancelMove()
{
    CThing* parent = ParentThing;
    if (!GetPlayerManager()->GetMainPlayer()->m_bCancelLocked) {
        CleanUpEffects();
        parent->RemoveTC(0xa9);
        ScheduleForCancel = false;
    } else {
        ScheduleForCancel = true;
    }
}

int main()
{
    CThing parent;
    CPlayer player;
    g_manager.m_pPlayer = &player;
    CTCAssassinRush rush;
    rush.ParentThing = &parent;
    player.m_bCancelLocked = true;
    rush.ScheduleForCancel = false;
    g_n = 0;
    rush.CancelMove();
    if (!rush.ScheduleForCancel || g_n != 0) { std::printf("007aa480_TEST FAIL\n"); return 1; }
    player.m_bCancelLocked = false;
    rush.CancelMove();
    if (rush.ScheduleForCancel || g_n != 2 || g_log[0] != 1 || g_log[1] != 2 || g_removed != 0xa9 || g_removedFrom != &parent) { std::printf("007aa480_TEST FAIL\n"); return 1; }
    std::printf("007aa480_TEST PASS\n");
    return 0;
}