#include <cstdio>
#include <cstring>
// behaviour test: CQ_GuildTrainingScript::KeepTabsOnWhisper -- wait for the woods quest + guild complex + live apprentice,
// remove him, wait for the woods quest to end, then (unless the departure quest is active) recreate the apprentice
static int g_frames = 0, g_endAt = 1000, g_removed = 0, g_created = 0, g_willEnds = 8;
static bool g_departure = false;
struct C3DVector { float X, Y, Z; };
struct CScriptThing { const char* name; C3DVector pos; bool IsAlive() const { return std::strcmp(name, "MeleeApprentice") == 0; } const C3DVector& GetPos() const { return pos; } };
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; }
    bool IsLevelLoaded(const char* l) const { return std::strcmp(l, "GuildWoods") == 0 ? g_frames >= 2 : g_frames >= 4; }
    bool IsQuestActive(const char* q) const { return std::strcmp(q, "Q_GuildTrainingDeparture") == 0 ? g_departure : g_frames < g_willEnds; }
    CScriptThing GetThingWithScriptName(const char* n) const { CScriptThing t; t.name = n; t.pos.X = 3.0f; t.pos.Y = t.pos.Z = 0; return t; }
    CScriptThing CreateCreature(const char* def, const C3DVector& pos, const char* name, bool) const
    { if (!std::strcmp(def, "CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE") && pos.X == 3.0f && !std::strcmp(name, "MeleeApprentice")) ++g_created; return GetThingWithScriptName(name); }
    void RemoveThing(const CScriptThing& t, bool a, bool b) const { if (!std::strcmp(t.name, "MeleeApprentice") && !a && b) ++g_removed; }
};
struct CQ_GuildTrainingScript {
    CGameScriptInterface* Game;
    bool IsActiveThreadTerminating() { return g_frames >= g_endAt; }
    void KeepTabsOnWhisper();
};
void CQ_GuildTrainingScript::KeepTabsOnWhisper()
{
    for (;;) {
        if (IsActiveThreadTerminating()) return;
        if (Game->IsLevelLoaded("GuildWoods") && Game->IsQuestActive("Q_GuildTrainingWoodsWill")) {
            if (IsActiveThreadTerminating()) return;
            while (!Game->IsLevelLoaded("HeroGuildComplex")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            if (Game->IsQuestActive("Q_GuildTrainingWoodsWill")) {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetThingWithScriptName("MeleeApprentice").IsAlive()) break;
            }
        }
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating()) return;
    Game->RemoveThing(Game->GetThingWithScriptName("MeleeApprentice"), false, true);
    while (Game->IsQuestActive("Q_GuildTrainingWoodsWill") != false) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    if (!Game->IsQuestActive("Q_GuildTrainingDeparture")) {
        if (IsActiveThreadTerminating()) return;
        Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;
        Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;
        Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;
        while (!Game->IsLevelLoaded("HeroGuildComplex")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Game->CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", Game->GetThingWithScriptName("M_MeleeOpponentStand").GetPos(), "MeleeApprentice", false);
    }
}
int main()
{
    CGameScriptInterface game; CQ_GuildTrainingScript s; s.Game = &game;
    s.KeepTabsOnWhisper();
    bool ok = g_removed == 1 && g_created == 1 && g_frames == 11;
    g_frames = 0; g_departure = true;
    s.KeepTabsOnWhisper();                              // departure quest active: no recreation
    ok = ok && g_removed == 2 && g_created == 1;
    if (!ok) { std::printf("00d3cbd0_TEST FAIL\n"); return 1; }
    std::printf("00d3cbd0_TEST PASS\n");
    return 0;
}
