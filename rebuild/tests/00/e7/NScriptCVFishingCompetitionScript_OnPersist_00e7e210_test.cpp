// Behaviour test for 0x00E7E210 NScript::CV_FishingCompetitionScript::OnPersist: twelve members are transferred under their
// member names in retail's order, the winner's name with the CCharString default, the others with 0 / 0.0f / false.
#include <cstdio>
#include <cstring>

static char g_log[1024];
class CCharString { public: const char* Text; CCharString() : Text("") {} };
class CPersistContext { public:
    void Note(const char* t, const char* name) { std::strcat(g_log, t); std::strcat(g_log, ":"); std::strcat(g_log, name); std::strcat(g_log, ";"); }
    void Transfer(const char* name, CCharString& v, const CCharString& d) { Note("s", name); v = d; }
    void Transfer(const char* name, CCharString& v) { Transfer(name, v, CCharString()); }
    void Transfer(const char* name, float& v, const float& d) { Note("f", name); v = d; }
    void Transfer(const char* name, bool& v, const bool& d) { Note("b", name); v = d; }
    void Transfer(const char* name, long& v, const long& d) { Note("l", name); v = d; }
};
struct CV_FishingCompetitionScript {
    bool BountyHuntActive, OrganiserRanAway, OrganiserKilled, CompetitionRunning, CompetitionWon, RodOfChampsGiven;
    bool CommentIRanAway, CompetitorsAttacked, CompetitorsJustGotAttacked;
    long AttackCounter, AttackDayCount, HitCount; bool DoneIntro, RodHintGiven; long PrizeNumber;
    CCharString Winner; float BiggestFish, BiggestCompetitorFish;
    void OnPersist(CPersistContext& context)
    {
        context.Transfer("Winner", Winner);
        context.Transfer("BiggestFish", BiggestFish, 0.0f);
        context.Transfer("BiggestCompetitorFish", BiggestCompetitorFish, 0.0f);
        context.Transfer("DoneIntro", DoneIntro, false);
        context.Transfer("PrizeNumber", PrizeNumber, 0L);
        context.Transfer("RodHintGiven", RodHintGiven, false);
        context.Transfer("CompetitionWon", CompetitionWon, false);
        context.Transfer("RodOfChampsGiven", RodOfChampsGiven, false);
        context.Transfer("BountyHuntActive", BountyHuntActive, false);
        context.Transfer("OrganiserRanAway", OrganiserRanAway, false);
        context.Transfer("OrganiserKilled", OrganiserKilled, false);
        context.Transfer("CompetitorsAttacked", CompetitorsAttacked, false);
    }
};

int main()
{
    CV_FishingCompetitionScript s;
    s.BountyHuntActive = s.OrganiserRanAway = s.OrganiserKilled = s.CompetitionRunning = s.CompetitionWon = true;
    s.RodOfChampsGiven = s.CommentIRanAway = s.CompetitorsAttacked = s.CompetitorsJustGotAttacked = true;
    s.DoneIntro = s.RodHintGiven = true; s.PrizeNumber = 4; s.HitCount = 3; s.BiggestFish = 1.5f; s.BiggestCompetitorFish = 2.5f;
    s.Winner.Text = "Hero";
    CPersistContext c;
    s.OnPersist(c);
    int ok = !std::strcmp(g_log, "s:Winner;f:BiggestFish;f:BiggestCompetitorFish;b:DoneIntro;l:PrizeNumber;b:RodHintGiven;"
                                 "b:CompetitionWon;b:RodOfChampsGiven;b:BountyHuntActive;b:OrganiserRanAway;b:OrganiserKilled;"
                                 "b:CompetitorsAttacked;")
             && !s.BountyHuntActive && !s.CompetitorsAttacked && s.CompetitionRunning && s.CommentIRanAway
             && s.CompetitorsJustGotAttacked && s.HitCount == 3 && s.PrizeNumber == 0 && s.BiggestFish == 0.0f && !s.Winner.Text[0];
    if (!ok) { std::printf("00e7e210_TEST FAIL\n"); return 1; }
    std::printf("00e7e210_TEST PASS\n");
    return 0;
}