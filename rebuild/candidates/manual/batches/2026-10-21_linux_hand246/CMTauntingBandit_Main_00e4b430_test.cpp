// Behaviour test for CM_TauntingBandit::Main @ 0x00E4B430 (copy of the model with a stub world).
#include <cstdio>
#include <cstring>
struct C3DVector { float x, y, z; };
static int g_existing, g_created, g_attached, g_live;
static bool g_done;
struct CScriptThing {
    int kind;                                        // 1 marker, 2 spawned bandit, 3 village
    CScriptThing() : kind(0) { ++g_live; }
    CScriptThing(const CScriptThing& o) : kind(o.kind) { ++g_live; }
    ~CScriptThing() { --g_live; }
    const char* GetDataString() const { return "CREATURE_BANDIT"; }
    const C3DVector& GetPos() const { static C3DVector p = { 1, 2, 3 }; return p; }
};
struct ThingVector {                                 // minimal owning vector of script things
    CScriptThing* items; unsigned int count;
    ThingVector() : items(0), count(0) { }
    ~ThingVector() { delete[] items; }
    unsigned int size() const { return count; }
    void resize(unsigned int n) { delete[] items; items = n ? new CScriptThing[n] : 0; count = n; }
};
struct CGameScriptInterface {
    long GetAllThingsWithScriptName(const char* name, ThingVector& v) const
    { if (std::strcmp(name, "TauntingBandit")) return 0; v.resize(g_existing); return (long)v.size(); }
    CScriptThing CreateCreature(const char* def, const C3DVector&, const char* name, bool f) const
    { CScriptThing t; if (!std::strcmp(def, "CREATURE_BANDIT") && !std::strcmp(name, "TauntingBandit") && !f) { t.kind = 2; ++g_created; } return t; }
    CScriptThing GetThingWithScriptName(const char* name) const { CScriptThing t; if (!std::strcmp(name, "BanditTollVillage")) t.kind = 3; return t; }
    void EntityAttachToVillage(const CScriptThing& t, const CScriptThing& v) const { if (t.kind == 2 && v.kind == 3) ++g_attached; }
};
struct CScriptDef { unsigned long MaxTauntingBandits; };
static CScriptDef g_def;
struct CM_TauntingBandit {
    CGameScriptInterface* Game; CScriptThing Me;
    bool IsDone() { return g_done; }
    void Main()
    {
        ThingVector bandits;
        Game->GetAllThingsWithScriptName("TauntingBandit", bandits);
        if (bandits.size() < g_def.MaxTauntingBandits) {
            if (IsDone()) return;
            CScriptThing bandit = Game->CreateCreature(Me.GetDataString(), Me.GetPos(), "TauntingBandit", false);
            Game->EntityAttachToVillage(bandit, Game->GetThingWithScriptName("BanditTollVillage"));
        }
    }
};
int main()
{
    int bad = 0;
    CGameScriptInterface game;
    {
        CM_TauntingBandit s; s.Game = &game; s.Me.kind = 1; g_def.MaxTauntingBandits = 3;
        int base = g_live;
        g_done = false; g_existing = 1; s.Main(); if (g_created != 1 || g_attached != 1 || g_live != base) bad++;
        g_existing = 3; s.Main(); if (g_created != 1 || g_live != base) bad++;
        g_existing = 0; g_done = true; s.Main(); if (g_created != 1 || g_live != base) bad++;
    }
    if (bad) { std::printf("00e4b430_TEST FAIL\n"); return 1; }
    std::printf("00e4b430_TEST PASS\n");
    return 0;
}
