#include <cstdio>
#include <cstring>
// behaviour test: CV_ChickenKickingScript::LookAfterOrganiser -- each time the hero leaves Oak Bay and comes back,
// re-create the chicken master if he ran off: at his usual marker once the ghost is known to be gone, else at the
// scared-master marker
static int g_frames = 0, g_endAt = 1000, g_created = 0; static const char* g_where = 0;
static const char g_loaded[] = "1100111001110000";
struct C3DVector { const char* Marker; };
struct CScriptThing { const char* name; C3DVector Pos; const C3DVector& GetPos() const { return Pos; } };
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; }
    bool IsRegionLoaded(const char* n) const { return !std::strcmp(n, "OakBay") && g_loaded[g_frames] == '1'; }
    CScriptThing GetThingWithScriptName(const char* n) const { CScriptThing t; t.name = n; t.Pos.Marker = n; return t; }
    void CreateCreature(const char* def, const C3DVector& at, const char* name) const
    { if (!std::strcmp(def, "CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED") && !std::strcmp(name, "ChickenMaster")) { ++g_created; g_where = at.Marker; } }
};
struct CV_ChickenKickingScript {
    CGameScriptInterface* Game; bool RanOff, KnowGhostHasGone;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void LookAfterOrganiser();
};
void CV_ChickenKickingScript::LookAfterOrganiser()
{
    if (IsActiveThreadTerminating()) return;
    do {
        while (Game->IsRegionLoaded("OakBay")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        while (!Game->IsRegionLoaded("OakBay")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (RanOff) {
            if (KnowGhostHasGone)
                Game->CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", Game->GetThingWithScriptName("MK_CK_ORG").GetPos(), "ChickenMaster");
            else
                Game->CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", Game->GetThingWithScriptName("ScaredMasterMarker").GetPos(), "ChickenMaster");
            RanOff = false;
        }
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
int main()
{
    CGameScriptInterface game; CV_ChickenKickingScript s; s.Game = &game; s.RanOff = true; s.KnowGhostHasGone = false; g_endAt = 6;
    s.LookAfterOrganiser();                             // back at frame 4: re-created once at the scared marker
    bool ok = g_created == 1 && !std::strcmp(g_where, "ScaredMasterMarker") && !s.RanOff;
    g_frames = 0; g_endAt = 12; s.RanOff = true; s.KnowGhostHasGone = true;
    s.LookAfterOrganiser();                             // two returns (frames 4 and 9), he only ran off before the first
    ok = ok && g_created == 2 && !std::strcmp(g_where, "MK_CK_ORG");
    if (!ok) { std::printf("00e631f0_TEST FAIL\n"); return 1; }
    std::printf("00e631f0_TEST PASS\n");
    return 0;
}
