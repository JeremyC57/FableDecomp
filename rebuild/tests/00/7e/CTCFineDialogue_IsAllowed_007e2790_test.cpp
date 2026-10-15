#include <cstdio>
struct CPlayerRecord { bool m_busy; };
struct CPlayerManager { CPlayerRecord* m_pMainPlayer; };
struct CGameUI { bool m_blockingF6; bool m_blockingFD; };
struct CWorld { long m_mode; };
static CPlayerRecord g_player; static CPlayerManager g_manager; static CGameUI g_ui; static CWorld g_world;
static CGameUI* g_pGameUI = &g_ui;
struct CTCFineDialogue {
    void* vptrSlot; void* ParentThing; long Fine; long Money; bool GivePlayerChanceToPayFine; bool WaitingForConfirmation;
    CPlayerManager* GetGameSubsystem1C() { return &g_manager; }
    CWorld* GetWorld() { return &g_world; }
    bool IsAllowed()
    {
        if (!GivePlayerChanceToPayFine) return false;
        if (GetGameSubsystem1C()->m_pMainPlayer->m_busy) return false;
        if (g_pGameUI->m_blockingFD) return false;
        if (g_pGameUI->m_blockingF6) return false;
        long mode = GetWorld()->m_mode;
        if (mode == 0 || mode == 9) return true;
        return false;
    }
};
int main()
{
    g_manager.m_pMainPlayer = &g_player; g_player.m_busy = false; g_ui.m_blockingF6 = false; g_ui.m_blockingFD = false; g_world.m_mode = 0;
    CTCFineDialogue d; d.GivePlayerChanceToPayFine = false;
    bool r0 = d.IsAllowed();
    d.GivePlayerChanceToPayFine = true; bool r1 = d.IsAllowed();
    g_world.m_mode = 9; bool r2 = d.IsAllowed();
    g_world.m_mode = 4; bool r3 = d.IsAllowed();
    g_world.m_mode = 0; g_player.m_busy = true; bool r4 = d.IsAllowed();
    g_player.m_busy = false; g_ui.m_blockingFD = true; bool r5 = d.IsAllowed();
    g_ui.m_blockingFD = false; g_ui.m_blockingF6 = true; bool r6 = d.IsAllowed();
    if (r0 || !r1 || !r2 || r3 || r4 || r5 || r6) { std::printf("007e2790_TEST FAIL\n"); return 1; }
    std::printf("007e2790_TEST PASS\n");
    return 0;
}