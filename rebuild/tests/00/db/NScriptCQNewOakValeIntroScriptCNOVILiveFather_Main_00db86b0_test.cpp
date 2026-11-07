// Behaviour test for NScript::CQ_NewOakValeIntroScript::CNOVI_LiveFather::Main (0x00DB86B0): a model of the control flow over a
// scripted fake game (intro once with the Xbox / PC hint, penny reward per new good deed, deed comments, temper when hit).
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Game {
    int frame, stopAt, gold, talkAt, hitAt, clickAt;
    bool xbox, chocolate;
    long counter;
    void NewScriptFrame() { ++frame; }
    bool StartScriptingEntity() { return true; }
    bool IsXbox() { return xbox; }
    void DisplayGameInfo(const char* t) { Log(t); }
    bool MsgIsGameInfoClickedPast() { return frame >= clickAt; }
    long AddQuestInfoCounter(const char* t) { Log(t); return 77; }
    void RunCutscene(const char* t) { Log(t); }
    void GiveHeroGold(long n) { gold += (int)n; Log("gold"); }
    long GetHeroGold() { return gold; }
    bool HasChocolate() { return chocolate; }
    void ClearThingHasInformation() { Log("clearinfo"); }
    void SetQuestCardObjective(const char* t) { Log(t); }
    void Speak(const char* t) { Log(t); }
    bool TalkedTo() { return frame == talkAt; }
    bool HitBy() { return frame == hitAt; }
};
struct Parent { bool DadFinishedIntro; long GoodDeedsPerformed, BadDeedsPerformed, GUIGoodDeedCounter; int badDeedType;
    void AddBadDeed(int t) { badDeedType = t; ++BadDeedsPerformed; Log("baddeed"); } };
struct LiveFather {
    Game* G; Parent* P; long PenniesGiven;
    bool T() { return G->frame >= G->stopAt; }
    void Say(const char* t) { G->Speak(t); }
    void Main()
    {
        G->NewScriptFrame();
        if (T()) return;
        if (!P->DadFinishedIntro) {
            if (T()) return;
            G->RunCutscene("CS_OAKVALE_INTRO_FATHER");
            P->DadFinishedIntro = true;
            if (G->IsXbox()) {
                if (T()) return;
                G->DisplayGameInfo("TEXT_QST_048_INSTRUCTION_HIGHLIGHTING");
                while (!G->MsgIsGameInfoClickedPast()) { G->NewScriptFrame(); if (T()) return; }
                if (T()) return;
            } else {
                if (T()) return;
                G->DisplayGameInfo("TEXT_QST_048_INSTRUCTION_HIGHLIGHTING_PC");
                while (!G->MsgIsGameInfoClickedPast()) { G->NewScriptFrame(); if (T()) return; }
                if (T()) return;
            }
            P->GUIGoodDeedCounter = G->AddQuestInfoCounter("HUD_DEED_GOOD_ICON");
        }
        for (;; G->NewScriptFrame()) {
            if (T()) return;
            if (G->TalkedTo()) {
                if (P->GoodDeedsPerformed == 0 && P->BadDeedsPerformed == 0) Say("TEXT_QST_048_DAD_DONE_NOTHING_YET");
                else if (P->GoodDeedsPerformed > PenniesGiven) {
                    long reward = P->GoodDeedsPerformed - PenniesGiven;
                    PenniesGiven += reward;
                    G->GiveHeroGold(reward);
                    if (P->BadDeedsPerformed == 0) Say("TEXT_QST_048_DAD_GIVE_REWARD_JUST_GOOD");
                    else Say("TEXT_QST_048_DAD_GIVE_REWARD_PART_BAD");
                    if (G->HasChocolate()) { Say("TEXT_QST_048_DAD_GIVE_PRESENT"); G->ClearThingHasInformation(); }
                    else if (G->GetHeroGold() > 3) { Say("TEXT_QST_048_DAD_YOU_HAVE_ENOUGH"); G->ClearThingHasInformation(); }
                    else { Say("TEXT_QST_048_DAD_IS_ENOUGH"); G->SetQuestCardObjective("TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01"); }
                }
                else if (P->BadDeedsPerformed > 0) Say("TEXT_QST_048_DAD_ANTISOCIAL");
                else {
                    Say("TEXT_QST_048_DAD_DO_MORE");
                    if (G->HasChocolate()) { G->ClearThingHasInformation(); Say("TEXT_QST_048_DAD_GIVE_PRESENT_ALT"); }
                }
            }
            if (G->HitBy()) { P->AddBadDeed(2); Say("TEXT_QST_048_DAD_TEMPER"); }
        }
    }
};
static bool Run(bool xbox, long good, long bad, int gold, bool choc, int talkAt, int hitAt, const char* want, int wantGold)
{
    g_log[0] = 0;
    Game g; std::memset(&g, 0, sizeof g); g.xbox = xbox; g.gold = gold; g.chocolate = choc; g.talkAt = talkAt; g.hitAt = hitAt; g.clickAt = 2; g.stopAt = 8;
    Parent p; std::memset(&p, 0, sizeof p); p.GoodDeedsPerformed = good; p.BadDeedsPerformed = bad;
    LiveFather f; f.G = &g; f.P = &p; f.PenniesGiven = 0;
    f.Main();
    if (std::strcmp(g_log, want) != 0) { std::printf("got %s\n", g_log); return false; }
    return g.gold == wantGold && p.DadFinishedIntro && p.GUIGoodDeedCounter == 77;
}
int main()
{
    bool ok = Run(true, 0, 0, 0, false, 4, -1, "CS_OAKVALE_INTRO_FATHER;TEXT_QST_048_INSTRUCTION_HIGHLIGHTING;HUD_DEED_GOOD_ICON;TEXT_QST_048_DAD_DONE_NOTHING_YET;", 0)
        && Run(false, 2, 0, 1, false, 3, -1, "CS_OAKVALE_INTRO_FATHER;TEXT_QST_048_INSTRUCTION_HIGHLIGHTING_PC;HUD_DEED_GOOD_ICON;gold;TEXT_QST_048_DAD_GIVE_REWARD_JUST_GOOD;TEXT_QST_048_DAD_IS_ENOUGH;TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01;", 3)
        && Run(false, 3, 1, 2, true, 3, 5, "CS_OAKVALE_INTRO_FATHER;TEXT_QST_048_INSTRUCTION_HIGHLIGHTING_PC;HUD_DEED_GOOD_ICON;gold;TEXT_QST_048_DAD_GIVE_REWARD_PART_BAD;TEXT_QST_048_DAD_GIVE_PRESENT;clearinfo;baddeed;TEXT_QST_048_DAD_TEMPER;", 5)
        && Run(true, 0, 0, 0, true, 3, -1, "CS_OAKVALE_INTRO_FATHER;TEXT_QST_048_INSTRUCTION_HIGHLIGHTING;HUD_DEED_GOOD_ICON;TEXT_QST_048_DAD_DONE_NOTHING_YET;", 0);
    if (!ok) { std::printf("00db86b0_TEST FAIL\n"); return 1; }
    std::printf("00db86b0_TEST PASS\n");
    return 0;
}