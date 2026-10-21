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
// behaviour test: CV_ArcheryCompetitionScript::DoTimesUp shows the times-up title, pauses, and has a living judge speak
// both game-over lines in order, waiting for each to finish; a null thing or an idle judge does nothing
enum ETextGroupSelectionMethod { GROUP_SELECT_RANDOM = 1 };
static int g_taskEnds = 0;
static float g_health = 1.0f;
struct CScriptThingN : CScriptThing {
    bool Null;
    CScriptThingN(bool null) : Null(null) { }
    bool IsNull() const { return Null; }
};
struct CScriptGameResourceObjectScriptedThingBase {
    bool Scripting;
    bool IsScripting() const { return Scripting; }
    void ClearCommands() { Log("clear"); }
    CScriptThing GetScriptThing() { return CScriptThing("JUDGE"); }
    void Speak(const CScriptThing& to, const CCharString& text, ETextGroupSelectionMethod m, bool a, bool b, bool c)
    { Log("speak " + to.Name + " " + text.S); g_taskEnds = g_frame + 2; (void)m; (void)a; (void)b; (void)c; }
    bool IsPerformingScriptTask() { return g_frame < g_taskEnds; }
};
struct CGameScriptInterface {
    void NewScriptFrame() { ++g_frame; }
    void AddScreenTitleMessage(const CCharString& text, float t, bool b) { Log("title " + text.S); (void)t; (void)b; }
    void Pause(float t) { g_frame += (int)t; }
    float GetHealth(const CScriptThing& thing) { return thing.Name == "JUDGE" ? g_health : 0.0f; }
    CScriptThing GetHero() { return CScriptThing("HERO"); }
};
struct CV_ArcheryCompetitionScript : CScriptBase {
    CGameScriptInterface* Game;
    void DoTimesUp(const CScriptThingN& thing, CScriptGameResourceObjectScriptedThingBase& judge);
};
void CV_ArcheryCompetitionScript::DoTimesUp(const CScriptThingN& thing, CScriptGameResourceObjectScriptedThingBase& judge)
{
    if (thing.IsNull()) {
        IsActiveThreadTerminating();
        return;
    }
    if (!judge.IsScripting())
        return;
    if (IsActiveThreadTerminating())
        return;
    judge.ClearCommands();
    Game->AddScreenTitleMessage("TEXT_QST_004_TIMES_UP", 5.0f, false);
    Game->Pause(5.0f);
    if (Game->GetHealth(judge.GetScriptThing()) > 0.0f) {
        judge.Speak(Game->GetHero(), "TEXT_QST_004_GAME_OVER1", GROUP_SELECT_RANDOM, false, true, false);
        while (judge.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
    if (Game->GetHealth(judge.GetScriptThing()) > 0.0f) {
        judge.Speak(Game->GetHero(), "TEXT_QST_004_GAME_OVER2", GROUP_SELECT_RANDOM, false, true, false);
        while (judge.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
}
int main()
{
    CGameScriptInterface game;
    CV_ArcheryCompetitionScript script;
    script.Game = &game;
    CScriptGameResourceObjectScriptedThingBase judge;
    CScriptThingN live(false), null(true);
    Reset(1000); judge.Scripting = true; g_health = 1.0f;
    script.DoTimesUp(live, judge);
    CHECK("00e36590", g_log == "clear@0;title TEXT_QST_004_TIMES_UP@0;speak HERO TEXT_QST_004_GAME_OVER1@5;speak HERO TEXT_QST_004_GAME_OVER2@7;" && g_frame == 9);
    Reset(1000); g_health = 0.0f;
    script.DoTimesUp(live, judge);
    CHECK("00e36590", g_log == "clear@0;title TEXT_QST_004_TIMES_UP@0;" && g_frame == 5);
    Reset(6); g_health = 1.0f;
    script.DoTimesUp(live, judge);
    CHECK("00e36590", g_log == "clear@0;title TEXT_QST_004_TIMES_UP@0;speak HERO TEXT_QST_004_GAME_OVER1@5;" && g_frame == 6);
    Reset(1000);
    script.DoTimesUp(null, judge);
    judge.Scripting = false;
    script.DoTimesUp(live, judge);
    CHECK("00e36590", g_log == "" && g_frame == 0);
    std::printf("00e36590_TEST PASS\n");
    return 0;
}