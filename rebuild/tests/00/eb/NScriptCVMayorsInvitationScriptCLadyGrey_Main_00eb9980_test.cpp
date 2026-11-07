// Behaviour test for NScript::CV_MayorsInvitationScript::CLadyGrey::Main (0x00EB9980): a log model of the quest stages by
// WhichQuest -- a struck deal jumps to stage 5, stage 0 plays the intro and gives the quest card, 1 accepts the black rose or the
// carrot, 2 needs a house, 3 the necklace / Thunder (killing Thunder with the necklace in hand defeats him), 4 waits for the deal,
// then the Thunder-duel wait and the ending: the marriage offer when Thunder is defeated (yes succeeds, no fails), otherwise rejection.
#include <cstdio>
#include <cstring>
struct Sim {
    char log[512]; int WhichQuest; bool StruckDeal, Defeated, Lost, KilledThunder, HasNecklace, CardGiven, Succeeded, Failed;
    const char* item; int houses; bool acceptMarriage;
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    void Main()
    {
        if (StruckDeal) WhichQuest = 5;
        if (WhichQuest == 5 && !Defeated && !Lost) Ev("DUEL");
        if (WhichQuest == 0) { Ev("INTRO"); WhichQuest = 1; if (!CardGiven) { Ev("CARD"); CardGiven = true; } }
        while (WhichQuest == 1) {
            if (!std::strcmp(item, "OBJECT_ROSE_BLACK_01") || !std::strcmp(item, "OBJECT_CARROT")) { Ev("ROSE"); WhichQuest = 2; }
            else item = "OBJECT_ROSE_BLACK_01";
        }
        while (WhichQuest == 2) { if (houses >= 1) { Ev("HOUSE"); WhichQuest = 3; } else { Ev("HOUSE_FAIL"); houses = 1; } }
        while (WhichQuest == 3) {
            if (KilledThunder && HasNecklace) { WhichQuest = 5; Defeated = true; Ev("THUNDER"); }
            else { Ev("FEARLESS"); WhichQuest = 4; }
        }
        while (WhichQuest == 4 && !StruckDeal) { Ev("DEAL"); StruckDeal = true; }
        if (!Defeated && !Lost) { Ev("WAIT"); Lost = true; }
        if (Defeated) { Ev("OFFER"); if (acceptMarriage) Succeeded = true; else Failed = true; }
        else { Ev("REJECTED"); Failed = true; }
    }
};
static void Make(Sim& s)
{
    s.log[0] = 0; s.WhichQuest = 0; s.StruckDeal = s.Defeated = s.Lost = s.KilledThunder = s.HasNecklace = s.CardGiven = false;
    s.Succeeded = s.Failed = false; s.item = ""; s.houses = 0; s.acceptMarriage = true;
}
int main()
{
    Sim a; Make(a); a.KilledThunder = a.HasNecklace = true; a.houses = 1; a.Main();
    bool ok = !std::strcmp(a.log, "INTRO;CARD;ROSE;HOUSE;THUNDER;OFFER;") && a.Succeeded && !a.Failed;
    Sim b; Make(b); b.item = "OBJECT_CARROT"; b.Main();
    ok = ok && !std::strcmp(b.log, "INTRO;CARD;ROSE;HOUSE_FAIL;HOUSE;FEARLESS;DEAL;WAIT;REJECTED;") && b.Failed;
    Sim c; Make(c); c.StruckDeal = true; c.Defeated = true; c.acceptMarriage = false; c.Main();
    ok = ok && !std::strcmp(c.log, "OFFER;") && c.Failed && !c.Succeeded;
    std::printf(ok ? "00eb9980_TEST PASS\n" : "00eb9980_TEST FAIL\n");
    return ok ? 0 : 1;
}