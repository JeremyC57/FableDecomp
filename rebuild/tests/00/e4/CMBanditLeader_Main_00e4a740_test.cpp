// Behaviour test for CM_BanditLeader::Main @ 0x00E4A740 (copy of the model with a stub world).
#include <cstdio>
#include <cstring>
struct C3DVector { float x, y, z; };
static int g_created, g_attached, g_live;
static bool g_done, g_leaderDead;
struct CScriptThing {
    int kind;
    CScriptThing() : kind(0) { ++g_live; }
    CScriptThing(const CScriptThing& o) : kind(o.kind) { ++g_live; }
    ~CScriptThing() { --g_live; }
    const char* GetDataString() const { return "CREATURE_BANDIT_LEADER"; }
    const C3DVector& GetPos() const { static C3DVector p = { 1, 2, 3 }; return p; }
    bool IsDead() const { return kind == 4 ? g_leaderDead : true; }
};
struct CGameScriptInterface {
    CScriptThing GetThingWithScriptName(const char* name) const
    { CScriptThing t; if (!std::strcmp(name, "BanditTollVillage")) t.kind = 3; else if (!std::strcmp(name, "BanditLeader")) t.kind = 4; return t; }
    CScriptThing CreateCreature(const char* def, const C3DVector&, const char* name, bool f) const
    { CScriptThing t; if (!std::strcmp(def, "CREATURE_BANDIT_LEADER") && !std::strcmp(name, "BanditLeader") && !f) { t.kind = 2; ++g_created; } return t; }
    void EntityAttachToVillage(const CScriptThing& t, const CScriptThing& v) const { if (t.kind == 2 && v.kind == 3) ++g_attached; }
};
struct CM_BanditLeader {
    CGameScriptInterface* Game; CScriptThing Me;
    bool IsDone() { return g_done; }
    void Main()
    {
        if (Game->GetThingWithScriptName("BanditLeader").IsDead()) {
            if (IsDone()) return;
            CScriptThing leader = Game->CreateCreature(Me.GetDataString(), Me.GetPos(), "BanditLeader", false);
            Game->EntityAttachToVillage(leader, Game->GetThingWithScriptName("BanditTollVillage"));
        }
    }
};
int main()
{
    int bad = 0;
    CGameScriptInterface game;
    {
        CM_BanditLeader s; s.Game = &game; s.Me.kind = 1; int base = g_live;
        g_done = false; g_leaderDead = true; s.Main(); if (g_created != 1 || g_attached != 1 || g_live != base) bad++;
        g_leaderDead = false; s.Main(); if (g_created != 1 || g_live != base) bad++;
        g_leaderDead = true; g_done = true; s.Main(); if (g_created != 1 || g_live != base) bad++;
    }
    if (bad) { std::printf("00e4a740_TEST FAIL\n"); return 1; }
    std::printf("00e4a740_TEST PASS\n");
    return 0;
}