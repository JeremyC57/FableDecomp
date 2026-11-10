// Behaviour test for NScript::CQ_MinionCampScript::CMC_MinionGuard::Main (0x00D9EB30): each frame, when the guard is hit and the
// camp says it is a hero attacker, retarget it onto whoever hit it unless that is already its target (the hero while it has none);
// every step stops when the script thread is terminating.
#include <cstdio>

struct Thing { int id; };
class CScriptThing { public:
    const Thing* p;
    CScriptThing(const Thing* t = 0) : p(t) { }
    bool IsNull() const { return p == 0; }
    bool operator==(const CScriptThing& o) const { return p == o.p; }
    bool operator!=(const CScriptThing& o) const { return !(*this == o); }
};
static Thing g_hero = { 1 }, g_orc = { 2 }, g_wolf = { 3 };
static int g_frames, g_frameLimit, g_terminateAtCheck, g_checks, g_setTargetCalls;
static const Thing* g_hitter;
static bool g_assigned;

struct Game {
    CScriptThing GetHero() { return CScriptThing(&g_hero); }
    void NewScriptFrame() { ++g_frames; }
};
struct CQ_MinionCampScript {
    bool IsAssignedAsHeroAttacker(const CScriptThing&) { return g_assigned; }
};
struct Me_t {
    bool MsgIsHitBy(const char*) { return g_hitter != 0; }
    CScriptThing MsgWhoHitMe() { return CScriptThing(g_hitter); }
};
struct Guard {
    Game* PGame; Me_t Me; CQ_MinionCampScript* ParentClass; CScriptThing Target;
    bool IsActiveThreadTerminating()
    {
        ++g_checks;
        if (g_terminateAtCheck && g_checks >= g_terminateAtCheck) return true;
        return g_frames >= g_frameLimit;
    }
    void SetTarget(const CScriptThing& t) { ++g_setTargetCalls; Target = t; }
    void Main()
    {
        while (!IsActiveThreadTerminating()) {
            if (Me.MsgIsHitBy("")) {
                if (IsActiveThreadTerminating()) return;
                if (ParentClass->IsAssignedAsHeroAttacker(CScriptThing())) {
                    if (IsActiveThreadTerminating()) return;
                    CScriptThing attacker = Me.MsgWhoHitMe();
                    if (Target.IsNull()) {
                        if (IsActiveThreadTerminating()) return;
                        if (attacker != PGame->GetHero()) {
                            if (IsActiveThreadTerminating()) return;
                            SetTarget(attacker);
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        if (attacker != Target) {
                            if (IsActiveThreadTerminating()) return;
                            SetTarget(attacker);
                        }
                    }
                }
            }
            PGame->NewScriptFrame();
        }
    }
};
static void Reset(int frames, const Thing* hitter, bool assigned)
{
    g_frames = 0; g_frameLimit = frames; g_terminateAtCheck = 0; g_checks = 0; g_setTargetCalls = 0;
    g_hitter = hitter; g_assigned = assigned;
}
int main()
{
    Game game; CQ_MinionCampScript camp; bool ok = true;
    Guard g; g.PGame = &game; g.ParentClass = &camp;
    Reset(1, &g_orc, true); g.Target = CScriptThing(); g.Main();                 // no target, hit by an orc: retarget
    if (g.Target.p != &g_orc || g_setTargetCalls != 1 || g_frames != 1) ok = false;
    Reset(1, &g_hero, true); g.Target = CScriptThing(); g.Main();                // no target, hit by the hero: keep none
    if (!g.Target.IsNull() || g_setTargetCalls != 0) ok = false;
    Reset(1, &g_wolf, true); g.Target = CScriptThing(&g_orc); g.Main();          // other target: retarget to the wolf
    if (g.Target.p != &g_wolf || g_setTargetCalls != 1) ok = false;
    Reset(1, &g_wolf, true); g.Target = CScriptThing(&g_wolf); g.Main();         // same target: nothing
    if (g_setTargetCalls != 0) ok = false;
    Reset(3, &g_orc, false); g.Target = CScriptThing(); g.Main();                // not a hero attacker: only frames
    if (g_setTargetCalls != 0 || g_frames != 3) ok = false;
    Reset(5, &g_orc, true); g.Target = CScriptThing(); g_terminateAtCheck = 4; g.Main();   // terminating before SetTarget
    if (g_setTargetCalls != 0 || g_frames != 0) ok = false;
    if (!ok) { std::printf("00d9eb30_TEST FAIL\n"); return 1; }
    std::printf("00d9eb30_TEST PASS\n");
    return 0;
}