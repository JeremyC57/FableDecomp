// Behaviour test for 0x00E37520 NScript::CV_ArcheryCompetitionScript::OnPersist: the ten flags are transferred in retail's
// order under their member names, each with a false default (a recording persist context checks names, order and binding).
#include <cstdio>
#include <cstring>

class CPersistContext { public:
    char Log[512]; bool Loading;
    void Transfer(const char* name, bool& value, const bool& def)
    {
        std::strcat(Log, name); std::strcat(Log, def ? "=1;" : "=0;");
        if (Loading) value = true;
    }
};
struct CV_ArcheryCompetitionScript {
    long Score; bool IsOwnerDead, PrizeAlreadyGot, QuestActivated, AttackedByHero, GiveAttackWarning, QuestFinished, WelcomeComplete, Pickpocketed;
    long DummyCounter; bool SteppedOut, UsedMagic, ShotOwner, GivenReallyCoolPrize, GivenFirstHighScorePrize, AddedReallyCoolPrizeToOwner, AddedFirstHighScorePrizeToOwner;
    void OnPersist(CPersistContext& context)
    {
        context.Transfer("QuestActivated", QuestActivated, false);
        context.Transfer("PrizeAlreadyGot", PrizeAlreadyGot, false);
        context.Transfer("QuestFinished", QuestFinished, false);
        context.Transfer("WelcomeComplete", WelcomeComplete, false);
        context.Transfer("GivenReallyCoolPrize", GivenReallyCoolPrize, false);
        context.Transfer("GivenFirstHighScorePrize", GivenFirstHighScorePrize, false);
        context.Transfer("GiveAttackWarning", GiveAttackWarning, false);
        context.Transfer("AddedReallyCoolPrizeToOwner", AddedReallyCoolPrizeToOwner, false);
        context.Transfer("AddedFirstHighScorePrizeToOwner", AddedFirstHighScorePrizeToOwner, false);
        context.Transfer("Pickpocketed", Pickpocketed, false);
    }
};

int main()
{
    CV_ArcheryCompetitionScript s; std::memset(&s, 0, sizeof(s));
    CPersistContext c; c.Log[0] = 0; c.Loading = true;
    s.OnPersist(c);
    int ok = !std::strcmp(c.Log, "QuestActivated=0;PrizeAlreadyGot=0;QuestFinished=0;WelcomeComplete=0;GivenReallyCoolPrize=0;"
                                 "GivenFirstHighScorePrize=0;GiveAttackWarning=0;AddedReallyCoolPrizeToOwner=0;"
                                 "AddedFirstHighScorePrizeToOwner=0;Pickpocketed=0;")
             && s.QuestActivated && s.PrizeAlreadyGot && s.QuestFinished && s.WelcomeComplete && s.GivenReallyCoolPrize
             && s.GivenFirstHighScorePrize && s.GiveAttackWarning && s.AddedReallyCoolPrizeToOwner
             && s.AddedFirstHighScorePrizeToOwner && s.Pickpocketed
             && !s.IsOwnerDead && !s.AttackedByHero && !s.SteppedOut && !s.UsedMagic && !s.ShotOwner;
    if (!ok) { std::printf("00e37520_TEST FAIL\n"); return 1; }
    std::printf("00e37520_TEST PASS\n");
    return 0;
}