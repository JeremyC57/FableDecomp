// Behaviour test for 0x0095B820 CAIStateGroup_HoverHornetQueen::Spawn(): the row's logic over stub classes (C++03).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
static std::string trace;
static void check(bool v, int line) { if (!v) { printf("0095b820_TEST FAIL line %d\n", line); exit(1); } }
class CThingAICreature;
class C2DCoordI { public: long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) { } };
class C3DVector { public: float X, Y, Z; C2DCoordI ToC2DCoordI() const { return C2DCoordI((long)X, (long)Y); } };
class C2DBoxI { public: long TLX, TLY, BRX, BRY;
    C2DBoxI(const C2DCoordI& p, long r) { long x = p.X; long y = p.Y; TLX = x - r; TLY = y - r; BRX = x + r; BRY = y + r; } };
class CThingAICreature { public:
    bool Dead, KillOnUnload, Persistent, LevelPersistent; unsigned long DefIndex; C3DVector Pos; void* Boss;
    bool IsDead() const { return Dead; } const C3DVector& GetPos() const { return Pos; }
    bool IsToKillOnLevelUnload() const { return KillOnUnload; } bool IsLevelPersistent() const { trace += 'P'; return Persistent; }
    void SetAsLevelPersistent(bool p) { LevelPersistent = p; }
    template <class T> bool GetTC(T** out) { *out = (T*)Boss; return true; } };
template <class T> static bool IS_THING_ALIVE(const T& thing) { if (thing != 0) { if (!thing->IsDead()) return true; } return false; }
class CTCBoss { public: std::vector<CThingAICreature*> Helpers; void SetHelperCreature(CThingAICreature& c) { Helpers.push_back(&c); } };
class CThingFilter_HasDefGlobalIndex { public: unsigned long DefGlobalIndex;
    CThingFilter_HasDefGlobalIndex(unsigned long i) : DefGlobalIndex(i) { }
    bool operator()(const CThingAICreature& t) const { return t.DefIndex == DefGlobalIndex; } };
class CGameDefinitionManager { public: static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; }
    long GetDefGlobalIndexFromName(const std::string& name) const { check(name == "CREATURE_HORNET_PICNIC", __LINE__); return 42; } };
static std::vector<CThingAICreature*> world;
class CThingSearchTools { public:
    template <class T, class Container, class Filter> void GetAllThingsInRoughArea(const C2DBoxI& area, const Filter& filter, Container& result) const
    { for (size_t i = 0; i < world.size(); ++i) { T* t = world[i]; if (t->Pos.X >= area.TLX && t->Pos.X <= area.BRX && t->Pos.Y >= area.TLY && t->Pos.Y <= area.BRY && filter(*t)) result.push_back(t); } } };
template <class T> class CIntelligentPointer { public: T* Item; T& operator*() const { return *Item; } };
class CCreatureAction_QueenHornetShootBabies { public: CThingAICreature* Target;
    CCreatureAction_QueenHornetShootBabies(CThingAICreature&, CThingAICreature& target) : Target(&target) { } };
class CAIStateGroup_HoverHornetQueen { public:
    CThingAICreature queen; CIntelligentPointer<CThingAICreature> PTargetThing; CThingSearchTools tools; int runnable;
    CThingAICreature& GetCreature() const { return const_cast<CThingAICreature&>(queen); }
    const CThingSearchTools& PeekThingSearchTools() const { return tools; }
    bool IsStillRunnable() { return runnable-- > 0; }
    void Terminate() { trace += 'X'; }
    int AIAction_PerformActionAndWait(const CCreatureAction_QueenHornetShootBabies& a, bool wait)
    { check(!wait && a.Target == PTargetThing.Item, __LINE__); trace += 'A'; return 2; }
    void Spawn();
};
void CAIStateGroup_HoverHornetQueen::Spawn()
{
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    AIAction_PerformActionAndWait(CCreatureAction_QueenHornetShootBabies(GetCreature(), *PTargetThing), false);
    std::vector<CThingAICreature*> helpers;
    unsigned long def_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_HORNET_PICNIC");
    PeekThingSearchTools().GetAllThingsInRoughArea<CThingAICreature>(C2DBoxI(GetCreature().GetPos().ToC2DCoordI(), 60),
        CThingFilter_HasDefGlobalIndex(def_index), helpers);
    bool level_persistent = false;
    if (!GetCreature().IsToKillOnLevelUnload() || GetCreature().IsLevelPersistent())
        level_persistent = true;
    CTCBoss* boss;
    GetCreature().GetTC(&boss);
    for (unsigned long i = 0; i < helpers.size(); ++i) {
        if (IS_THING_ALIVE(helpers[i])) {
            helpers[i]->SetAsLevelPersistent(level_persistent);
            boss->SetHelperCreature(*helpers[i]);
        }
    }
    if (!IsStillRunnable())
        Terminate();
}

int main()
{
    CTCBoss boss; CThingAICreature target, h1, h2, h3, far1, other;
    memset(&target, 0, sizeof target);
    CThingAICreature* all[] = { &h1, &h2, &h3, &far1, &other };
    for (int i = 0; i < 5; ++i) { memset(all[i], 0, sizeof *all[i]); all[i]->DefIndex = 42; all[i]->Pos.X = 10.0f * i; all[i]->Pos.Y = 5.0f; world.push_back(all[i]); }
    far1.Pos.X = 500.0f; other.DefIndex = 7; h2.Dead = true;
    CAIStateGroup_HoverHornetQueen g; memset(&g.queen, 0, sizeof g.queen);
    g.queen.Boss = &boss; g.queen.Pos.X = 20.0f; g.queen.Pos.Y = 0.0f; g.queen.KillOnUnload = false; g.PTargetThing.Item = &target;
    g.runnable = 2; trace = ""; g.Spawn();
    check(trace == "A" && boss.Helpers.size() == 2 && boss.Helpers[0] == &h1 && boss.Helpers[1] == &h3, __LINE__);
    check(h1.LevelPersistent && h3.LevelPersistent && !h2.LevelPersistent, __LINE__);  // not killed on unload: persistent
    boss.Helpers.clear(); g.queen.KillOnUnload = true; g.queen.Persistent = false; g.runnable = 1; trace = ""; g.Spawn();
    check(trace == "APX" && boss.Helpers.size() == 2 && !h1.LevelPersistent, __LINE__);  // killed on unload, not persistent; then terminate
    boss.Helpers.clear(); g.queen.Persistent = true; g.runnable = 2; trace = ""; g.Spawn();
    check(trace == "AP" && h1.LevelPersistent, __LINE__);
    g.runnable = 0; trace = ""; g.Spawn();
    check(trace == "X", __LINE__);
    printf("0095b820_TEST PASS\n");
    return 0;
}