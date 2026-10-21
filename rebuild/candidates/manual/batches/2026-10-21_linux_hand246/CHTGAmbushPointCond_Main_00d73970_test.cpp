// Behaviour test for CHTG_AmbushPointCond::Main @ 0x00D73970 (copy of the model with a scripted stub world).
#include <cstdio>
#include <cstring>
struct C3DVector { float x, y, z; };
static C3DVector g_ZeroVector = { 0, 0, 0 };
static int g_frame, g_doneAt, g_nearAt, g_offScreenAt, g_spawned, g_pauses, g_removed;
static bool g_spawnPosOk;
struct CScriptThing {
    int id; const C3DVector* pos;
    const C3DVector& GetPos() const { return pos ? *pos : g_ZeroVector; }
    int GetDataString() const { return id * 10; }
};
static C3DVector g_markerPos = { 1, 2, 3 };
static bool IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float d) { return d == 6.0f && g_frame >= g_nearAt; }
struct CGameScriptInterface {
    CScriptThing hero;
    void NewScriptFrame() const { ++g_frame; }
    const CScriptThing& GetHero() const { return hero; }
    CScriptThing GetNearestWithScriptName(const CScriptThing&, const char* name) const
    { CScriptThing t; t.id = std::strcmp(name, "HTG_AmbushPointCond") == 0 ? 5 : 0; t.pos = &g_markerPos; return t; }
    void RemoveThing(const CScriptThing& t, bool a, bool b) const { if (!a && b) g_removed += t.id; }
    void Pause(float s) const { if (s == 1.0f) ++g_pauses; }
    bool IsCameraPosOnScreen(const C3DVector&) const { return g_frame < g_offScreenAt; }
};
struct CQ_HangingTreeGoodScript {
    void SpawnEnemies(const C3DVector& p, int name, long count) { g_spawnPosOk = p.z == 3.0f; if (name == 50 && count == 4) ++g_spawned; }
};
struct CHTG_AmbushPointCond {
    CGameScriptInterface* Game; CScriptThing Me; CQ_HangingTreeGoodScript* ParentClass;
    bool IsDone() { return g_frame >= g_doneAt; }
    void Main()
    {
        while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 6.0f)) { Game->NewScriptFrame(); if (IsDone()) return; }
        if (IsDone()) return;
        CScriptThing marker = Game->GetNearestWithScriptName(Me, "HTG_AmbushPointCond");
        while (Game->IsCameraPosOnScreen(marker.GetPos())) { Game->NewScriptFrame(); if (IsDone()) return; }
        if (IsDone()) return;
        Game->Pause(1.0f);
        ParentClass->SpawnEnemies(marker.GetPos(), marker.GetDataString(), 4);
        Game->RemoveThing(marker, false, true);
        Game->RemoveThing(Me, false, true);
    }
};
static void reset(int doneAt, int nearAt, int offAt) { g_frame = 0; g_doneAt = doneAt; g_nearAt = nearAt; g_offScreenAt = offAt; g_spawned = g_pauses = g_removed = 0; g_spawnPosOk = false; }
int main()
{
    int bad = 0;
    CGameScriptInterface game; CQ_HangingTreeGoodScript quest; CHTG_AmbushPointCond s;
    game.hero.id = 9; game.hero.pos = 0; s.Game = &game; s.ParentClass = &quest; s.Me.id = 100; s.Me.pos = 0;
    reset(100, 3, 6); s.Main(); if (g_spawned != 1 || g_pauses != 1 || g_removed != 105 || !g_spawnPosOk) bad++;
    reset(2, 5, 0); s.Main(); if (g_spawned != 0 || g_removed != 0) bad++;
    reset(8, 0, 50); s.Main(); if (g_spawned != 0 || g_pauses != 0) bad++;
    if (bad) { std::printf("00d73970_TEST FAIL\n"); return 1; }
    std::printf("00d73970_TEST PASS\n");
    return 0;
}
