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
// behaviour test: CV_BordelloScript::WatchForDeedStatus waits for the Bordello, pulls the hidden deeds out of limbo
// once their location is found and records the pickup; it does nothing once the deeds were already found
class CGameScriptInterface { public:
    void NewScriptFrame() { ++g_frame; if (g_hook) g_hook(g_frame); }
    bool IsRegionLoaded(const CCharString& name) { return name.S == "Bordello" && g_frame >= 2; }
    CScriptThing GetThingWithScriptName(const CCharString& name) { Log("get " + name.S); return CScriptThing(name.S); }
    void EntitySetInLimbo(const CScriptThing& thing, bool limbo, bool b) { Log(std::string(limbo ? "limbo " : "unlimbo ") + thing.Name); (void)b; }
    CScriptThing GetHero() { return CScriptThing("HERO"); }
    bool IsObjectInThingsPossession(const CCharString& object, const CScriptThing& thing)
    { return object.S == "OBJECT_DEEDS_BORDELLO" && thing.Name == "HERO" && g_frame >= 6; }
};
struct CV_BordelloScript : CScriptBase {
    CGameScriptInterface* Game;
    bool HeroFoundDeeds;
    bool HeroFoundDeedsLocation;
    void WatchForDeedStatus();
};
static CV_BordelloScript* g_script;
static void FoundAt4(int frame) { if (frame == 4) g_script->HeroFoundDeedsLocation = true; }
void CV_BordelloScript::WatchForDeedStatus()
{
    while (!Game->IsRegionLoaded("Bordello")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (HeroFoundDeeds) {
        IsActiveThreadTerminating();
        return;
    }
    CScriptThing deeds = Game->GetThingWithScriptName("M_HiddenDeeds");
    Game->EntitySetInLimbo(deeds, true, true);
    while (!HeroFoundDeedsLocation) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetInLimbo(deeds, false, true);
    while (!Game->IsObjectInThingsPossession("OBJECT_DEEDS_BORDELLO", Game->GetHero())) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    HeroFoundDeeds = true;
}
int main()
{
    CGameScriptInterface game;
    CV_BordelloScript script;
    script.Game = &game;
    g_script = &script;
    Reset(1000); g_hook = FoundAt4; script.HeroFoundDeeds = false; script.HeroFoundDeedsLocation = false;
    script.WatchForDeedStatus();
    CHECK("00e3a150", g_log == "get M_HiddenDeeds@2;limbo M_HiddenDeeds@2;unlimbo M_HiddenDeeds@4;" && script.HeroFoundDeeds && g_frame == 6);
    Reset(1000); script.HeroFoundDeeds = true; script.HeroFoundDeedsLocation = false;
    script.WatchForDeedStatus();
    CHECK("00e3a150", g_log == "" && g_frame == 2);
    Reset(3); g_hook = FoundAt4; script.HeroFoundDeeds = false; script.HeroFoundDeedsLocation = false;
    script.WatchForDeedStatus();
    CHECK("00e3a150", g_log == "get M_HiddenDeeds@2;limbo M_HiddenDeeds@2;" && !script.HeroFoundDeeds && g_frame == 3);
    std::printf("00e3a150_TEST PASS\n");
    return 0;
}