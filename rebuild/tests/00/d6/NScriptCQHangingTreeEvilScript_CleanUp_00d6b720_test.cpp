#include <cstdio>
#include <cstring>
// behaviour test: CQ_HangingTreeEvilScript::CleanUp -- remove every HTE_FreedomFighter, then (unless the thread is
// ending) the prisoner and the two guards if they are still alive
static int g_removed[16]; static int g_frames = 0, g_endAt = 1000;
struct CScriptThing { int id; bool alive; bool IsAlive() const { return alive; } };
static CScriptThing Make(int id, bool alive) { CScriptThing t; t.id = id; t.alive = alive; return t; }
static bool g_guard2Alive = true;
struct CScriptThingVector {
    CScriptThing Items[8]; unsigned int Count;
    CScriptThingVector() : Count(0) { }
    void push_back(const CScriptThing& t) { Items[Count++] = t; }
    unsigned int size() const { return Count; }
    const CScriptThing& operator[](unsigned int i) const { return Items[i]; }
};
struct CGameScriptInterface {
    CScriptThing GetThingWithScriptName(const char* n) const
    { return !std::strcmp(n, "GUARD1") ? Make(1, false) : !std::strcmp(n, "GUARD2") ? Make(2, g_guard2Alive) : Make(3, true); }
    void GetAllThingsWithScriptName(const char* n, CScriptThingVector& v) const
    { if (!std::strcmp(n, "HTE_FreedomFighter")) { v.push_back(Make(10, true)); v.push_back(Make(11, false)); } }
    void RemoveThing(const CScriptThing& t, bool a, bool b) const { if (!a && b) ++g_removed[t.id]; }
};
struct CQ_HangingTreeEvilScript {
    CGameScriptInterface* Game; CScriptThing Prisoner;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void CleanUp();
};
void CQ_HangingTreeEvilScript::CleanUp()
{
    CScriptThing guard1 = Game->GetThingWithScriptName("GUARD1");
    CScriptThing guard2 = Game->GetThingWithScriptName("GUARD2");
    CScriptThingVector freedomFighters;
    Game->GetAllThingsWithScriptName("HTE_FreedomFighter", freedomFighters);
    for (unsigned int i = 0; i < freedomFighters.size(); ++i) {
        if (IsActiveThreadTerminating()) return;
        Game->RemoveThing(freedomFighters[i], false, true);
    }
    if (IsActiveThreadTerminating()) return;
    if (Prisoner.IsAlive()) Game->RemoveThing(Prisoner, false, true);
    if (guard1.IsAlive()) Game->RemoveThing(guard1, false, true);
    if (guard2.IsAlive()) Game->RemoveThing(guard2, false, true);
}
int main()
{
    CGameScriptInterface game; CQ_HangingTreeEvilScript s; s.Game = &game; s.Prisoner = Make(4, true);
    s.CleanUp();
    bool ok = g_removed[10] == 1 && g_removed[11] == 1 && g_removed[4] == 1 && g_removed[1] == 0 && g_removed[2] == 1;
    g_endAt = 0; std::memset(g_removed, 0, sizeof(g_removed));
    s.CleanUp();                                        // thread ending: nothing removed
    ok = ok && g_removed[10] == 0 && g_removed[4] == 0 && g_removed[2] == 0;
    if (!ok) { std::printf("00d6b720_TEST FAIL\n"); return 1; }
    std::printf("00d6b720_TEST PASS\n");
    return 0;
}