#include <cstdio>
struct CThingCreatureBase { int x; };
static long g_frames = 0; static CThingCreatureBase* g_def = 0; static CThingCreatureBase* g_att = 0;
struct CCombatManager { long zone; long GetDefenderZone(CThingCreatureBase* d, CThingCreatureBase* a, long f) { g_def = d; g_att = a; g_frames = f; return zone; } };
static CCombatManager g_mgr; CCombatManager* GetCombatManager() { return &g_mgr; }
struct CSeq {
    virtual long GetZone() { return mask; } virtual long GetFramesToPredictTargetPosition() { return 7; }
    long mask; CThingCreatureBase* Creature; CThingCreatureBase* Target;
    bool IsZoneValid() { long zone = GetCombatManager()->GetDefenderZone(Target, Creature, GetFramesToPredictTargetPosition()); return (zone & GetZone()) != 0 ? true : false; }
};
int main()
{
    CThingCreatureBase c, t; CSeq s; s.Creature = &c; s.Target = &t; s.mask = 6; g_mgr.zone = 4;
    if (!s.IsZoneValid() || g_def != &t || g_att != &c || g_frames != 7) { std::printf("008b0610_TEST FAIL\n"); return 1; }
    g_mgr.zone = 1;
    if (s.IsZoneValid()) { std::printf("008b0610_TEST FAIL\n"); return 1; }
    std::printf("008b0610_TEST PASS\n");
    return 0;
}
