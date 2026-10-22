#include <cstdio>
#include <cstring>
// behaviour test: CQ_HangingTreeEvilScript::SpawnParty -- once the previous bandits have left the region, create
// <count> friendly bandit grunts near the spawn point (allied both ways with the hero, friendly faction, white health
// bar, green minimap orb), then start the exit, region and follow-status watchers
static int g_frames = 0, g_endAt = 1000, g_created = 0, g_allies = 0, g_factions = 0, g_bars = 0, g_markers = 0, g_spawned = 0;
struct C3DVector { float X, Y, Z; };
struct CScriptThing { int id; C3DVector Pos; const C3DVector& GetPos() const { return Pos; } };
struct CRGBColour { unsigned char R, G, B, A; CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : R(r), G(g), B(b), A(a) { } };
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; }
    CScriptThing GetHero() const { CScriptThing t; t.id = 1; return t; }
    CScriptThing CreateCreatureNearby(const char* def, const C3DVector& p, float r, const char* name) const
    { CScriptThing t; t.id = 100 + g_created++; t.Pos = p; if (std::strcmp(def, "CREATURE_BANDIT_GRUNT") || r != 3.0f || std::strcmp(name, "HTE_FreedomFighter") || p.X != 5.0f) t.id = 0; return t; }
    void EntitySetThingAsAllyOfThing(const CScriptThing& a, const CScriptThing& b) const { if (a.id && b.id) ++g_allies; }
    void EntitySetInFaction(const CScriptThing& t, const char* f) const { if (t.id && !std::strcmp(f, "FACTION_BANDITS_FRIENDLY")) ++g_factions; }
    void AddQuestInfoBarHealth(const CScriptThing& t, const CRGBColour& c, const char* icon) const
    { if (t.id && c.R == 255 && c.A == 255 && !std::strcmp(icon, "HUD_QUEST_ICON_BANDIT")) ++g_bars; }
    void MiniMapAddMarker(const CScriptThing& t, const char* m) const { if (t.id && !std::strcmp(m, "HUD_ORB_GREEN_SMALL")) ++g_markers; }
};
struct CQ_HangingTreeEvilScript {
    CGameScriptInterface* Game; bool BanditsLeftRegion;
    bool IsActiveThreadTerminating() { if (g_frames >= 2) BanditsLeftRegion = true; return g_frames >= g_endAt; }
    void SpawnFunction(const char*) { ++g_spawned; }
    void SpawnParty(long count, const CScriptThing& spawnPoint);
};
void CQ_HangingTreeEvilScript::SpawnParty(long count, const CScriptThing& spawnPoint)
{
    if (count > 0) {
        while (!BanditsLeftRegion) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
    }
    for (long i = 0; i < count; ++i) {
        if (IsActiveThreadTerminating()) return;
        CScriptThing bandit = Game->CreateCreatureNearby("CREATURE_BANDIT_GRUNT", spawnPoint.GetPos(), 3.0f, "HTE_FreedomFighter");
        Game->EntitySetThingAsAllyOfThing(bandit, Game->GetHero());
        Game->EntitySetThingAsAllyOfThing(Game->GetHero(), bandit);
        Game->EntitySetInFaction(bandit, "FACTION_BANDITS_FRIENDLY");
        Game->AddQuestInfoBarHealth(bandit, CRGBColour(255, 255, 255, 255), "HUD_QUEST_ICON_BANDIT");
        Game->MiniMapAddMarker(bandit, "HUD_ORB_GREEN_SMALL");
    }
    if (IsActiveThreadTerminating()) return;
    SpawnFunction("WatchForHeroNearExit");
    SpawnFunction("MonitorBanditRegion");
    SpawnFunction("CheckBanditFollowStatus");
}
int main()
{
    CGameScriptInterface game; CQ_HangingTreeEvilScript s; s.Game = &game; s.BanditsLeftRegion = false;
    CScriptThing point; point.id = 50; point.Pos.X = 5.0f; point.Pos.Y = 0; point.Pos.Z = 0;
    s.SpawnParty(3, point);
    bool ok = g_frames == 2 && g_created == 3 && g_allies == 6 && g_factions == 3 && g_bars == 3 && g_markers == 3 && g_spawned == 3;
    g_frames = 0; s.BanditsLeftRegion = false;
    s.SpawnParty(0, point);                             // nobody to spawn: no wait, watchers still start
    ok = ok && g_frames == 0 && g_created == 3 && g_spawned == 6;
    if (!ok) { std::printf("00d6c4a0_TEST FAIL\n"); return 1; }
    std::printf("00d6c4a0_TEST PASS\n");
    return 0;
}
