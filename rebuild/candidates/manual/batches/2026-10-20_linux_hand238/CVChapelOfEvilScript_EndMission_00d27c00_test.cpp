// Behaviour test for 0x00D27C00 EndMission: waits for the end flag (frames), then for the guild region, then makes
// the main guild map unusable and switches off the guide's quest cards; terminating threads stop early.
#include <cstdio>
#include <cstring>
struct CCharString { const char* Text; CCharString(const char* t) : Text(t) {} };
struct CScriptThing { const char* Name; };
static int g_frames, g_termAt, g_termCalls, g_loadedAfter, g_loadedCalls, g_usable, g_cards; static const char* g_thing;
struct CGameScriptInterface {
    bool* Flag; int FlagAfter;
    void NewScriptFrame() const { ++g_frames; if (g_frames >= FlagAfter) *Flag = true; }
    bool IsRegionLoaded(const CCharString& r) const { return std::strcmp(r.Text, "HeroGuildComplexInside") == 0 && ++g_loadedCalls > g_loadedAfter; }
    CScriptThing GetThingWithScriptName(const CCharString& n) const { CScriptThing t; t.Name = n.Text; return t; }
    void SetThingAsUsable(const CScriptThing& t, bool u) const { g_thing = t.Name; g_usable = u ? 1 : 0; }
    void SetHeroGuideToShowQuestCardsWhenSpokenTo(bool s) { g_cards = s ? 1 : 0; }
};
struct CScript {
    CGameScriptInterface* Game; bool MissionEnding;
    bool IsActiveThreadTerminating() { return ++g_termCalls >= g_termAt; }
    void EndMission()
    {
        while (!MissionEnding) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsRegionLoaded("HeroGuildComplexInside")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->SetThingAsUsable(Game->GetThingWithScriptName("MainGuildMap"), false);
        Game->SetHeroGuideToShowQuestCardsWhenSpokenTo(false);
    }
};
static bool run(int flagAfter, int loadedAfter, int termAt, bool wantDone, int wantFrames)
{
    g_frames = 0; g_termAt = termAt; g_termCalls = 0; g_loadedAfter = loadedAfter; g_loadedCalls = 0; g_usable = -1; g_cards = -1; g_thing = 0;
    CScript s; CGameScriptInterface g; g.Flag = &s.MissionEnding; g.FlagAfter = flagAfter; s.Game = &g; s.MissionEnding = flagAfter == 0;
    s.EndMission();
    if (g_frames != wantFrames) return false;
    if (wantDone) return g_usable == 0 && g_cards == 0 && g_thing && std::strcmp(g_thing, "MainGuildMap") == 0;
    return g_usable == -1 && g_cards == -1;
}
int main()
{
    bool ok = run(2, 3, 100, true, 5) && run(0, 0, 100, true, 0) && run(3, 0, 2, false, 2) && run(0, 5, 3, false, 2);
    if (!ok) { std::printf("00d27c00_TEST FAIL\n"); return 1; }
    std::printf("00d27c00_TEST PASS\n");
    return 0;
}
