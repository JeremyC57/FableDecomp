#include <cstdio>
#include <cstring>
// Behaviour test for CQ_NewOakValeIntroScript::AddGoodDeed (0x00DB0660): counts the deed, gives morality, shows the
// first/next message and waits for the click, adds the map tutorial on the first deed, sets the gold objective while
// gold < 3 and no sweets were given, then updates the counter.
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CGSI {
    int Frames; int ClickAt; long Gold;
    void NewScriptFrame() { ++Frames; }
    bool MsgIsGameInfoClickedPast() { return Frames >= ClickAt; }
    void DisplayGameInfo(const char* t) { char b[96]; std::sprintf(b, "Info:%s", t); Log(b); }
    long GetHeroGold() { return Gold; }
    void GiveHeroMorality(float m) { char b[32]; std::sprintf(b, "Morality:%g", m); Log(b); }
    const char* GetActiveQuestName() { return "Q_Intro"; }
    void SetQuestCardObjective(const char* q, const char* o, const char*, const char*) { char b[96]; std::sprintf(b, "Obj:%s:%s", q, o); Log(b); }
    void UpdateQuestInfoCounter(long c, long v, long t) { char b[48]; std::sprintf(b, "Counter:%ld:%ld:%ld", c, v, t); Log(b); }
};
static void AddLogbookTutorialEntry(const char* t) { char b[64]; std::sprintf(b, "Log:%s", t); Log(b); }
struct CDef { float OVI_MoralityChangePerDeed; };
static CDef* g_pScriptDef;
struct CQ_NewOakValeIntroScript {
    CGSI* GSI; long GoodDeedsPerformed; long BadDeedsPerformed; long GUIGoodDeedCounter; bool GivenSweets;
    bool IsActiveThreadTerminating() { return false; }
    void AddGoodDeed()
    {
        ++GoodDeedsPerformed;
        GSI->GiveHeroMorality(g_pScriptDef->OVI_MoralityChangePerDeed);
        if (GoodDeedsPerformed == 1 && BadDeedsPerformed == 0) {
            if (IsActiveThreadTerminating()) return;
            GSI->DisplayGameInfo("TEXT_QST_048_SCRMSG_DID_FIRST_GOOD_DEED");
            while (!GSI->MsgIsGameInfoClickedPast()) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            AddLogbookTutorialEntry("TEXT_QST_LOG_BASICS_MAP");
        } else {
            if (IsActiveThreadTerminating()) return;
            GSI->DisplayGameInfo("TEXT_QST_048_SCRMSG_DID_GOOD_DEED");
            while (!GSI->MsgIsGameInfoClickedPast()) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
        }
        if (GSI->GetHeroGold() < 3 && !GivenSweets) {
            if (IsActiveThreadTerminating()) return;
            GSI->SetQuestCardObjective(GSI->GetActiveQuestName(), "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_02", "", "");
        }
        GSI->UpdateQuestInfoCounter(GUIGoodDeedCounter, GoodDeedsPerformed, -1);
    }
};
int main()
{
    CDef def; def.OVI_MoralityChangePerDeed = 2.5f; g_pScriptDef = &def;
    CGSI gsi; gsi.Frames = 0; gsi.ClickAt = 3; gsi.Gold = 1;
    CQ_NewOakValeIntroScript s; s.GSI = &gsi; s.GoodDeedsPerformed = 0; s.BadDeedsPerformed = 0; s.GUIGoodDeedCounter = 9; s.GivenSweets = false;
    g_log[0] = 0; s.AddGoodDeed();
    bool ok = gsi.Frames == 3 && std::strcmp(g_log, "Morality:2.5;Info:TEXT_QST_048_SCRMSG_DID_FIRST_GOOD_DEED;Log:TEXT_QST_LOG_BASICS_MAP;Obj:Q_Intro:TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_02;Counter:9:1:-1;") == 0;
    gsi.Gold = 5; g_log[0] = 0; s.AddGoodDeed();
    ok = ok && std::strcmp(g_log, "Morality:2.5;Info:TEXT_QST_048_SCRMSG_DID_GOOD_DEED;Counter:9:2:-1;") == 0;
    if (!ok) { std::printf("00db0660_TEST FAIL\n"); return 1; }
    std::printf("00db0660_TEST PASS\n");
    return 0;
}