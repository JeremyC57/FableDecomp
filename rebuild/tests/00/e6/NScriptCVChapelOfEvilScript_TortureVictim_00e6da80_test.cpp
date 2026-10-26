// Behaviour test for CV_ChapelOfEvilScript::TortureVictim (0x00E6DA80).
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CScriptThing { const char* def; float health; int sex; const char* GetDefName() const { return def; } };
struct CHandle { const char* name; };
struct CGameScriptInterface {
    int time; int day;
    void NewScriptFrame() { Log("frame"); }
    float GetHealth(const CScriptThing& t) { return t.health; }
    int EntityGetSex(const CScriptThing& t) { return t.sex; }
    int GetTimeOfDay() { return time; }
    int GetDayOfWeek() { return day; }
};
struct CMaster { bool flag; };
struct CV_ChapelOfEvilScript {
    CGameScriptInterface* Game;
    CMaster* PMasterData;
    int termAfter;
    int VictimHealth; int VictimGender; bool VictimNightTime, VictimSunday, VictimIsGuard, VictimIsBandit, VictimIsTrader;
    const char* TortureSound;
    bool IsActiveThreadTerminating() { return --termAfter < 0; }
    bool GetVictimIsGuard(const char* d) { return std::strcmp(d, "CREATURE_GUARD") == 0; }
    bool GetVictimIsBandit(const char* d) { return std::strcmp(d, "CREATURE_BANDIT") == 0; }
    bool GetVictimIsTrader(const char* d) { return std::strcmp(d, "CREATURE_TRADER") == 0; }
    void GetTortureSoundEffect(int sex) { TortureSound = sex ? "SND_F" : "SND_M"; }
    void RunCutsceneMacro(const char* name, const CHandle& hero, const CHandle& victim, const char* sound)
    {
        Log("cutscene"); Log(name); Log(hero.name); Log(victim.name); Log(sound); Log(PMasterData->flag ? "on" : "off");
    }
    void TortureVictim(CScriptThing& victim, CHandle& victimObject, CHandle& heroObject)
    {
        VictimHealth = (int)Game->GetHealth(victim);
        VictimGender = Game->EntityGetSex(victim);
        VictimNightTime = Game->GetTimeOfDay() > 2159 || Game->GetTimeOfDay() < 600;
        VictimSunday = Game->GetDayOfWeek() == 6;
        VictimIsGuard = GetVictimIsGuard(victim.GetDefName());
        VictimIsBandit = GetVictimIsBandit(victim.GetDefName());
        VictimIsTrader = GetVictimIsTrader(victim.GetDefName());
        GetTortureSoundEffect(VictimGender);
        PMasterData->flag = true;
        RunCutsceneMacro("CS_CHAPEL_EVIL_TORTURE", heroObject, victimObject, TortureSound);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        PMasterData->flag = false;
    }
};
static bool Run(int time, int day, const char* def, int sex, int term, bool expFlag, bool night, bool sunday, const char* tail)
{
    g_log[0] = 0;
    CGameScriptInterface g; g.time = time; g.day = day;
    CMaster m; m.flag = false;
    CV_ChapelOfEvilScript s; s.Game = &g; s.PMasterData = &m; s.termAfter = term;
    CScriptThing v; v.def = def; v.health = 42.75f; v.sex = sex;
    CHandle vh = { "victim" }, hh = { "hero" };
    s.TortureVictim(v, vh, hh);
    bool ok = std::strcmp(g_log, tail) == 0 && m.flag == expFlag && s.VictimHealth == 42 && s.VictimGender == sex
        && s.VictimNightTime == night && s.VictimSunday == sunday
        && s.VictimIsGuard == (std::strcmp(def, "CREATURE_GUARD") == 0) && s.VictimIsBandit == (std::strcmp(def, "CREATURE_BANDIT") == 0)
        && s.VictimIsTrader == (std::strcmp(def, "CREATURE_TRADER") == 0);
    if (!ok) std::printf("got %s\n", g_log);
    return ok;
}
int main()
{
    bool ok = Run(2200, 6, "CREATURE_GUARD", 0, 100, false, true, true, "cutscene;CS_CHAPEL_EVIL_TORTURE;hero;victim;SND_M;on;frame;")
        && Run(1200, 2, "CREATURE_BANDIT", 1, 100, false, false, false, "cutscene;CS_CHAPEL_EVIL_TORTURE;hero;victim;SND_F;on;frame;")
        && Run(500, 0, "CREATURE_TRADER", 0, 0, true, true, false, "cutscene;CS_CHAPEL_EVIL_TORTURE;hero;victim;SND_M;on;frame;")
        && Run(2159, 6, "CREATURE_X", 0, 100, false, false, true, "cutscene;CS_CHAPEL_EVIL_TORTURE;hero;victim;SND_M;on;frame;");
    if (!ok) { std::printf("00e6da80_TEST FAIL\n"); return 1; }
    std::printf("00e6da80_TEST PASS\n");
    return 0;
}