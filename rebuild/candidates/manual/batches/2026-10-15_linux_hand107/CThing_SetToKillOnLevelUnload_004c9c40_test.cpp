// Behaviour test for CThing::SetToKillOnLevelUnload @ 0x004C9C40: change flag; newly set + in limbo + map not loaded -> Kill(false).
#include <cstdio>
struct CMapManager {
    bool loaded; long asked;
    virtual bool IsMapLoaded(long mapNumber) { asked = mapNumber; return loaded; }
};
struct CThingManager { CMapManager* m_pMapManager; };
static CMapManager g_maps;
static CThingManager g_manager;
static CThingManager* g_pThingManager = &g_manager;
struct CThing {
    short CurrentMapNumber;
    unsigned char InLimbo : 1;
    unsigned char KillOnLevelUnload : 1;
    int kills; bool lastKill;
    void Kill(bool immediately) { ++kills; lastKill = immediately; }
    void SetToKillOnLevelUnload(bool kill)
    {
        if (KillOnLevelUnload != kill) {
            KillOnLevelUnload = kill;
            if (KillOnLevelUnload && InLimbo && !g_pThingManager->m_pMapManager->IsMapLoaded(CurrentMapNumber))
                Kill(false);
        }
    }
};
int main()
{
    g_manager.m_pMapManager = &g_maps; g_maps.loaded = false; g_maps.asked = 0;
    CThing t; t.CurrentMapNumber = -3; t.InLimbo = 1; t.KillOnLevelUnload = 0; t.kills = 0; t.lastKill = true;
    t.SetToKillOnLevelUnload(true);
    if (!t.KillOnLevelUnload || t.kills != 1 || t.lastKill || g_maps.asked != -3) { std::printf("004c9c40_TEST FAIL\n"); return 1; }
    t.SetToKillOnLevelUnload(true);
    if (t.kills != 1) { std::printf("004c9c40_TEST FAIL\n"); return 1; }
    t.SetToKillOnLevelUnload(false);
    if (t.KillOnLevelUnload || t.kills != 1) { std::printf("004c9c40_TEST FAIL\n"); return 1; }
    g_maps.loaded = true; t.SetToKillOnLevelUnload(true);
    if (t.kills != 1) { std::printf("004c9c40_TEST FAIL\n"); return 1; }
    t.SetToKillOnLevelUnload(false); g_maps.loaded = false; t.InLimbo = 0; t.SetToKillOnLevelUnload(true);
    if (t.kills != 1 || !t.KillOnLevelUnload) { std::printf("004c9c40_TEST FAIL\n"); return 1; }
    std::printf("004c9c40_TEST PASS\n");
    return 0;
}
