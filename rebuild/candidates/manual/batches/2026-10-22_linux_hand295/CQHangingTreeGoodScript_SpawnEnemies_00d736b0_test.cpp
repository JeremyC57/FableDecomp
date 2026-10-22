#include <cstdio>
#include <cstring>
// behaviour test: CQ_HangingTreeGoodScript::SpawnEnemies -- nothing during state 3 or with a dead leader; during an
// ambush (state 4) wait up to a 10-second timer for the state to change (giving up when it runs out); then create
// <count> HTG_FreedomFighter enemies near pos targeting the leader, have the first one shout the charge line and
// move to state 5
static int g_frames = 0, g_endAt = 1000, g_created = 0, g_lines = 0, g_delays = 0;
static float g_clock = 0;                               // seconds left on the local timer
struct C3DVector { float X, Y, Z; };
struct CScriptThing { int id; bool dead; bool IsDead() const { return dead; } };
struct CTimer { void SetTime(float t) { g_clock = t; } float GetTime() const { return g_clock; } };
struct CThings {
    CScriptThing Items[8]; unsigned int Count;
    CThings() : Count(0) { }
    void clear() { Count = 0; }
    void push_back(const CScriptThing& t) { Items[Count++] = t; }
    unsigned int size() const { return Count; }
    const CScriptThing& front() const { return Items[0]; }
};
static CThings g_world;
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; g_clock -= 1.0f; }
    void SetCreatureCreationDelayFrames(long n) const { if (n == 1) ++g_delays; }
    void ResetCreatureCreationDelayFrames() const { --g_delays; }
    CScriptThing CreateCreatureNearby(const char* def, const C3DVector& p, float r, const char* name) const
    {
        CScriptThing t; t.id = 100 + g_created++; t.dead = false;
        if (!std::strcmp(def, "CREATURE_BANDIT") && p.X == 2.0f && r == 1.0f && !std::strcmp(name, "HTG_FreedomFighter")) g_world.push_back(t);
        return t;
    }
    void GetAllThingsWithScriptName(const char* n, CThings& v) const
    { if (!std::strcmp(n, "HTG_FreedomFighter")) for (unsigned int i = 0; i < g_world.Count; ++i) v.push_back(g_world.Items[i]); }
    void AddLineToConversation(const CScriptThing& speaker, const char* t) const { if (speaker.id == 100 && !std::strcmp(t, "TEXT_QST_029_BANDIT_CHARGE")) ++g_lines; }
};
struct CQ_HangingTreeGoodScript {
    CGameScriptInterface* Game; long HTG_State; CScriptThing Leader; CThings CurrentEnemyVector;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void SpawnEnemies(const C3DVector& pos, const char* defName, long count);
};
void CQ_HangingTreeGoodScript::SpawnEnemies(const C3DVector& pos, const char* defName, long count)
{
    if (HTG_State == 3 || Leader.IsDead()) return;
    if (HTG_State == 4 || HTG_State == 3) {
        CTimer timer;
        timer.SetTime(10);
        while (timer.GetTime() > 0 && (HTG_State == 4 || HTG_State == 3)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (timer.GetTime() <= 0) return;
    }
    Game->SetCreatureCreationDelayFrames(1);
    for (long i = 0; i < count; ++i) {
        if (IsActiveThreadTerminating()) return;
        Game->CreateCreatureNearby(defName, pos, 1.0f, "HTG_FreedomFighter");
    }
    Game->ResetCreatureCreationDelayFrames();
    CurrentEnemyVector.clear();
    Game->GetAllThingsWithScriptName("HTG_FreedomFighter", CurrentEnemyVector);
    if (CurrentEnemyVector.size() != 0) Game->AddLineToConversation(CurrentEnemyVector.front(), "TEXT_QST_029_BANDIT_CHARGE");
    HTG_State = 5;
}
int main()
{
    CGameScriptInterface game; CQ_HangingTreeGoodScript s; s.Game = &game; s.Leader.id = 1; s.Leader.dead = false;
    C3DVector pos; pos.X = 2.0f; pos.Y = 0; pos.Z = 0;
    s.HTG_State = 2;
    s.SpawnEnemies(pos, "CREATURE_BANDIT", 3);
    bool ok = g_created == 3 && s.CurrentEnemyVector.size() == 3 && g_lines == 1 && s.HTG_State == 5 && g_delays == 0 && g_frames == 0;
    s.HTG_State = 4;
    s.SpawnEnemies(pos, "CREATURE_BANDIT", 2);          // the ambush never ends: timer runs out after 10 frames
    ok = ok && g_frames == 10 && g_created == 3 && s.HTG_State == 4;
    s.HTG_State = 3; s.SpawnEnemies(pos, "CREATURE_BANDIT", 2);
    s.HTG_State = 2; s.Leader.dead = true; s.SpawnEnemies(pos, "CREATURE_BANDIT", 2);
    ok = ok && g_created == 3 && s.HTG_State == 2;
    if (!ok) { std::printf("00d736b0_TEST FAIL\n"); return 1; }
    std::printf("00d736b0_TEST PASS\n");
    return 0;
}
