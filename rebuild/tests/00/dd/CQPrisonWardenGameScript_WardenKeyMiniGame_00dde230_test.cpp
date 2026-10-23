#include <cstdio>
#include <cstring>
// Behaviour test for CQ_PrisonWardenGameScript::WardenKeyMiniGame (0x00DDE230): the key rack is targetable only while
// the hero is close; once the hero uses it the diary key is taken, the race's year code is shown and the sparkles removed.
struct CScriptThing { int Id; };
struct CGSI {
    int Frames; int UseFrame; bool Targetable; int TargetCalls; char Info[64]; bool SparklesRemoved; int ClickFrame;
    void NewScriptFrame() { ++Frames; }
    bool Near() { return Frames >= 2; }
    bool MsgIsUsedBy(const char* who) { return Frames >= UseFrame && !std::strcmp(who, "SCRIPT_NAME_HERO"); }
    void EntitySetTargetable(const CScriptThing&, bool t) { Targetable = t; ++TargetCalls; }
    void DisplayGameInfo(const char* t) { std::strcpy(Info, t); ClickFrame = Frames + 2; }
    bool MsgIsGameInfoClickedPast() { return Frames >= ClickFrame; }
    void RemoveThing(const CScriptThing& t, bool, bool) { SparklesRemoved = t.Id == 2; }
};
struct CData { long PrisonRaceNumber; };
struct CQ_PrisonWardenGameScript {
    CGSI* GSI; CData* PMasterData; bool HeroHasSecretDiaryKey;
    bool IsActiveThreadTerminating() { return GSI->Frames > 30; }
    void WardenKeyMiniGame()
    {
        CScriptThing keyRack; keyRack.Id = 1;
        CScriptThing sparkles; sparkles.Id = 2;
        for (;;) {
            if (IsActiveThreadTerminating()) return;
            if (!GSI->Near()) { if (IsActiveThreadTerminating()) return; GSI->EntitySetTargetable(keyRack, false); }
            else { if (IsActiveThreadTerminating()) return; GSI->EntitySetTargetable(keyRack, true); }
            if (GSI->MsgIsUsedBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return;
                HeroHasSecretDiaryKey = true;
                GSI->EntitySetTargetable(keyRack, false);
                if (PMasterData->PrisonRaceNumber % 2 == 0) {
                    if (IsActiveThreadTerminating()) return;
                    GSI->DisplayGameInfo("TEXT_QST_055_DIARY_CODE_NUMBER_YEAR_TWO");
                    while (!GSI->MsgIsGameInfoClickedPast()) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                    if (IsActiveThreadTerminating()) return;
                } else {
                    if (IsActiveThreadTerminating()) return;
                    GSI->DisplayGameInfo("TEXT_QST_055_DIARY_CODE_NUMBER_YEAR_ONE");
                    while (!GSI->MsgIsGameInfoClickedPast()) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                    if (IsActiveThreadTerminating()) return;
                }
                GSI->RemoveThing(sparkles, false, true);
                return;
            }
            GSI->NewScriptFrame();
        }
    }
};
int main()
{
    CGSI gsi; std::memset(&gsi, 0, sizeof gsi); gsi.UseFrame = 5;
    CData data; data.PrisonRaceNumber = 3;
    CQ_PrisonWardenGameScript s; s.GSI = &gsi; s.PMasterData = &data; s.HeroHasSecretDiaryKey = false;
    s.WardenKeyMiniGame();
    bool ok = s.HeroHasSecretDiaryKey && !gsi.Targetable && gsi.SparklesRemoved && gsi.Frames == 7
              && !std::strcmp(gsi.Info, "TEXT_QST_055_DIARY_CODE_NUMBER_YEAR_ONE") && gsi.TargetCalls == 7;
    if (!ok) { std::printf("00dde230_TEST FAIL %d %d\n", gsi.Frames, gsi.TargetCalls); return 1; }
    std::printf("00dde230_TEST PASS\n");
    return 0;
}