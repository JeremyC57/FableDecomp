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
// behaviour test: CV_FisticuffsClubScript::FisticuffsChallenge sets the both-alive predicate, makes the fighters enemies,
// waits until either drops below the club's minimum health (or the hero paid the entry fee), then makes them allies again
struct CScriptDef { float FC_MinimumFighterHealth; };
static CScriptDef g_def;
static CScriptDef* g_pScriptDef = &g_def;
static float g_h1 = 100.0f, g_h2 = 100.0f;
struct CGameScriptInterface {
    void NewScriptFrame() { ++g_frame; g_h1 -= 10.0f; g_h2 -= 15.0f; }
    void GiveThingBestEnemyTarget(const CScriptThing& a, const CScriptThing& b) { Log("enemy " + a.Name + ">" + b.Name); }
    void ClearThingBestEnemyTarget(const CScriptThing& a) { Log("clear " + a.Name); }
    void EntityUnsetThingAsAllyOfThing(const CScriptThing& a, const CScriptThing& b) { Log("unally " + a.Name + ">" + b.Name); }
    void EntitySetThingAsAllyOfThing(const CScriptThing& a, const CScriptThing& b) { Log("ally " + a.Name + ">" + b.Name); }
    float GetHealth(const CScriptThing& thing) { return thing.Name == "F1" ? g_h1 : g_h2; }
};
struct CIsThingAlive { std::string Name; CIsThingAlive(const CScriptThing& t) : Name(t.Name) { } };
struct CAnd { std::string Text; };
static CAnd operator&&(const CIsThingAlive& a, const CIsThingAlive& b) { CAnd r; r.Text = a.Name + "&" + b.Name; return r; }
struct CV_FisticuffsClubScript : CScriptBase {
    CGameScriptInterface* Game;
    CScriptThing SFighterOne, SFighterTwo;
    bool FightFinished;
    bool HeroPaidEntryFee;
    void SetPredicate(const CAnd& predicate) { Log("predicate " + predicate.Text); }
    void FisticuffsChallenge();
};
void CV_FisticuffsClubScript::FisticuffsChallenge()
{
    SetPredicate(CIsThingAlive(SFighterOne) && CIsThingAlive(SFighterTwo));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(SFighterOne, SFighterTwo);
    Game->GiveThingBestEnemyTarget(SFighterTwo, SFighterOne);
    Game->EntityUnsetThingAsAllyOfThing(SFighterOne, SFighterTwo);
    Game->EntityUnsetThingAsAllyOfThing(SFighterTwo, SFighterOne);
    while (!FightFinished) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        CScriptDef* def = g_pScriptDef;
        if (Game->GetHealth(SFighterOne) < def->FC_MinimumFighterHealth) {
            if (IsActiveThreadTerminating())
                return;
            FightFinished = true;
        }
        def = g_pScriptDef;
        if (Game->GetHealth(SFighterTwo) < def->FC_MinimumFighterHealth) {
            if (IsActiveThreadTerminating())
                return;
            FightFinished = true;
        }
        if (HeroPaidEntryFee) {
            if (IsActiveThreadTerminating())
                return;
            FightFinished = true;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->ClearThingBestEnemyTarget(SFighterOne);
    Game->ClearThingBestEnemyTarget(SFighterTwo);
    Game->EntitySetThingAsAllyOfThing(SFighterOne, SFighterTwo);
    Game->EntitySetThingAsAllyOfThing(SFighterTwo, SFighterOne);
}
int main()
{
    CGameScriptInterface game;
    CV_FisticuffsClubScript script;
    script.Game = &game;
    script.SFighterOne = CScriptThing("F1");
    script.SFighterTwo = CScriptThing("F2");
    g_def.FC_MinimumFighterHealth = 30.0f;
    const char* setup = "predicate F1&F2@0;enemy F1>F2@1;enemy F2>F1@1;unally F1>F2@1;unally F2>F1@1;";
    Reset(1000); g_h1 = g_h2 = 100.0f; script.FightFinished = false; script.HeroPaidEntryFee = false;
    script.FisticuffsChallenge();
    CHECK("00e86e60", g_log == std::string(setup) + "clear F1@5;clear F2@5;ally F1>F2@5;ally F2>F1@5;" && script.FightFinished);
    Reset(1000); g_h1 = g_h2 = 100.0f; script.FightFinished = false; script.HeroPaidEntryFee = true;
    script.FisticuffsChallenge();
    CHECK("00e86e60", g_frame == 2 && script.FightFinished);
    Reset(3); g_h1 = g_h2 = 100.0f; script.FightFinished = false; script.HeroPaidEntryFee = false;
    script.FisticuffsChallenge();
    CHECK("00e86e60", g_log == setup && !script.FightFinished);
    std::printf("00e86e60_TEST PASS\n");
    return 0;
}