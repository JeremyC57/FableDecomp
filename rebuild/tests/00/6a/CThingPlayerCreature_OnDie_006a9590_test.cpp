#include <cstdio>
struct CWorldState { long m_value80; };
static CWorldState g_world;
static int g_crimes = 0, g_baseDies = 0;
struct CTCVillage { void ProcessCrimeDeeds() { ++g_crimes; } };
struct CThingCreatureBase;
struct CPlayer {
    long PlayerRespawnDelay; bool m_main; CTCVillage* m_village; CThingCreatureBase* m_lastDied;
    void GlobalEventCreatureDied(CThingCreatureBase* creature) { m_lastDied = creature; }
    bool IsMainPlayer() { return m_main; }
    CTCVillage* GetPNearestTCVillage() { return m_village; }
};
struct CThingCreatureBase {
    CPlayer* m_player;
    CPlayer*     GetPlayer() { return m_player; }
    CWorldState* GetWorldState() { return &g_world; }
    void         OnDie() { ++g_baseDies; }
};
struct CThingPlayerCreature : CThingCreatureBase {
    void OnDie();
};
void CThingPlayerCreature::OnDie()
{
    CPlayer* player = GetPlayer();
    player->GlobalEventCreatureDied(this);
    if (player->IsMainPlayer())
    {
        player->PlayerRespawnDelay = GetWorldState()->m_value80;
        CTCVillage* village = player->GetPNearestTCVillage();
        if (village)
            village->ProcessCrimeDeeds();
    }
    CThingCreatureBase::OnDie();
}
int main()
{
    int bad = 0;
    g_world.m_value80 = 1234;
    CPlayer p; p.PlayerRespawnDelay = 0; p.m_main = false; p.m_village = 0; p.m_lastDied = 0;
    CThingPlayerCreature c; c.m_player = &p;
    c.OnDie();
    if (p.m_lastDied != &c || p.PlayerRespawnDelay != 0 || g_baseDies != 1) bad = 1;
    p.m_main = true;
    c.OnDie();
    if (p.PlayerRespawnDelay != 1234 || g_crimes != 0 || g_baseDies != 2) bad = 2;
    CTCVillage v; p.m_village = &v;
    c.OnDie();
    if (g_crimes != 1 || g_baseDies != 3) bad = 3;
    if (bad) { std::printf("006a9590_TEST FAIL\n"); return 1; }
    std::printf("006a9590_TEST PASS\n");
    return 0;
}