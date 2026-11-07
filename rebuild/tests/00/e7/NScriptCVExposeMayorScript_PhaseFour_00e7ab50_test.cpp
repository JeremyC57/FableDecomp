// Behaviour test for NScript::CV_ExposeMayorScript::PhaseFour (0x00E7AB50): a copy of the stage-3 logic over a mock game and
// a scripted event log; checks the married (bribe) path, the yes answer (thunder-dead variant) and the no (expose) answer,
// the stage-mismatch no-op and the terminate exits.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

struct MockGame {
    std::vector<std::string> log;
    int levelFramesLeft, evidenceFramesLeft, answerFramesLeft, answer;
    int paused;
    bool IsLevelLoaded(const char*) { return levelFramesLeft-- <= 0; }
    bool IsObjectInThingsPossession(long, long) { return evidenceFramesLeft-- <= 0; }
    long MsgIsQuestionAnsweredYesOrNo() { return answerFramesLeft-- <= 0 ? answer : -1; }
    void NewScriptFrame() { log.push_back("frame"); }
    long GetHero() { return 7; }
    const char* GetActiveQuestName() { return "QuestExposeMayor"; }
    void SetQuestCardObjective(const char*, const char* text, const char*, const char*) { log.push_back(std::string("objective:") + text); }
    void StartMovieSequence(const char*, int) { log.push_back("movie"); }
    void PauseAllNonScriptedEntities(bool on) { paused += on ? 1 : -1; log.push_back(on ? "pause" : "unpause"); }
    void GiveHeroYesNoQuestion(const char* q, const char*, const char*, const char*, bool) { log.push_back(std::string("question:") + q); }
    void TakeObjectFromHero(long) { log.push_back("take"); }
    void RemoveQuestCardFromHero(const char*) { log.push_back("removecard"); }
};
struct MasterData { bool DefeatedThunder, KilledThunder, HeroMarriedLadyGrey; };
struct Pause { MockGame* g; Pause(MockGame& game) : g(&game) { g->PauseAllNonScriptedEntities(true); } ~Pause() { g->PauseAllNonScriptedEntities(false); } };

