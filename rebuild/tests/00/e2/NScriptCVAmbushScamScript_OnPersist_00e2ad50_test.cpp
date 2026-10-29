// Behaviour test for NScript::CV_AmbushScamScript::OnPersist 0x00E2AD50: every persisted member goes through
// CPersistContext::Transfer under its own name, in retail's order, with the right overload (bool / float / long) and a
// zero default; a loading context writes each value back into the member its name names.
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
struct CV_AmbushScamScript {
    bool QuestFinished, QuestComplete, QuestActivated, LeftRegion, MoralitySwitch, BuddyIntroComplete, DoTricksterIntro, DoAmbush;
    bool TricksterApproachComplete, TricksterSpeechComplete, TricksterRetreatComplete, TricksterQuestComplete;
    bool TricksterWhiningComplete, TricksterDeadOrLeft, TricksterComplete, TricksterKilledBeforeAmbush, TricksterKilledAfterAmbush;
    bool TricksterKilledAfterBeingSpared, TricksterResetAllegience, TricksterWaitHere;
    float TricksterApproachProximity;
    bool AmbushSpringComplete, AllAmbushersDead, ExpressionDetectedFollow, ExpressionDetectedWait, HeroAttackOnBuddy;
    bool HeroAttackOnTricksterBeforeAmbush, HeroAttackOnTricksterAfterAmbush, HeroAttackOnTricksterKillingBlow, HeroTalkToTrickster;
    bool HeroAttachOnTricksterWarning, BuddyHelped, SpeakTime1, SpeakTime2, AllDone, BanditSpoken, AllBanditsDead, LeaderDead;
    long HitCount, BuddyCounter;
    void OnPersist(CPersistContext& context)
    {
        context.Transfer("QuestFinished", QuestFinished, false);
        context.Transfer("QuestComplete", QuestComplete, false);
        context.Transfer("QuestActivated", QuestActivated, false);
        context.Transfer("LeftRegion", LeftRegion, false);
        context.Transfer("MoralitySwitch", MoralitySwitch, false);
        context.Transfer("AllAmbushersDead", AllAmbushersDead, false);
        context.Transfer("BuddyHelped", BuddyHelped, false);
        context.Transfer("HeroAttackOnBuddy", HeroAttackOnBuddy, false);
        context.Transfer("HeroAttackOnTricksterAfterAmbush", HeroAttackOnTricksterAfterAmbush, false);
        context.Transfer("HeroAttackOnTricksterBeforeAmbush", HeroAttackOnTricksterBeforeAmbush, false);
        context.Transfer("HeroAttackOnTricksterKillingBlow", HeroAttackOnTricksterKillingBlow, false);
        context.Transfer("HeroAttachOnTricksterWarning", HeroAttachOnTricksterWarning, false);
        context.Transfer("HeroTalkToTrickster", HeroTalkToTrickster, false);
        context.Transfer("BuddyIntroComplete", BuddyIntroComplete, false);
        context.Transfer("TricksterApproachComplete", TricksterApproachComplete, false);
        context.Transfer("TricksterSpeechComplete", TricksterSpeechComplete, false);
        context.Transfer("TricksterRetreatComplete", TricksterRetreatComplete, false);
        context.Transfer("TricksterQuestComplete", TricksterQuestComplete, false);
        context.Transfer("TricksterDeadOrLeft", TricksterDeadOrLeft, false);
        context.Transfer("TricksterWhiningComplete", TricksterWhiningComplete, false);
        context.Transfer("TricksterComplete", TricksterComplete, false);
        context.Transfer("TricksterKilledBeforeAmbush", TricksterKilledBeforeAmbush, false);
        context.Transfer("TricksterKilledAfterAmbush", TricksterKilledAfterAmbush, false);
        context.Transfer("TricksterKilledAfterBeingSpared", TricksterKilledAfterBeingSpared, false);
        context.Transfer("TricksterResetAllegience", TricksterResetAllegience, false);
        context.Transfer("TricksterWaitHere", TricksterWaitHere, false);
        context.Transfer("TricksterApproachProximity", TricksterApproachProximity, 0.0f);
        context.Transfer("AmbushSpringComplete", AmbushSpringComplete, false);
        context.Transfer("DoTricksterIntro", DoTricksterIntro, false);
        context.Transfer("DoAmbush", DoAmbush, false);
        context.Transfer("HitCount", HitCount, 0L);
        context.Transfer("BuddyCounter", BuddyCounter, 0L);
    }
};
int main()
{
    CV_AmbushScamScript s;
    std::memset(&s, 0, sizeof(s));
    CPersistContext save; save.loading = false;
    g_log[0] = 0; g_count = 0;
    s.OnPersist(save);
    const char* expect = "QuestFinished:b;QuestComplete:b;QuestActivated:b;LeftRegion:b;MoralitySwitch:b;AllAmbushersDead:b;"
        "BuddyHelped:b;HeroAttackOnBuddy:b;HeroAttackOnTricksterAfterAmbush:b;HeroAttackOnTricksterBeforeAmbush:b;"
        "HeroAttackOnTricksterKillingBlow:b;HeroAttachOnTricksterWarning:b;HeroTalkToTrickster:b;BuddyIntroComplete:b;"
        "TricksterApproachComplete:b;TricksterSpeechComplete:b;TricksterRetreatComplete:b;TricksterQuestComplete:b;"
        "TricksterDeadOrLeft:b;TricksterWhiningComplete:b;TricksterComplete:b;TricksterKilledBeforeAmbush:b;"
        "TricksterKilledAfterAmbush:b;TricksterKilledAfterBeingSpared:b;TricksterResetAllegience:b;TricksterWaitHere:b;"
        "TricksterApproachProximity:f;AmbushSpringComplete:b;DoTricksterIntro:b;DoAmbush:b;HitCount:l;BuddyCounter:l;";
    bool ok = std::strcmp(g_log, expect) == 0 && g_count == 32;
    CPersistContext load; load.loading = true;
    s.OnPersist(load);
    ok = ok && s.QuestFinished && s.DoAmbush && s.TricksterWaitHere && s.HeroAttachOnTricksterWarning && s.BuddyIntroComplete;
    ok = ok && !s.ExpressionDetectedFollow && !s.ExpressionDetectedWait && !s.SpeakTime1 && !s.AllDone && !s.LeaderDead;
    ok = ok && s.TricksterApproachProximity > 2.4f && s.TricksterApproachProximity < 2.6f && s.HitCount == 8 && s.BuddyCounter == 12;
    if (!ok) { std::printf("got %s\n00e2ad50_TEST FAIL\n", g_log); return 1; }
    std::printf("00e2ad50_TEST PASS\n");
    return 0;
}