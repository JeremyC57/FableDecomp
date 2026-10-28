#include <cstdio>
#include <cstring>
class CCharString { public:
    char m_text[16];
    const CCharString& operator=(const CCharString& other) { std::strcpy(m_text, other.m_text); return *this; }
};
struct TGenerationParameters {
    float GenerationRadius; float SelfTriggerRadius; bool SelfTrigger; long SelfTriggerResetInterval;
    bool TriggerOnActivate; long ActiveCreatureLimit; long TotalGenerationLimit; long NumTriggers;
    CCharString ScriptNameOfAllGeneratedCreatures;
};
class CTCCreatureGenerator { public:
    TGenerationParameters GenParams;
    bool Active; bool Initialised; bool ReadyToInitialise; bool ExternalTrigger; bool WaitForAllCreaturesDead;
    long NumTriggersLeft;
    void CloneFromCreator(const CTCCreatureGenerator* creator)
    {
        GenParams = creator->GenParams;
        Active = GenParams.SelfTrigger;
        NumTriggersLeft = GenParams.NumTriggers;
        ReadyToInitialise = false;
    }
};
int main()
{
    CTCCreatureGenerator src, dst;
    src.GenParams.GenerationRadius = 4.5f; src.GenParams.SelfTriggerRadius = 9.0f; src.GenParams.SelfTrigger = true;
    src.GenParams.SelfTriggerResetInterval = 30; src.GenParams.TriggerOnActivate = false;
    src.GenParams.ActiveCreatureLimit = 3; src.GenParams.TotalGenerationLimit = 10; src.GenParams.NumTriggers = 6;
    std::strcpy(src.GenParams.ScriptNameOfAllGeneratedCreatures.m_text, "BANDIT");
    dst.Active = false; dst.Initialised = true; dst.ReadyToInitialise = true; dst.NumTriggersLeft = 0;
    dst.CloneFromCreator(&src);
    if (dst.GenParams.GenerationRadius != 4.5f || dst.GenParams.SelfTriggerRadius != 9.0f || dst.GenParams.SelfTriggerResetInterval != 30
        || dst.GenParams.ActiveCreatureLimit != 3 || dst.GenParams.TotalGenerationLimit != 10
        || std::strcmp(dst.GenParams.ScriptNameOfAllGeneratedCreatures.m_text, "BANDIT") != 0
        || !dst.Active || !dst.Initialised || dst.ReadyToInitialise || dst.NumTriggersLeft != 6)
    { std::printf("00749e40_TEST FAIL\n"); return 1; }
    src.GenParams.SelfTrigger = false; dst.CloneFromCreator(&src);
    if (dst.Active) { std::printf("00749e40_TEST FAIL\n"); return 1; }
    std::printf("00749e40_TEST PASS\n");
    return 0;
}