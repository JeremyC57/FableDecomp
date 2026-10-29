// Behaviour test for NScript::CV_ChapelOfEvilScript::OnPersist 0x00E690F0: the 29 persisted members go through
// CPersistContext::Transfer under their own names, in retail's order, with the right overload (long / bool / float) and a
// zero default; a loading context writes each value back into the member its name names, and the unpersisted members
// (victim gender, torture type) are left alone.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static int g_count;
class CPersistContext { public:
    bool loading;
    void Note(const char* name, const char* kind, int isDefaultZero)
    {
        std::strcat(g_log, name); std::strcat(g_log, kind); if (!isDefaultZero) std::strcat(g_log, "!"); std::strcat(g_log, ";"); ++g_count;
    }
    void Transfer(const char* name, bool& value, const bool& def) { Note(name, ":b", def == false); if (loading) value = true; }
    void Transfer(const char* name, float& value, const float& def) { Note(name, ":f", def == 0.0f); if (loading) value = 2.5f; }
    void Transfer(const char* name, long& value, const long& def) { Note(name, ":l", def == 0); if (loading) value = (long)std::strlen(name); }
};
struct CV_ChapelOfEvilScript {
    long DonationTotal, DonationThisVisit, DonationCounter;
    bool DonationInvalid, DisplayQuestInfo, WeaponWarningComplete, WeaponUnsheathed, ChapelIntroComplete, InsideChapel;
    bool AttackedByHero, AcolyteAttackMessageComplete, ReceivedReward, GivenCoolThing1, GivenCoolThing2, AwardWeapon;
    float MoralityInitial, MoralityCurrent, MoralityCumulative;
    bool QuestFinished, QuestComplete, QuestActivated;
    long TortureCount;
    bool VictimTortured;
    long VictimHealth;
    int VictimGender;
    bool VictimNightTime, VictimSunday, VictimIsGuard, VictimIsBandit, VictimIsTrader;
    int TortureType;
    void OnPersist(CPersistContext& context)
    {
        context.Transfer("DonationTotal", DonationTotal, 0L);
        context.Transfer("DonationThisVisit", DonationThisVisit, 0L);
        context.Transfer("DonationCounter", DonationCounter, 0L);
        context.Transfer("DonationInvalid", DonationInvalid, false);
        context.Transfer("DisplayQuestInfo", DisplayQuestInfo, false);
        context.Transfer("WeaponWarningComplete", WeaponWarningComplete, false);
        context.Transfer("WeaponUnsheathed", WeaponUnsheathed, false);
        context.Transfer("ChapelIntroComplete", ChapelIntroComplete, false);
        context.Transfer("InsideChapel", InsideChapel, false);
        context.Transfer("AttackedByHero", AttackedByHero, false);
        context.Transfer("AcolyteAttackMessageComplete", AcolyteAttackMessageComplete, false);
        context.Transfer("MoralityInitial", MoralityInitial, 0.0f);
        context.Transfer("MoralityCurrent", MoralityCurrent, 0.0f);
        context.Transfer("MoralityCumulative", MoralityCumulative, 0.0f);
        context.Transfer("QuestFinished", QuestFinished, false);
        context.Transfer("QuestActivated", QuestActivated, false);
        context.Transfer("QuestComplete", QuestComplete, false);
        context.Transfer("ReceivedReward", ReceivedReward, false);
        context.Transfer("GivenCoolThing1", GivenCoolThing1, false);
        context.Transfer("GivenCoolThing2", GivenCoolThing2, false);
        context.Transfer("AwardWeapon", AwardWeapon, false);
        context.Transfer("TortureCount", TortureCount, 0L);
        context.Transfer("VictimTortured", VictimTortured, false);
        context.Transfer("VictimHealth", VictimHealth, 0L);
        context.Transfer("VictimNightTime", VictimNightTime, false);
        context.Transfer("VictimSunday", VictimSunday, false);
        context.Transfer("VictimIsGuard", VictimIsGuard, false);
        context.Transfer("VictimIsBandit", VictimIsBandit, false);
        context.Transfer("VictimIsTrader", VictimIsTrader, false);
    }
};
int main()
{
    CV_ChapelOfEvilScript s;
    std::memset(&s, 0, sizeof(s));
    s.VictimGender = 7; s.TortureType = 3;
    CPersistContext save; save.loading = false;
    g_log[0] = 0; g_count = 0;
    s.OnPersist(save);
    const char* expect = "DonationTotal:l;DonationThisVisit:l;DonationCounter:l;DonationInvalid:b;DisplayQuestInfo:b;"
        "WeaponWarningComplete:b;WeaponUnsheathed:b;ChapelIntroComplete:b;InsideChapel:b;AttackedByHero:b;"
        "AcolyteAttackMessageComplete:b;MoralityInitial:f;MoralityCurrent:f;MoralityCumulative:f;QuestFinished:b;"
        "QuestActivated:b;QuestComplete:b;ReceivedReward:b;GivenCoolThing1:b;GivenCoolThing2:b;AwardWeapon:b;"
        "TortureCount:l;VictimTortured:b;VictimHealth:l;VictimNightTime:b;VictimSunday:b;VictimIsGuard:b;"
        "VictimIsBandit:b;VictimIsTrader:b;";
    bool ok = std::strcmp(g_log, expect) == 0 && g_count == 29;
    CPersistContext load; load.loading = true;
    s.OnPersist(load);
    ok = ok && s.DonationTotal == 13 && s.DonationThisVisit == 17 && s.DonationCounter == 15 && s.TortureCount == 12
        && s.VictimHealth == 12 && s.DonationInvalid && s.AttackedByHero && s.QuestComplete && s.VictimIsTrader
        && s.MoralityCumulative > 2.4f && s.MoralityCumulative < 2.6f && s.VictimGender == 7 && s.TortureType == 3;
    if (!ok) { std::printf("00e690f0_TEST FAIL\n"); return 1; }
    std::printf("00e690f0_TEST PASS\n");
    return 0;
}