// Behaviour test for 0x00E4F620 NScript::CV_BeardyBaldyScript::Init: every flag cleared, QuestPhase 0 with InitialisePhase
// set, the required-hair names emptied, the counters zeroed and the ten random-speech keys TEXT_QST_014_RANDOM_00 .. _90.
#include <cstdio>
#include <cstring>

class CCharString { public:
    char Text[32];
    const CCharString& operator=(const char* t) { std::strcpy(Text, t); return *this; }
};
struct CV_BeardyBaldyScript {
    bool QuestFinished, QuestComplete, QuestActivated; long QuestPhase;
    bool InitialisePhase, IntroComplete, Phase1RequirementsComplete, Phase2RequirementsComplete, Phase3RequirementsComplete;
    CCharString RequiredHairdo, RequiredBeard, RequiredTash;
    bool AcquiredNewHairdo, AcquiredNewBeard, AcquiredNewTash, AllHairChanged, BeardyBaldyKilled, BarberLadyKilled, BeardyBaldyLeft;
    long IncorrectHairComboCount; bool AttackedByHero; long RandomSpeechTimer; CCharString RandomSpeech[10]; int BeardyBaldyPhase;
    void Init()
    {
        QuestFinished = false;
        QuestActivated = false;
        QuestComplete = false;
        QuestPhase = 0;
        InitialisePhase = true;
        IntroComplete = false;
        Phase1RequirementsComplete = false;
        Phase2RequirementsComplete = false;
        Phase3RequirementsComplete = false;
        RequiredHairdo = "";
        RequiredBeard = "";
        RequiredTash = "";
        BeardyBaldyKilled = false;
        BarberLadyKilled = false;
        BeardyBaldyLeft = false;
        AcquiredNewHairdo = false;
        AcquiredNewBeard = false;
        AcquiredNewTash = false;
        AllHairChanged = false;
        IncorrectHairComboCount = 0;
        AttackedByHero = false;
        RandomSpeech[0] = "TEXT_QST_014_RANDOM_00";
        RandomSpeech[1] = "TEXT_QST_014_RANDOM_10";
        RandomSpeech[2] = "TEXT_QST_014_RANDOM_20";
        RandomSpeech[3] = "TEXT_QST_014_RANDOM_30";
        RandomSpeech[4] = "TEXT_QST_014_RANDOM_40";
        RandomSpeech[5] = "TEXT_QST_014_RANDOM_50";
        RandomSpeech[6] = "TEXT_QST_014_RANDOM_60";
        RandomSpeech[7] = "TEXT_QST_014_RANDOM_70";
        RandomSpeech[8] = "TEXT_QST_014_RANDOM_80";
        RandomSpeech[9] = "TEXT_QST_014_RANDOM_90";
    }
};

int main()
{
    CV_BeardyBaldyScript s;
    std::memset(&s, 1, sizeof(s));
    s.RandomSpeechTimer = 77; s.BeardyBaldyPhase = 3;
    s.Init();
    int ok = !s.QuestFinished && !s.QuestComplete && !s.QuestActivated && s.QuestPhase == 0 && s.InitialisePhase
             && !s.IntroComplete && !s.Phase1RequirementsComplete && !s.Phase2RequirementsComplete && !s.Phase3RequirementsComplete
             && !s.RequiredHairdo.Text[0] && !s.RequiredBeard.Text[0] && !s.RequiredTash.Text[0]
             && !s.AcquiredNewHairdo && !s.AcquiredNewBeard && !s.AcquiredNewTash && !s.AllHairChanged
             && !s.BeardyBaldyKilled && !s.BarberLadyKilled && !s.BeardyBaldyLeft && s.IncorrectHairComboCount == 0
             && !s.AttackedByHero && s.RandomSpeechTimer == 77 && s.BeardyBaldyPhase == 3;
    for (int i = 0; i < 10; ++i) {
        char want[32]; std::sprintf(want, "TEXT_QST_014_RANDOM_%d0", i);
        if (std::strcmp(s.RandomSpeech[i].Text, want)) ok = 0;
    }
    if (!ok) { std::printf("00e4f620_TEST FAIL\n"); return 1; }
    std::printf("00e4f620_TEST PASS\n");
    return 0;
}