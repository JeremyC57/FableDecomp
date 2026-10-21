// Behaviour test for CM_Guard::Main @ 0x00DA63F0 (copy of the model with a scripted stub world).
#include <cstdio>
#include <cstring>
struct C3DVector { float x, y, z; };
static int g_frame, g_doneAt, g_nearAt, g_created, g_removed;
static char g_tag[8];
struct CScriptThing {
    bool real; C3DVector pos;
    const C3DVector& GetPos() const { return pos; }
    void SetDataString(const char* t) { if (real) std::strcpy(g_tag, t); }
};
static bool IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float d) { return d == 20.0f && g_frame >= g_nearAt; }
struct CGameScriptInterface {
    CScriptThing hero;
    void NewScriptFrame() const { ++g_frame; }
    const CScriptThing& GetHero() const { return hero; }
    CScriptThing CreateCreature(const char* def, const C3DVector& p, const char* name, bool f) const
    {
        CScriptThing t; t.real = !std::strcmp(def, "CREATURE_BS_GUARD") && !std::strcmp(name, "GuardToLeadTheWay") && !f && p.x == 5.0f;
        t.pos = p; if (t.real) ++g_created; return t;
    }
    void RemoveThing(const CScriptThing& t, bool a, bool b) const { if (!t.real && !a && b) ++g_removed; }
};
struct CQ_MinionClifftopChaseScript { bool FirstGuardTagged; };
struct CM_Guard {
    CGameScriptInterface* Game; CScriptThing Me; CQ_MinionClifftopChaseScript* ParentClass;
    bool IsDone() { return g_frame >= g_doneAt; }
    void Main()
    {
        while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 20.0f)) { Game->NewScriptFrame(); if (IsDone()) return; }
        if (IsDone()) return;
        CScriptThing guard = Game->CreateCreature("CREATURE_BS_GUARD", Me.GetPos(), "GuardToLeadTheWay", false);
        if (!ParentClass->FirstGuardTagged) { if (IsDone()) return; guard.SetDataString("MCC4"); ParentClass->FirstGuardTagged = true; }
        else { if (IsDone()) return; guard.SetDataString("MCC5"); }
        Game->RemoveThing(Me, false, true);
    }
};
int main()
{
    int bad = 0;
    CGameScriptInterface game; game.hero.real = false; CQ_MinionClifftopChaseScript q; q.FirstGuardTagged = false;
    CM_Guard s; s.Game = &game; s.ParentClass = &q; s.Me.real = false; s.Me.pos.x = 5.0f; s.Me.pos.y = s.Me.pos.z = 0;
    g_frame = 0; g_doneAt = 50; g_nearAt = 4; g_created = g_removed = 0; g_tag[0] = 0;
    s.Main(); if (g_created != 1 || std::strcmp(g_tag, "MCC4") || !q.FirstGuardTagged || g_removed != 1) bad++;
    g_frame = 0; s.Main(); if (g_created != 2 || std::strcmp(g_tag, "MCC5") || g_removed != 2) bad++;
    g_frame = 0; g_doneAt = 2; s.Main(); if (g_created != 2 || g_removed != 2) bad++;
    if (bad) { std::printf("00da63f0_TEST FAIL\n"); return 1; }
    std::printf("00da63f0_TEST PASS\n");
    return 0;
}