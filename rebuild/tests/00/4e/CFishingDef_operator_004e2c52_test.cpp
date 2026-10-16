// behaviour test for CFishingDef::operator= (0x004E2C52): every member copied, base assignment called, *this returned
#include <cstdio>
static int g_base = 0, g_ops = 0;
struct CDefClassBase { unsigned char _body[0x25]; CDefClassBase& operator=(const CDefClassBase& o) { ++g_base; for (int i = 0; i < 0x25; ++i) _body[i] = o._body[i]; return *this; } };
struct CLongVector { unsigned long m_words[3]; CLongVector& operator=(const CLongVector& o) { ++g_ops; for (int i = 0; i < 3; ++i) m_words[i] = o.m_words[i]; return *this; } };
struct CFishingDef : CDefClassBase {
    CLongVector FishTypes;
    float FishingSpotProximity;
    float FishingSpotMinDepth;
    float FishingSpotMinHeightDiff;
    float FishingSpotMaxHeightDiff;
    long PointsPerFishingLevel;
    float SlowFishingSpeed;
    float NormalFishingSpeed;
    float FastFishingSpeed;
    float FastestFishingSpeed;
    float FishWeightRainMult;
    float FishWeightSnowMult;
    float FishWeightMistMult;
    float BaseWaitPeriodSeconds;
    float MaxRandomWaitPeriodSeconds;
    float HookWindowSeconds;
    long BaseHookPresses;
    long MaxRandomHookPresses;
    float StrainIncreasePerPress;
    CFishingDef& operator=(const CFishingDef& other);
};
CFishingDef& CFishingDef::operator=(const CFishingDef& other)
{
    CDefClassBase::operator=(other);
    FishTypes = other.FishTypes;
    FishingSpotProximity = other.FishingSpotProximity;
    FishingSpotMinDepth = other.FishingSpotMinDepth;
    FishingSpotMinHeightDiff = other.FishingSpotMinHeightDiff;
    FishingSpotMaxHeightDiff = other.FishingSpotMaxHeightDiff;
    PointsPerFishingLevel = other.PointsPerFishingLevel;
    SlowFishingSpeed = other.SlowFishingSpeed;
    NormalFishingSpeed = other.NormalFishingSpeed;
    FastFishingSpeed = other.FastFishingSpeed;
    FastestFishingSpeed = other.FastestFishingSpeed;
    FishWeightRainMult = other.FishWeightRainMult;
    FishWeightSnowMult = other.FishWeightSnowMult;
    FishWeightMistMult = other.FishWeightMistMult;
    BaseWaitPeriodSeconds = other.BaseWaitPeriodSeconds;
    MaxRandomWaitPeriodSeconds = other.MaxRandomWaitPeriodSeconds;
    HookWindowSeconds = other.HookWindowSeconds;
    BaseHookPresses = other.BaseHookPresses;
    MaxRandomHookPresses = other.MaxRandomHookPresses;
    StrainIncreasePerPress = other.StrainIncreasePerPress;
    return *this;
}
int main()
{
    CFishingDef a, b;
    unsigned char* pa = (unsigned char*)&a; unsigned char* pb = (unsigned char*)&b;
    for (unsigned int i = 0; i < sizeof(a); ++i) { pa[i] = (unsigned char)(i * 7 + 3); pb[i] = 0; }
    CFishingDef* r = &(b = a);
    int bad = (r != &b) || g_base != 1;
    { const unsigned char* x = (const unsigned char*)&a.FishTypes; const unsigned char* y = (const unsigned char*)&b.FishTypes; for (unsigned int i = 0; i < sizeof(a.FishTypes); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.FishingSpotProximity; const unsigned char* y = (const unsigned char*)&b.FishingSpotProximity; for (unsigned int i = 0; i < sizeof(a.FishingSpotProximity); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.FishingSpotMinDepth; const unsigned char* y = (const unsigned char*)&b.FishingSpotMinDepth; for (unsigned int i = 0; i < sizeof(a.FishingSpotMinDepth); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.FishingSpotMinHeightDiff; const unsigned char* y = (const unsigned char*)&b.FishingSpotMinHeightDiff; for (unsigned int i = 0; i < sizeof(a.FishingSpotMinHeightDiff); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.FishingSpotMaxHeightDiff; const unsigned char* y = (const unsigned char*)&b.FishingSpotMaxHeightDiff; for (unsigned int i = 0; i < sizeof(a.FishingSpotMaxHeightDiff); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.PointsPerFishingLevel; const unsigned char* y = (const unsigned char*)&b.PointsPerFishingLevel; for (unsigned int i = 0; i < sizeof(a.PointsPerFishingLevel); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SlowFishingSpeed; const unsigned char* y = (const unsigned char*)&b.SlowFishingSpeed; for (unsigned int i = 0; i < sizeof(a.SlowFishingSpeed); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.NormalFishingSpeed; const unsigned char* y = (const unsigned char*)&b.NormalFishingSpeed; for (unsigned int i = 0; i < sizeof(a.NormalFishingSpeed); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.FastFishingSpeed; const unsigned char* y = (const unsigned char*)&b.FastFishingSpeed; for (unsigned int i = 0; i < sizeof(a.FastFishingSpeed); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.FastestFishingSpeed; const unsigned char* y = (const unsigned char*)&b.FastestFishingSpeed; for (unsigned int i = 0; i < sizeof(a.FastestFishingSpeed); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.FishWeightRainMult; const unsigned char* y = (const unsigned char*)&b.FishWeightRainMult; for (unsigned int i = 0; i < sizeof(a.FishWeightRainMult); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.FishWeightSnowMult; const unsigned char* y = (const unsigned char*)&b.FishWeightSnowMult; for (unsigned int i = 0; i < sizeof(a.FishWeightSnowMult); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.FishWeightMistMult; const unsigned char* y = (const unsigned char*)&b.FishWeightMistMult; for (unsigned int i = 0; i < sizeof(a.FishWeightMistMult); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.BaseWaitPeriodSeconds; const unsigned char* y = (const unsigned char*)&b.BaseWaitPeriodSeconds; for (unsigned int i = 0; i < sizeof(a.BaseWaitPeriodSeconds); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.MaxRandomWaitPeriodSeconds; const unsigned char* y = (const unsigned char*)&b.MaxRandomWaitPeriodSeconds; for (unsigned int i = 0; i < sizeof(a.MaxRandomWaitPeriodSeconds); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.HookWindowSeconds; const unsigned char* y = (const unsigned char*)&b.HookWindowSeconds; for (unsigned int i = 0; i < sizeof(a.HookWindowSeconds); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.BaseHookPresses; const unsigned char* y = (const unsigned char*)&b.BaseHookPresses; for (unsigned int i = 0; i < sizeof(a.BaseHookPresses); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.MaxRandomHookPresses; const unsigned char* y = (const unsigned char*)&b.MaxRandomHookPresses; for (unsigned int i = 0; i < sizeof(a.MaxRandomHookPresses); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.StrainIncreasePerPress; const unsigned char* y = (const unsigned char*)&b.StrainIncreasePerPress; for (unsigned int i = 0; i < sizeof(a.StrainIncreasePerPress); ++i) if (x[i] != y[i]) bad = 1; }
    for (int i = 0; i < 0x25; ++i) if (a._body[i] != b._body[i]) bad = 1;
    if (bad) { std::printf("004e2c52_TEST FAIL\n"); return 1; }
    std::printf("004e2c52_TEST PASS\n");
    return 0;
}