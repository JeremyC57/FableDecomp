// Behaviour test for NScript::CV_TempleOfLightScript::OnPersist (0x00EDAF30): every persisted member is transferred once,
// under its own key, in retail's key order, with a zero / false default; a loading context resets each member to it.
#include <cstdio>
#include <cstring>

static const char* gKeys[32];
static int gCount;
static bool gLoad;

class CPersistContext { public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue)
    {
        if (gCount < 32) gKeys[gCount] = name;
        ++gCount;
        if (gLoad) value = defaultValue;
    }
};

struct CV_TempleOfLightScript {
    long DonationTotal, DonationThisVisit, DonationCounter;
    bool DonationInvalid, DonationActive, DisplayQuestInfo, WeaponWarningComplete, WeaponUnsheathed, TempleIntroComplete,
         InsideTemple, AttackedByHero, AcolyteAttackMessageComplete, AllAcolytesEscaped, ReceivedReward, GivenCoolThing1,
         GivenCoolThing2, AwardWeapon;
    float MoralityInitial, MoralityCurrent, MoralityCumulative;
    bool QuestFinished, QuestComplete, QuestActivated;
    void OnPersist(CPersistContext& context)
    {
        context.Transfer("DonationTotal", DonationTotal, 0L);
        context.Transfer("DonationThisVisit", DonationThisVisit, 0L);
        context.Transfer("DonationCounter", DonationCounter, 0L);
        context.Transfer("DonationInvalid", DonationInvalid, false);
        context.Transfer("DonationActive", DonationActive, false);
        context.Transfer("DisplayQuestInfo", DisplayQuestInfo, false);
        context.Transfer("WeaponWarningComplete", WeaponWarningComplete, false);
        context.Transfer("WeaponUnsheathed", WeaponUnsheathed, false);
        context.Transfer("TempleIntroComplete", TempleIntroComplete, false);
        context.Transfer("InsideTemple", InsideTemple, false);
        context.Transfer("AttackedByHero", AttackedByHero, false);
        context.Transfer("AcolyteAttackMessageComplete", AcolyteAttackMessageComplete, false);
        context.Transfer("AllAcolytesEscaped", AllAcolytesEscaped, false);
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
    }
};

int main()
{
    CV_TempleOfLightScript s;
    s.DonationTotal = 500; s.DonationCounter = 3; s.MoralityCurrent = 7.5f; s.QuestComplete = true; s.AwardWeapon = true;
    CPersistContext c;
    gCount = 0; gLoad = false;
    s.OnPersist(c);
    if (gCount != 23 || std::strcmp(gKeys[0], "DonationTotal") != 0 || std::strcmp(gKeys[13], "MoralityInitial") != 0
        || std::strcmp(gKeys[17], "QuestActivated") != 0 || std::strcmp(gKeys[22], "AwardWeapon") != 0
        || s.DonationTotal != 500 || s.MoralityCurrent != 7.5f) { std::printf("00edaf30_TEST FAIL\n"); return 1; }
    gCount = 0; gLoad = true;
    s.OnPersist(c);
    if (gCount != 23 || s.DonationTotal != 0 || s.DonationCounter != 0 || s.MoralityCurrent != 0.0f || s.QuestComplete
        || s.AwardWeapon) { std::printf("00edaf30_TEST FAIL\n"); return 1; }
    std::printf("00edaf30_TEST PASS\n");
    return 0;
}