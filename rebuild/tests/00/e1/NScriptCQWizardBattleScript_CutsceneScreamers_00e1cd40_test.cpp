#include <cstdio>
#include <cstring>
#include <map>
#include <string>
// Behaviour test for CQ_WizardBattleScript::CutsceneScreamers (0x00E1CD40): the hero is scripted and cast as "Hero", the
// screamers cut-scene runs inside a paused movie sequence, the quest start screen is kicked off only the first time, and
// a terminating thread skips it but still unpauses.
struct CHandle { int Id; };
struct CGSI {
    int Scripted; int Movies; int PauseDepth; int Cutscenes; int StartScreens; char LastCutscene[32];
    bool StartScriptingEntity(int hero, CHandle& h, int priority) { h.Id = hero; ++Scripted; return priority == 4; }
    int GetHero() { return 42; }
    void StartMovieSequence(const char*, int&) { ++Movies; }
    void PauseAllNonScriptedEntities(bool pause) { PauseDepth += pause ? 1 : -1; }
    void KickOffQuestStartScreen(const char* q, bool, bool) { if (std::strcmp(q, "Q_WizardBattle") == 0) ++StartScreens; }
};
static CGSI* g_gsi;
static void RunCutsceneMacro(const char* name, std::map<std::string, CHandle>& cast)
{ if (cast["Hero"].Id == 42) { ++g_gsi->Cutscenes; std::strcpy(g_gsi->LastCutscene, name); } }
struct CQ_WizardBattleScript {
    CGSI* GSI; bool QuestStartScreenShown; bool Terminating;
    bool IsActiveThreadTerminating() { return Terminating; }
    void CutsceneScreamers()
    {
        CHandle hero;
        GSI->StartScriptingEntity(GSI->GetHero(), hero, 4);
        std::map<std::string, CHandle> cast;
        cast["Hero"] = hero;
        int movie = 0;
        GSI->StartMovieSequence("", movie);
        CGSI* game = GSI;
        game->PauseAllNonScriptedEntities(true);
        RunCutsceneMacro("CS_HOOK_COAST_SCREAMERS", cast);
        if (QuestStartScreenShown == false) {
            if (IsActiveThreadTerminating()) {
                game->PauseAllNonScriptedEntities(false);
                return;
            }
            GSI->KickOffQuestStartScreen("Q_WizardBattle", true, false);
            QuestStartScreenShown = true;
        }
        game->PauseAllNonScriptedEntities(false);
    }
};
int main()
{
    CGSI gsi; std::memset(&gsi, 0, sizeof gsi); g_gsi = &gsi;
    CQ_WizardBattleScript s; s.GSI = &gsi; s.QuestStartScreenShown = false; s.Terminating = true;
    s.CutsceneScreamers();
    bool ok = gsi.Cutscenes == 1 && gsi.StartScreens == 0 && gsi.PauseDepth == 0 && !s.QuestStartScreenShown;
    s.Terminating = false;
    s.CutsceneScreamers();
    s.CutsceneScreamers();
    ok = ok && gsi.Cutscenes == 3 && gsi.StartScreens == 1 && gsi.PauseDepth == 0 && s.QuestStartScreenShown
            && gsi.Movies == 3 && gsi.Scripted == 3 && std::strcmp(gsi.LastCutscene, "CS_HOOK_COAST_SCREAMERS") == 0;
    if (!ok) { std::printf("00e1cd40_TEST FAIL\n"); return 1; }
    std::printf("00e1cd40_TEST PASS\n");
    return 0;
}