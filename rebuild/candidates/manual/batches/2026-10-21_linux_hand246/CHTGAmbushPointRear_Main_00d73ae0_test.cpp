// Behaviour test for CHTG_AmbushPointRear::Main @ 0x00D73AE0 (copy of the model with a scripted stub world).
#include <cstdio>
struct C3DVector { float x, y, z; };
static int g_frame, g_doneAt, g_spawned, g_pauses, g_onScreenUntil;
static float g_dist[64];
struct CScriptThing { C3DVector pos; const C3DVector& GetPos() const { return pos; } int GetDataString() const { return 7; } };
static bool IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float d) { return g_dist[g_frame] < d; }
static bool IsDistanceBetweenThingsOver(const CScriptThing&, const CScriptThing&, float d) { return g_dist[g_frame] > d; }
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frame; }
    void Pause(float) const { ++g_pauses; ++g_frame; }
    bool IsCameraPosOnScreen(const C3DVector&) const { return g_frame < g_onScreenUntil; }
};
struct CQ_HangingTreeGoodScript { CScriptThing m_trackedThing; void SpawnEnemies(const C3DVector&, int name, long count) { if (name == 7 && count == 4) ++g_spawned; } };
struct CHTG_AmbushPointRear {
    CGameScriptInterface* Game; CScriptThing Me; CQ_HangingTreeGoodScript* ParentClass;
    bool IsDone() { return g_frame >= g_doneAt; }
    void Main()
    {
        do {
            Game->NewScriptFrame();
            if (IsDone()) return;
            while (!IsDistanceBetweenThingsUnder(Me, ParentClass->m_trackedThing, 15.0f)) { Game->NewScriptFrame(); if (IsDone()) return; }
            if (IsDone()) return;
            while (!IsDistanceBetweenThingsOver(Me, ParentClass->m_trackedThing, 20.0f)) { Game->NewScriptFrame(); if (IsDone()) return; }
            if (IsDone()) return;
            while (Game->IsCameraPosOnScreen(Me.GetPos())) { Game->NewScriptFrame(); if (IsDone()) return; }
            if (IsDone()) return;
            Game->Pause(1.0f);
        } while (Game->IsCameraPosOnScreen(Me.GetPos()));
        if (IsDone()) return;
        ParentClass->SpawnEnemies(Me.GetPos(), Me.GetDataString(), 4);
        IsDone();
    }
};
static void reset(int doneAt, int onScreenUntil)
{
    g_frame = 0; g_doneAt = doneAt; g_spawned = 0; g_pauses = 0; g_onScreenUntil = onScreenUntil;
    for (int i = 0; i < 64; ++i) g_dist[i] = 30.0f;          // far
    g_dist[3] = 10.0f; g_dist[4] = 10.0f;                    // near at frames 3..4, far again from 5
}
int main()
{
    int bad = 0;
    CGameScriptInterface game; CQ_HangingTreeGoodScript quest; CHTG_AmbushPointRear s; s.Game = &game; s.ParentClass = &quest;
    reset(100, 0); s.Main(); if (g_spawned != 1 || g_pauses != 1) bad++;
    reset(100, 9); s.Main(); if (g_spawned != 1 || g_pauses != 1 || g_frame < 9) bad++;
    reset(2, 0); s.Main(); if (g_spawned != 0) bad++;                 // done while waiting to get near
    reset(100, 0); for (int i = 0; i < 64; ++i) g_dist[i] = 10.0f; g_doneAt = 20; s.Main(); if (g_spawned != 0) bad++;   // never leaves
    if (bad) { std::printf("00d73ae0_TEST FAIL\n"); return 1; }
    std::printf("00d73ae0_TEST PASS\n");
    return 0;
}
