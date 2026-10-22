#include <cstdio>
// behaviour test: CQ_MinionCampScript::RunFirstWaveControl -- while in stage 2, whenever neither hero attacker is
// alive, pick the two minion guards nearest the hero that are not already attacking him and make them the attackers
static int g_frames = 0, g_endAt = 1000, g_added[8], g_addCount = 0;
struct CScriptThing { int id; bool null, alive; bool IsNull() const { return null; } bool IsAlive() const { return alive; } };
static CScriptThing g_guards[5];
static float g_distance[5] = { 9.0f, 3.0f, 7.0f, 1.0f, 2.0f };
static bool g_assigned[5] = { false, false, false, true, false };
struct CThings {
    CScriptThing Items[8]; unsigned int Count;
    unsigned int size() const { return Count; }
    CScriptThing& operator[](unsigned int i) { return Items[i]; }
};
struct CQ_MinionCampScript;
struct CGameScriptInterface {
    CQ_MinionCampScript* Owner;
    void NewScriptFrame() const;
    void GetAllThingsWithScriptName(const char*, CThings& v) const { v.Count = 5; for (int i = 0; i < 5; ++i) v.Items[i] = g_guards[i]; }
};
struct CQ_MinionCampScript {
    CGameScriptInterface* Game; long CurrentStage; CScriptThing HeroAttackerOne, HeroAttackerTwo;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    bool IsAssignedAsHeroAttacker(const CScriptThing& t) const { return g_assigned[t.id]; }
    float GetDistanceToHero(const CScriptThing& t) const { return g_distance[t.id]; }
    void AddNewHeroAttacker(const CScriptThing& t)
    { g_added[g_addCount++] = t.id; if (HeroAttackerOne.alive) HeroAttackerTwo = t; else HeroAttackerOne = t; }
    void RunFirstWaveControl();
};
void CGameScriptInterface::NewScriptFrame() const { if (++g_frames == 3) Owner->CurrentStage = 3; }
void CQ_MinionCampScript::RunFirstWaveControl()
{
    while (CurrentStage == 2) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if ((HeroAttackerOne.IsNull() || !HeroAttackerOne.IsAlive()) && (HeroAttackerTwo.IsNull() || !HeroAttackerTwo.IsAlive())) {
            CThings guards;
            Game->GetAllThingsWithScriptName("MC_MinionGuard", guards);
            unsigned int count = guards.size();
            CScriptThing* nearest = 0; CScriptThing* second = 0;
            float nearestDistance = -1.0f, secondDistance = -1.0f;
            for (unsigned int i = 0; i < count; ++i) {
                if (IsActiveThreadTerminating()) return;
                if (IsAssignedAsHeroAttacker(guards[i])) continue;
                float distance = GetDistanceToHero(guards[i]);
                if (distance < nearestDistance || nearestDistance == -1.0f) {
                    second = nearest; secondDistance = nearestDistance; nearest = &guards[i]; nearestDistance = distance;
                } else if (distance < secondDistance || secondDistance == -1.0f) {
                    second = &guards[i]; secondDistance = distance;
                }
            }
            if (nearest) AddNewHeroAttacker(*nearest);
            if (second) AddNewHeroAttacker(*second);
        }
    }
}
int main()
{
    for (int i = 0; i < 5; ++i) { g_guards[i].id = i; g_guards[i].null = false; g_guards[i].alive = true; }
    CGameScriptInterface game; CQ_MinionCampScript s; game.Owner = &s; s.Game = &game; s.CurrentStage = 2;
    s.HeroAttackerOne.null = true; s.HeroAttackerOne.alive = false; s.HeroAttackerTwo.null = true; s.HeroAttackerTwo.alive = false;
    s.RunFirstWaveControl();                            // guard 3 is nearest but already attacking: picks 4 then 1, once
    bool ok = g_addCount == 2 && g_added[0] == 4 && g_added[1] == 1 && s.HeroAttackerTwo.id == 1 && g_frames == 3;
    if (!ok) { std::printf("00da0820_TEST FAIL\n"); return 1; }
    std::printf("00da0820_TEST PASS\n");
    return 0;
}
