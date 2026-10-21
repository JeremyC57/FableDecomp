#include <cstdio>
#include <cstring>
#include <string>
static std::string g_log;
static int g_frame = 0;
static int g_termAt = 1000;
static void (*g_hook)(int frame) = 0;
static void Log(const std::string& s) { char b[16]; std::sprintf(b, "@%d", g_frame); g_log += s; g_log += b; g_log += ";"; }
static void Reset(int termAt) { g_log = ""; g_frame = 0; g_termAt = termAt; g_hook = 0; }
struct CCharString {
    std::string S;
    CCharString(const char* text, long length = -1) : S(text) { (void)length; }
};
struct CScriptThing {
    std::string Name;
    CScriptThing() { }
    CScriptThing(const std::string& name) : Name(name) { }
};
struct CScriptBase {
    bool IsActiveThreadTerminating() { return g_frame >= g_termAt; }
};
static int Fail(const char* addr, const char* what) { std::printf("%s_TEST FAIL %s [%s]\n", addr, what, g_log.c_str()); return 1; }
#define CHECK(addr, cond) do { if (!(cond)) return Fail(addr, #cond); } while (0)
// behaviour test: CV_HauntedHouseScript::CheckRegionLeaving disables the house's creature generators each pass, makes
// the cellar door unusable while the house is loaded and drops the undead counter when the hero leaves
struct CGameScriptInterface {
    void NewScriptFrame() { ++g_frame; }
    bool IsRegionLoaded(const CCharString& name)            // loaded on frames 2-3 and 6-7
    { return name.S == "HauntedHouse" && ((g_frame >= 2 && g_frame <= 3) || (g_frame >= 6 && g_frame <= 7)); }
    void SetCreatureGeneratorsEnabled(const CCharString& region, bool enabled) { Log("gen " + region.S + (enabled ? " on" : " off")); }
    CScriptThing GetThingWithScriptName(const CCharString& name) { return CScriptThing(name.S); }
    void SetThingAsUsable(const CScriptThing& thing, bool usable) { Log(thing.Name + (usable ? " on" : " off")); }
    void RemoveQuestInfoElement(long id) { char b[16]; std::sprintf(b, "remove %ld", id); Log(b); }
};
struct CV_HauntedHouseScript : CScriptBase {
    CGameScriptInterface* Game;
    long GuiCounter;
    void CheckRegionLeaving();
};
void CV_HauntedHouseScript::CheckRegionLeaving()
{
    while (!IsActiveThreadTerminating()) {
        Game->SetCreatureGeneratorsEnabled("HauntedHouse", false);
        while (!Game->IsRegionLoaded("HauntedHouse")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CScriptThing door = Game->GetThingWithScriptName("HH_CellarDoor");
            Game->SetThingAsUsable(door, false);
            while (!(!Game->IsRegionLoaded("HauntedHouse"))) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->RemoveQuestInfoElement(GuiCounter);
        }
        Game->NewScriptFrame();
    }
}
int main()
{
    CGameScriptInterface game;
    CV_HauntedHouseScript script;
    script.Game = &game;
    script.GuiCounter = 7;
    Reset(9);
    script.CheckRegionLeaving();
    CHECK("00e953e0", g_log == "gen HauntedHouse off@0;HH_CellarDoor off@2;remove 7@4;gen HauntedHouse off@5;HH_CellarDoor off@6;remove 7@8;" && g_frame == 9);
    Reset(3);
    script.CheckRegionLeaving();
    CHECK("00e953e0", g_log == "gen HauntedHouse off@0;HH_CellarDoor off@2;" && g_frame == 3);
    std::printf("00e953e0_TEST PASS\n");
    return 0;
}
