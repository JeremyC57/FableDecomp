#include <cstdio>
#include <cstring>
// behaviour test: CQ_FireHeartScript::RunThemeOverriding -- while the demon door's lookout region is not loaded, each
// visit to LookoutPoint overrides the music with the haunted theme and leaving it restores the default theme
static int g_frames = 0, g_endAt = 1000, g_overrides = 0, g_restores = 0;
static const char g_lookout[] = "0011100011100000";      // LookoutPoint loaded per frame
static int g_doorAt = 11;
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; }
    bool IsLevelLoaded(const char* n) const
    {
        if (std::strcmp(n, "DemonDoor_LookoutPoint") == 0) return g_frames >= g_doorAt;
        return g_lookout[g_frames] == '1';
    }
    void OverrideMusic(long id, bool, bool) const { if (id == 0x11) ++g_overrides; }
    void TransitionToTheme(const char* t, float f) const { if (std::strcmp(t, "ENVIRONMENT_HAUNTED") || f != 0.5f) g_overrides = -99; }
    void StopOverrideMusic(bool) const { ++g_restores; }
    void ResetToDefaultTheme(float) const { }
};
struct CQ_FireHeartScript {
    CGameScriptInterface* Game;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void RunThemeOverriding();
};
void CQ_FireHeartScript::RunThemeOverriding()
{
    while (!Game->IsLevelLoaded("DemonDoor_LookoutPoint")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        while (!Game->IsLevelLoaded("LookoutPoint")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        Game->OverrideMusic(0x11, false, false);
        Game->TransitionToTheme("ENVIRONMENT_HAUNTED", 0.5f);
        while (Game->IsLevelLoaded("LookoutPoint")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        Game->StopOverrideMusic(false);
        Game->ResetToDefaultTheme(0.5f);
    }
}
int main()
{
    CGameScriptInterface game; CQ_FireHeartScript s; s.Game = &game;
    s.RunThemeOverriding();
    bool ok = g_overrides == 2 && g_restores == 2 && g_frames == 11;
    g_frames = 0; g_overrides = 0; g_restores = 0; g_endAt = 4;
    s.RunThemeOverriding();                             // ends while in LookoutPoint: overridden, not restored
    ok = ok && g_overrides == 1 && g_restores == 0;
    if (!ok) { std::printf("00d35ed0_TEST FAIL\n"); return 1; }
    std::printf("00d35ed0_TEST PASS\n");
    return 0;
}