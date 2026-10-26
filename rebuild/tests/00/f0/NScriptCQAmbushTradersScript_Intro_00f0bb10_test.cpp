#include <cstdio>
#include <cstring>
// behaviour test: CQ_AmbushTradersScript::Intro -- script four bandits and the hero, cast BAN1..BAN4/HERO, play the intro
// cut-scene with the quest's flag map inside a paused movie sequence, release all five handles, mark the intro done
static int g_scripted = 0, g_played = 0, g_paused = 0, g_fixed = 0, g_released = 0;
class CScriptThing { public: const char* name; };
struct CHandle { const CScriptThing* thing; CHandle() : thing(0) {} bool IsScripting() { return thing != 0; } void Release() { thing = 0; ++g_released; } };
struct CCastMap { const char* keys[5]; const CScriptThing* things[5]; int n; CCastMap() : n(0) {}
    void Set(const char* k, const CHandle& h) { keys[n] = k; things[n] = h.thing; ++n; } };
struct CFlagMap { int marker; };
class CGameScriptInterface { public:
    CScriptThing hero;
    bool StartScriptingEntity(const CScriptThing& t, CHandle& h, int priority) { if (priority != 4) return false; h.thing = &t; ++g_scripted; return true; }
    CScriptThing& GetHero() { return hero; }
    void StartMovieSequence(const char*, int&) {}
    void FixMovieSequenceCamera(bool f) const { g_fixed += f ? 1 : -1; }
    void PauseAllNonScriptedEntities(bool p) { g_paused += p ? 1 : -1; }
};
static void RunCutsceneMacro(const char* name, CCastMap& cast, CFlagMap* flags, void* args, bool a5, bool a6)
{
    if (!std::strcmp(name, "CS_GIBBETHILL_AMBUSH_INTRO") && cast.n == 5 && flags && flags->marker == 7 && !args && !a5 && a6
        && !std::strcmp(cast.keys[0], "HERO") && !std::strcmp(cast.things[0]->name, "Hero")
        && !std::strcmp(cast.keys[4], "BAN4") && !std::strcmp(cast.things[4]->name, "Bandit4") && g_paused == 1 && g_fixed == 1)
        ++g_played;
}
struct CQ_AmbushTradersScript {
    CGameScriptInterface* Game;
    bool BanditIntroDone;
    CScriptThing AllBandits[4];
    CFlagMap CSFlagMap;
    void Intro();
};
void CQ_AmbushTradersScript::Intro()
{
    CHandle bandit1, bandit2, bandit3, bandit4;
    Game->StartScriptingEntity(AllBandits[0], bandit1, 4);
    Game->StartScriptingEntity(AllBandits[1], bandit2, 4);
    Game->StartScriptingEntity(AllBandits[2], bandit3, 4);
    Game->StartScriptingEntity(AllBandits[3], bandit4, 4);
    CHandle hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, 4);
    {
        CCastMap cast;
        cast.Set("HERO", hero); cast.Set("BAN1", bandit1); cast.Set("BAN2", bandit2); cast.Set("BAN3", bandit3); cast.Set("BAN4", bandit4);
        int movie = 0; Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_GIBBETHILL_AMBUSH_INTRO", cast, &CSFlagMap, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        game->PauseAllNonScriptedEntities(false);
    }
    if (bandit1.IsScripting()) bandit1.Release();
    if (bandit2.IsScripting()) bandit2.Release();
    if (bandit3.IsScripting()) bandit3.Release();
    if (bandit4.IsScripting()) bandit4.Release();
    if (hero.IsScripting()) hero.Release();
    BanditIntroDone = true;
}
int main()
{
    CGameScriptInterface game; game.hero.name = "Hero";
    CQ_AmbushTradersScript s; s.Game = &game; s.BanditIntroDone = false; s.CSFlagMap.marker = 7;
    s.AllBandits[0].name = "Bandit1"; s.AllBandits[1].name = "Bandit2"; s.AllBandits[2].name = "Bandit3"; s.AllBandits[3].name = "Bandit4";
    s.Intro();
    bool ok = g_scripted == 5 && g_played == 1 && g_paused == 0 && g_fixed == 0 && g_released == 5 && s.BanditIntroDone;
    if (!ok) { std::printf("00f0bb10_TEST FAIL\n"); return 1; }
    std::printf("00f0bb10_TEST PASS\n");
    return 0;
}