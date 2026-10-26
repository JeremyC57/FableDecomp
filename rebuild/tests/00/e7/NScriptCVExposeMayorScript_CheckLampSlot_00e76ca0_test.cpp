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
// behaviour test: CV_ExposeMayorScript::CheckLampSlot, while the Haunted House is loaded and the lights are not yet
// flashed, puts the hand lamp in the quick slot when the hero comes within range of the flash point and clears it when
// he leaves; on exit the slot is cleared
static float g_dist[16];
struct CGameScriptInterface {
    void NewScriptFrame() { ++g_frame; }
    CScriptThing GetThingWithScriptName(const CCharString& name) { return CScriptThing(name.S); }
    CScriptThing GetHero() { return CScriptThing("HERO"); }
    bool IsLevelLoaded(const CCharString& name) { return name.S == "HauntedHouse" && g_frame < 8; }
    void SetPreferredQuickAccessItem(const CCharString& item, long slot, long expiry)
    { char t[96]; std::sprintf(t, "slot %s %ld %ld", item.S.c_str(), slot, expiry); Log(t); }
};
static bool IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float d)
{ return a.Name == "HERO" && b.Name == "EM_FlashLightPoint" && g_dist[g_frame] > d; }
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float d)
{ return a.Name == "HERO" && b.Name == "EM_FlashLightPoint" && g_dist[g_frame] < d; }
struct CV_ExposeMayorScript : CScriptBase {
    CGameScriptInterface* Game;
    bool HeroFlashedLightsCorrectly;
    float MaxFlashDistFromStables;
    void CheckLampSlot();
};
void CV_ExposeMayorScript::CheckLampSlot()
{
    CScriptThing point = Game->GetThingWithScriptName("EM_FlashLightPoint");
    bool lampSet = false;
    for (;;) {
        bool inHouse = !HeroFlashedLightsCorrectly && Game->IsLevelLoaded("HauntedHouse");
        if (!inHouse)
            break;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (lampSet) {
            if (IsActiveThreadTerminating())
                return;
            if (IsDistanceBetweenThingsOver(Game->GetHero(), point, MaxFlashDistFromStables)) {
                if (IsActiveThreadTerminating())
                    return;
                Game->SetPreferredQuickAccessItem("OBJECT_HAND_LAMP", 2, -1);
                lampSet = false;
            }
        } else {
            if (IsActiveThreadTerminating())
                return;
            if (IsDistanceBetweenThingsUnder(Game->GetHero(), point, MaxFlashDistFromStables)) {
                if (IsActiveThreadTerminating())
                    return;
                Game->SetPreferredQuickAccessItem("OBJECT_HAND_LAMP", 2, 1000000);
                lampSet = true;
            }
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetPreferredQuickAccessItem("OBJECT_HAND_LAMP", 2, -1);
}
int main()
{
    CGameScriptInterface game;
    CV_ExposeMayorScript script;
    script.Game = &game;
    script.MaxFlashDistFromStables = 10.0f;
    float d[16] = { 50, 50, 5, 5, 8, 20, 3, 50, 50, 50, 50, 50, 50, 50, 50, 50 };
    for (int i = 0; i < 16; ++i) g_dist[i] = d[i];
    Reset(1000); script.HeroFlashedLightsCorrectly = false;
    script.CheckLampSlot();
    CHECK("00e76ca0", g_log == "slot OBJECT_HAND_LAMP 2 1000000@2;slot OBJECT_HAND_LAMP 2 -1@5;slot OBJECT_HAND_LAMP 2 1000000@6;slot OBJECT_HAND_LAMP 2 -1@7;slot OBJECT_HAND_LAMP 2 -1@8;");
    Reset(1000); script.HeroFlashedLightsCorrectly = true;
    script.CheckLampSlot();
    CHECK("00e76ca0", g_log == "slot OBJECT_HAND_LAMP 2 -1@0;" && g_frame == 0);
    Reset(4); script.HeroFlashedLightsCorrectly = false;
    script.CheckLampSlot();
    CHECK("00e76ca0", g_log == "slot OBJECT_HAND_LAMP 2 1000000@2;" && g_frame == 4);
    std::printf("00e76ca0_TEST PASS\n");
    return 0;
}