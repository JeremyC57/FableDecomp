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
// behaviour test: CQ_HobbeCaveScript::WatchForHavingKey makes the hexagon door usable exactly when the hero carries
// the key, once per load of the main chamber, and waits for the chamber to unload before looping
static bool g_hasKey = false;
struct CGameScriptInterface {
    void NewScriptFrame() { ++g_frame; }
    bool IsRegionLoaded(const CCharString& name)            // loaded on frames 2-3 and 6-7
    { return name.S == "HobbeCaveMainChamber" && ((g_frame >= 2 && g_frame <= 3) || (g_frame >= 6 && g_frame <= 7)); }
    CScriptThing GetThingWithScriptName(const CCharString& name) { return CScriptThing(name.S); }
    CScriptThing GetHero() { return CScriptThing("HERO"); }
    bool IsObjectInThingsPossession(const CCharString& object, const CScriptThing& thing)
    { return object.S == "OBJECT_HEXAGON_DOOR_KEY" && thing.Name == "HERO" && g_hasKey; }
    void SetThingAsUsable(const CScriptThing& thing, bool usable) { Log(thing.Name + (usable ? " on" : " off")); }
};
struct CQ_HobbeCaveScript : CScriptBase {
    CGameScriptInterface* Game;
    void WatchForHavingKey();
};
void CQ_HobbeCaveScript::WatchForHavingKey()
{
    while (!IsActiveThreadTerminating()) {
        while (!Game->IsRegionLoaded("HobbeCaveMainChamber")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CScriptThing door = Game->GetThingWithScriptName("HexagonDoor");
            if (Game->IsObjectInThingsPossession("OBJECT_HEXAGON_DOOR_KEY", Game->GetHero())) {
                if (IsActiveThreadTerminating())
                    return;
                Game->SetThingAsUsable(door, true);
            } else {
                if (IsActiveThreadTerminating())
                    return;
                Game->SetThingAsUsable(door, false);
            }
            while (!(!Game->IsRegionLoaded("HobbeCaveMainChamber"))) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        Game->NewScriptFrame();
    }
}
int main()
{
    CGameScriptInterface game;
    CQ_HobbeCaveScript script;
    script.Game = &game;
    Reset(9); g_hasKey = false;
    script.WatchForHavingKey();
    CHECK("00d8ee50", g_log == "HexagonDoor off@2;HexagonDoor off@6;" && g_frame == 9);
    Reset(5); g_hasKey = true;
    script.WatchForHavingKey();
    CHECK("00d8ee50", g_log == "HexagonDoor on@2;" && g_frame == 5);
    std::printf("00d8ee50_TEST PASS\n");
    return 0;
}