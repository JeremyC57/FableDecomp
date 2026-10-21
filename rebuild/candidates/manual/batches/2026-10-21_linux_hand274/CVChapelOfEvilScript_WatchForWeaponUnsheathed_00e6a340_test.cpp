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
// behaviour test: CV_ChapelOfEvilScript::WatchForWeaponUnsheathed tracks the hero's weapon inside the Chapel; the first draw after the
// intro has a living sound altar warn him once (TEXT_QST_019_WEAPON_01), sheathing clears the flag, and it stops on termination
static bool g_wielding[16];
static bool g_altarAlive = true;
struct CScriptThingA : CScriptThing {
    CScriptThingA(const std::string& n) : CScriptThing(n) { }
    bool IsAlive() const { return g_altarAlive; }
};
struct CGameScriptInterface {
    void NewScriptFrame() { ++g_frame; }
    CScriptThingA GetThingWithScriptName(const CCharString& name) { return CScriptThingA(name.S); }
    CScriptThing GetHero() { return CScriptThing("HERO"); }
    bool IsPlayerWieldingWeapon() { return g_wielding[g_frame]; }
    long AddNewConversation(const CScriptThing& thing, bool a, bool b) { Log("conv " + thing.Name); (void)a; (void)b; return 3; }
    void AddPersonToConversation(long c, const CScriptThing& person) { Log("person " + person.Name); (void)c; }
    void AddLineToConversation(long c, const CCharString& text, bool a, const CScriptThing& speaker, const CScriptThing& listener)
    { Log("line " + text.S + " " + speaker.Name + ">" + listener.Name); (void)c; (void)a; }
};
struct CV_ChapelOfEvilScript : CScriptBase {
    CGameScriptInterface* Game;
    bool WeaponWarningComplete;
    bool WeaponUnsheathed;
    bool ChapelIntroComplete;
    bool InsideChapel;
    void WatchForWeaponUnsheathed();
};
void CV_ChapelOfEvilScript::WatchForWeaponUnsheathed()
{
    CScriptThingA altar = Game->GetThingWithScriptName("M_SoundAltar");
    if (!IsActiveThreadTerminating()) {
        do {
            if (InsideChapel && Game->IsPlayerWieldingWeapon() && !WeaponUnsheathed) {
                if (IsActiveThreadTerminating())
                    return;
                WeaponUnsheathed = true;
                if (!WeaponWarningComplete && ChapelIntroComplete) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (altar.IsAlive()) {
                        if (IsActiveThreadTerminating())
                            return;
                        long conversation = Game->AddNewConversation(altar, false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, "TEXT_QST_019_WEAPON_01", false, altar, Game->GetHero());
                    }
                    WeaponWarningComplete = true;
                }
            } else if (!Game->IsPlayerWieldingWeapon() && WeaponUnsheathed) {
                if (IsActiveThreadTerminating())
                    return;
                WeaponUnsheathed = false;
            }
            Game->NewScriptFrame();
        } while (!IsActiveThreadTerminating());
    }
}
int main()
{
    CGameScriptInterface game;
    CV_ChapelOfEvilScript script;
    script.Game = &game;
    bool w[16] = { false, true, true, false, true, false, false, false, false, false, false, false, false, false, false, false };
    for (int i = 0; i < 16; ++i) g_wielding[i] = w[i];
    Reset(8); script.WeaponWarningComplete = false; script.WeaponUnsheathed = false; script.ChapelIntroComplete = true; script.InsideChapel = true; g_altarAlive = true;
    script.WatchForWeaponUnsheathed();
    CHECK("00e6a340", g_log == "conv M_SoundAltar@1;person HERO@1;line TEXT_QST_019_WEAPON_01 M_SoundAltar>HERO@1;" && script.WeaponWarningComplete && !script.WeaponUnsheathed && g_frame == 8);
    Reset(3); script.WeaponWarningComplete = false; script.WeaponUnsheathed = false; script.ChapelIntroComplete = true; script.InsideChapel = true; g_altarAlive = false;
    script.WatchForWeaponUnsheathed();
    CHECK("00e6a340", g_log == "" && script.WeaponWarningComplete && script.WeaponUnsheathed);
    Reset(8); script.WeaponWarningComplete = false; script.WeaponUnsheathed = false; script.ChapelIntroComplete = false; script.InsideChapel = true; g_altarAlive = true;
    script.WatchForWeaponUnsheathed();
    CHECK("00e6a340", g_log == "" && !script.WeaponWarningComplete);
    Reset(8); script.WeaponWarningComplete = false; script.WeaponUnsheathed = false; script.ChapelIntroComplete = true; script.InsideChapel = false;
    script.WatchForWeaponUnsheathed();
    CHECK("00e6a340", g_log == "" && !script.WeaponUnsheathed && !script.WeaponWarningComplete);
    std::printf("00e6a340_TEST PASS\n");
    return 0;
}
