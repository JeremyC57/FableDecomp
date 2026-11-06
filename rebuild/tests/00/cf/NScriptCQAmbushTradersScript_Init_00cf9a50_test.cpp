// Behaviour test for NScript::CQ_AmbushTradersScript::Init (0x00CF9A50): four quest regions, every flag cleared except SpyWanderReset,
// five traders and the 0x1f guard mask, the two speech timers armed from the def (bandit long, spy long), the ConvoyTrigger1 flag
// cleared, three regions' generators off, the master-data tallies cleared, the sentry speech tables reusing the spy NORMAL / RANDOM lines
// and seven boasts (ids 1, 3, 6, 11, 12, 13, 14; KILLALLGUARDS / KILLALLTRADERS flagged with extra 2 / 4, KILLNOONE extra 6).
#include <cstdio>
#include <cstring>
struct Boast { long Id; bool Flag; long Extra; };
struct Quest {
    int Regions, GeneratorsOff;
    bool SpyWanderReset, QuestFailed, EnableSpeechBandit, ConvoyTrigger1;
    long TraderAliveCount, GuardAliveCount, BanditTimer, SpyTimer, SpyHealthBarID;
    const char* SpySpeechNormal[5]; const char* SpySpeechSentryNormal[5];
    Boast Boasts[7]; int NBoasts;
    void AddBoast(long id, bool flag, long extra) { Boast b = { id, flag, extra }; Boasts[NBoasts++] = b; }
    void Init(long banditLong, long spyLong)
    {
        Regions = 4;
        QuestFailed = false; SpyWanderReset = true; EnableSpeechBandit = false;
        TraderAliveCount = 5; GuardAliveCount = 0x1f;
        BanditTimer = banditLong; SpyTimer = spyLong; SpyHealthBarID = 0;
        ConvoyTrigger1 = false;
        GeneratorsOff = 3;
        static const char* normal[5] = { "TEXT_QST_003_SPY_SPEECH_NORMAL_01", "TEXT_QST_003_SPY_SPEECH_NORMAL_02",
            "TEXT_QST_003_SPY_SPEECH_NORMAL_03", "TEXT_QST_003_SPY_SPEECH_NORMAL_04", "TEXT_QST_003_SPY_SPEECH_NORMAL_05" };
        for (int i = 0; i < 5; ++i) { SpySpeechNormal[i] = normal[i]; SpySpeechSentryNormal[i] = normal[i]; }
        AddBoast(1, false, 0); AddBoast(3, false, 0); AddBoast(6, false, 0); AddBoast(11, true, 2);
        AddBoast(12, true, 4); AddBoast(13, false, 6); AddBoast(14, false, 0);
    }
};
int main()
{
    Quest q; std::memset(&q, 0, sizeof(q));
    q.QuestFailed = true; q.ConvoyTrigger1 = true; q.SpyHealthBarID = 9;
    q.Init(30, 45);
    bool ok = q.Regions == 4 && q.GeneratorsOff == 3 && q.SpyWanderReset && !q.QuestFailed && !q.ConvoyTrigger1
        && q.TraderAliveCount == 5 && q.GuardAliveCount == 0x1f && q.BanditTimer == 30 && q.SpyTimer == 45 && q.SpyHealthBarID == 0
        && !std::strcmp(q.SpySpeechSentryNormal[4], "TEXT_QST_003_SPY_SPEECH_NORMAL_05") && q.SpySpeechSentryNormal[0] == q.SpySpeechNormal[0]
        && q.NBoasts == 7 && q.Boasts[3].Id == 11 && q.Boasts[3].Flag && q.Boasts[3].Extra == 2 && q.Boasts[5].Extra == 6
        && !q.Boasts[5].Flag && q.Boasts[6].Id == 14;
    std::printf(ok ? "00cf9a50_TEST PASS\n" : "00cf9a50_TEST FAIL\n");
    return ok ? 0 : 1;
}