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
// behaviour test: CV_KnotholeGladeGatesScript::DoPanicBanter waits for the hero to reach the gate, then until the hero
// is alerted has the gate guard complain to him whenever rand() % 300 hits zero
#include <stdlib.h>
static int g_nearAt = 3;
static int g_rolls[16];
static int g_roll = 0;
static int FakeRand() { return g_rolls[g_roll++ & 15]; }
static void AlertHero();
struct CGameScriptInterface {
    void NewScriptFrame() { ++g_frame; if (g_frame == 8) AlertHero(); }
    CScriptThing GetThingWithScriptName(const CCharString& name) { return CScriptThing(name.S); }
    CScriptThing GetHero() { return CScriptThing("HERO"); }
    long AddNewConversation(const CScriptThing& thing, bool a, bool b) { Log("conv " + thing.Name); (void)a; (void)b; return 42; }
    void AddPersonToConversation(long c, const CScriptThing& person) { Log("person " + person.Name); (void)c; }
    void AddLineToConversation(long c, const CCharString& text, bool a, const CScriptThing& speaker, const CScriptThing& listener)
    { Log("line " + text.S + " " + speaker.Name + ">" + listener.Name); (void)c; (void)a; }
};
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance)
{ return a.Name == "HERO" && b.Name == "GATE" && distance == 30.0f && g_frame >= g_nearAt; }
struct CV_KnotholeGladeGatesScript : CScriptBase {
    CGameScriptInterface* Game;
    bool HeroAlertedToProblem;
    CScriptThing TheGate;
    void DoPanicBanter();
};
static CV_KnotholeGladeGatesScript* g_script;
static void AlertHero() { g_script->HeroAlertedToProblem = true; }
void CV_KnotholeGladeGatesScript::DoPanicBanter()
{
    CScriptThing guard = Game->GetThingWithScriptName("KG_GateGuard");
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), TheGate, 30.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!HeroAlertedToProblem) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (FakeRand() % 300 == 0) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(guard, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_050_GUARD_STOP_COMPLAINING", false, guard, Game->GetHero());
        }
    }
    if (IsActiveThreadTerminating())
        return;
}
int main()
{
    CGameScriptInterface game;
    CV_KnotholeGladeGatesScript script;
    script.Game = &game;
    script.TheGate = CScriptThing("GATE");
    g_script = &script;
    int rolls[16] = { 1, 600, 299, 300, 5, 0, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    for (int i = 0; i < 16; ++i) g_rolls[i] = rolls[i];
    Reset(1000); g_roll = 0; script.HeroAlertedToProblem = false;
    script.DoPanicBanter();
    const char* one = "conv KG_GateGuard@%d;person HERO@%d;line TEXT_QST_050_GUARD_STOP_COMPLAINING KG_GateGuard>HERO@%d;";
    char expect[512], a[200], b[200];
    std::sprintf(a, one, 5, 5, 5);
    std::sprintf(b, one, 7, 7, 7);
    std::sprintf(expect, "%s%s", a, b);
    CHECK("00ea1ec0", g_log == expect && g_frame == 8 && g_roll == 5);
    Reset(2); g_roll = 0; script.HeroAlertedToProblem = false;
    script.DoPanicBanter();
    CHECK("00ea1ec0", g_log == "" && g_frame == 2 && g_roll == 0);
    std::printf("00ea1ec0_TEST PASS\n");
    return 0;
}