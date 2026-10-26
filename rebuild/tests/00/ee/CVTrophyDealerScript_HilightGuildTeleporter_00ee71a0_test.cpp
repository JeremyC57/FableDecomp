#include <cstdio>
#include <cstring>
#include <string>
static std::string g_log;
static int g_frame = 0;
static int g_termAt = 1000;
static void (*g_hook)(int frame) = 0;
static void Log(const std::string& s) { char b[16]; std::sprintf(b, "@%d", g_frame); g_log += s; g_log += b; g_log += ";"; }
static void Reset(int termAt) { g_log = ""; g_frame = 0; g_termAt = termAt; g_hook = 0; }
class CCharString { public:
    std::string S;
    CCharString(const char* text, long length = -1) : S(text) { (void)length; }
};
class CScriptThing { public:
    std::string Name;
    CScriptThing() { }
    CScriptThing(const std::string& name) : Name(name) { }
};
class CScriptBase { public:
    bool IsActiveThreadTerminating() { return g_frame >= g_termAt; }
};
static int Fail(const char* addr, const char* what) { std::printf("%s_TEST FAIL %s [%s]\n", addr, what, g_log.c_str()); return 1; }
#define CHECK(addr, cond) do { if (!(cond)) return Fail(addr, #cond); } while (0)
// behaviour test: CV_TrophyDealerScript::HilightGuildTeleporter waits for the Guild interior, puts the quest orb on the
// teleporter marker, then removes it once Witchwood1 loads; termination stops it at either wait
class CGameScriptInterface { public:
    void NewScriptFrame() { ++g_frame; }
    bool IsRegionLoaded(const CCharString& name)
    { return (name.S == "HeroGuildComplexInside" && g_frame >= 2) || (name.S == "Witchwood1" && g_frame >= 5); }
    CScriptThing GetThingWithScriptName(const CCharString& name) { return CScriptThing(name.S); }
    void MiniMapAddMarker(const CScriptThing& thing, const CCharString& orb) { Log("add " + thing.Name + " " + orb.S); }
    void MiniMapRemoveMarker(const CScriptThing& thing) { Log("remove " + thing.Name); }
};
struct CV_TrophyDealerScript : CScriptBase {
    CGameScriptInterface* Game;
    void HilightGuildTeleporter();
};
void CV_TrophyDealerScript::HilightGuildTeleporter()
{
    while (!Game->IsRegionLoaded("HeroGuildComplexInside")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing teleporter = Game->GetThingWithScriptName("HERO_GUILD_TELEPORT_MARKER");
    Game->MiniMapAddMarker(teleporter, "HUD_ORB_QUEST_CORE");
    while (!Game->IsRegionLoaded("Witchwood1")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->MiniMapRemoveMarker(teleporter);
}
int main()
{
    CGameScriptInterface game;
    CV_TrophyDealerScript script;
    script.Game = &game;
    Reset(1000);
    script.HilightGuildTeleporter();
    CHECK("00ee71a0", g_log == "add HERO_GUILD_TELEPORT_MARKER HUD_ORB_QUEST_CORE@2;remove HERO_GUILD_TELEPORT_MARKER@5;");
    Reset(4);
    script.HilightGuildTeleporter();
    CHECK("00ee71a0", g_log == "add HERO_GUILD_TELEPORT_MARKER HUD_ORB_QUEST_CORE@2;" && g_frame == 4);
    Reset(1);
    script.HilightGuildTeleporter();
    CHECK("00ee71a0", g_log == "" && g_frame == 1);
    std::printf("00ee71a0_TEST PASS\n");
    return 0;
}