struct Script {
    MockGame* Game; MasterData* MasterScriptData; long Path, StageNumber; bool MayorExposeGhostCutscenePlayed; long Evidence;
    int terminateAt, checks;
    bool IsActiveThreadTerminating() { return ++checks == terminateAt; }
    void PlayCutscene(const char* name, bool, const char*) { Game->log.push_back(std::string("cs:") + name); }
    void PhaseFour()
    {
        if (StageNumber == 3) {
            if (IsActiveThreadTerminating()) return;
            while (!Game->IsLevelLoaded("HauntedHouseCellar")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            if (!MayorExposeGhostCutscenePlayed) {
                if (IsActiveThreadTerminating()) return;
                PlayCutscene("CS_MAYOREXPOSE_GHOST", true, "NULL");
                MayorExposeGhostCutscenePlayed = true;
            }
            Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_EXPOSE_MAYOR_OBJECTIVE_04", "HauntedHouseCellar", "");
            while (!Game->IsObjectInThingsPossession(Evidence, Game->GetHero())) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            if (MasterScriptData->HeroMarriedLadyGrey) {
                if (IsActiveThreadTerminating()) return;
                int movie = 0; Game->StartMovieSequence("", movie);
                Pause PauseMyEntitiesPlease(*Game);
                PlayCutscene("CS_MAYOREXPOSE_GREYMARRIED", false, "NULL");
                Game->TakeObjectFromHero(Evidence);
                Game->RemoveQuestCardFromHero(Game->GetActiveQuestName());
                Path = 2;
            } else {
                if (IsActiveThreadTerminating()) return;
                int movie = 0; Game->StartMovieSequence("", movie);
                Pause PauseMyEntitiesPlease(*Game);
                PlayCutscene("CS_MAYOREXPOSE_GREYCHOICE", false, "NULL");
                Game->GiveHeroYesNoQuestion("TEXT_QST_B18_LADYGREY_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                long answer = Game->MsgIsQuestionAnsweredYesOrNo();
                while (answer < 0) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; answer = Game->MsgIsQuestionAnsweredYesOrNo(); }
                if (IsActiveThreadTerminating()) return;
                if (answer == 1) {
                    if (IsActiveThreadTerminating()) return;
                    if (MasterScriptData->KilledThunder || MasterScriptData->DefeatedThunder) {
                        if (IsActiveThreadTerminating()) return;
                        PlayCutscene("CS_MAYOREXPOSE_GREYAGREE_THUNDERDEAD", false, "NULL");
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        PlayCutscene("CS_MAYOREXPOSE_GREYAGREE", false, "NULL");
                    }
                    Game->TakeObjectFromHero(Evidence);
                    Path = 2;
                } else {
                    if (IsActiveThreadTerminating()) return;
                    PlayCutscene("CS_MAYOREXPOSE_GREYREFUSE", false, "NULL");
                    Path = 1;
                }
            }
        }
        StageNumber = 4;
    }
};

static std::string Join(const std::vector<std::string>& v) { std::string s; for (size_t i = 0; i < v.size(); ++i) { if (i) s += " "; s += v[i]; } return s; }
static bool Fail() { std::printf("00e7ab50_TEST FAIL\n"); return false; }
static bool Run(bool married, int answer, bool thunder, int terminateAt, const char* expected, long expectPath, long expectStage)
{
    MockGame g; g.levelFramesLeft = 2; g.evidenceFramesLeft = 1; g.answerFramesLeft = 1; g.answer = answer; g.paused = 0;
    MasterData m; m.DefeatedThunder = thunder; m.KilledThunder = false; m.HeroMarriedLadyGrey = married;
    Script s; s.Game = &g; s.MasterScriptData = &m; s.Path = 0; s.StageNumber = 3; s.MayorExposeGhostCutscenePlayed = false; s.Evidence = 9;
    s.terminateAt = terminateAt; s.checks = 0;
    s.PhaseFour();
    if (Join(g.log) != expected || s.Path != expectPath || s.StageNumber != expectStage || g.paused != 0) return Fail();
    return true;
}

int main()
{
    if (!Run(true, -1, false, 0, "frame frame cs:CS_MAYOREXPOSE_GHOST objective:TEXT_QUEST_EXPOSE_MAYOR_OBJECTIVE_04 frame movie pause cs:CS_MAYOREXPOSE_GREYMARRIED take removecard unpause", 2, 4)) return 1;
    if (!Run(false, 1, true, 0, "frame frame cs:CS_MAYOREXPOSE_GHOST objective:TEXT_QUEST_EXPOSE_MAYOR_OBJECTIVE_04 frame movie pause cs:CS_MAYOREXPOSE_GREYCHOICE question:TEXT_QST_B18_LADYGREY_QUESTION frame cs:CS_MAYOREXPOSE_GREYAGREE_THUNDERDEAD take unpause", 2, 4)) return 1;
    if (!Run(false, 0, false, 0, "frame frame cs:CS_MAYOREXPOSE_GHOST objective:TEXT_QUEST_EXPOSE_MAYOR_OBJECTIVE_04 frame movie pause cs:CS_MAYOREXPOSE_GREYCHOICE question:TEXT_QST_B18_LADYGREY_QUESTION frame cs:CS_MAYOREXPOSE_GREYREFUSE unpause", 1, 4)) return 1;
    // terminate on the second check (inside the level wait): nothing else happens and the stage stays 3
    if (!Run(true, -1, false, 2, "frame", 0, 3)) return 1;
    // stage mismatch: the body is skipped, the stage is still set to 4
    { MockGame g; g.levelFramesLeft = 0; g.evidenceFramesLeft = 0; g.answerFramesLeft = 0; g.answer = 0; g.paused = 0; MasterData m; m.DefeatedThunder = m.KilledThunder = m.HeroMarriedLadyGrey = false;
      Script s; s.Game = &g; s.MasterScriptData = &m; s.Path = 0; s.StageNumber = 2; s.MayorExposeGhostCutscenePlayed = true; s.Evidence = 1; s.terminateAt = 0; s.checks = 0;
      s.PhaseFour(); if (!g.log.empty() || s.StageNumber != 4) { Fail(); return 1; } }
    std::printf("00e7ab50_TEST PASS\n");
    return 0;
}