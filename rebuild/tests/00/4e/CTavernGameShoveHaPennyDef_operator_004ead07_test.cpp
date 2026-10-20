#include <cstdio>
#include <cstring>
struct CTavernGameDef {
    unsigned char _body[0x1a4];                  // CDefClassBase head + the tavern-game text ids
    CTavernGameDef& operator=(const CTavernGameDef& other);   // 0x004EA6DD
};
struct CShoveHaPennyRegionDefVector {            // retail std::vector<CShoveHaPennyRegionDef> (12 bytes)
    unsigned long m_words[3];
    CShoveHaPennyRegionDefVector& operator=(const CShoveHaPennyRegionDefVector& other);   // 0x004EAE1E
};

struct CTavernGameShoveHaPennyDef : CTavernGameDef {
    CShoveHaPennyRegionDefVector Regions;        // +0x1a4
    long  BoardDefIndex;                         // +0x1b0
    long  ArrowDefIndex;                         // +0x1b4
    float PositionCoinSpeedXbox;                 // +0x1b8
    float PositionCoinSpeedPC;                   // +0x1bc
    float PositionCoinMinimum;                   // +0x1c0
    float PositionCoinMaximum;                   // +0x1c4
    float PositionCoinDepth;                     // +0x1c8
    float ImpulseScale;                          // +0x1cc
    float TargetDistanceScale;                   // +0x1d0
    float JoystickScaleXboxX;                    // +0x1d4
    float JoystickScaleXboxY;                    // +0x1d8
    float JoystickScalePCX;                      // +0x1dc
    float JoystickScalePCY;                      // +0x1e0
    float JoystickTranslateY;                    // +0x1e4
    float JoystickMagBelow;                      // +0x1e8
    float AverageJoystickMagAbove;               // +0x1ec
    float AverageDrift;                          // +0x1f0
    float MaxPower;                              // +0x1f4
    float RestSpeed;                             // +0x1f8
    long  Rounds;                                // +0x1fc
    CTavernGameShoveHaPennyDef& operator=(const CTavernGameShoveHaPennyDef& other);
};

CTavernGameShoveHaPennyDef& CTavernGameShoveHaPennyDef::operator=(const CTavernGameShoveHaPennyDef& other)
{
    CTavernGameDef::operator=(other);
    Regions = other.Regions;
    BoardDefIndex = other.BoardDefIndex;
    ArrowDefIndex = other.ArrowDefIndex;
    PositionCoinSpeedXbox = other.PositionCoinSpeedXbox;
    PositionCoinSpeedPC = other.PositionCoinSpeedPC;
    PositionCoinMinimum = other.PositionCoinMinimum;
    PositionCoinMaximum = other.PositionCoinMaximum;
    PositionCoinDepth = other.PositionCoinDepth;
    ImpulseScale = other.ImpulseScale;
    TargetDistanceScale = other.TargetDistanceScale;
    JoystickScaleXboxX = other.JoystickScaleXboxX;
    JoystickScaleXboxY = other.JoystickScaleXboxY;
    JoystickScalePCX = other.JoystickScalePCX;
    JoystickScalePCY = other.JoystickScalePCY;
    JoystickTranslateY = other.JoystickTranslateY;
    JoystickMagBelow = other.JoystickMagBelow;
    AverageJoystickMagAbove = other.AverageJoystickMagAbove;
    AverageDrift = other.AverageDrift;
    MaxPower = other.MaxPower;
    RestSpeed = other.RestSpeed;
    Rounds = other.Rounds;
    return *this;
}
static int g_baseCalls = 0, g_vecCalls = 0;
CTavernGameDef& CTavernGameDef::operator=(const CTavernGameDef& other)
{
    ++g_baseCalls; std::memcpy(_body, other._body, sizeof _body); return *this;
}
CShoveHaPennyRegionDefVector& CShoveHaPennyRegionDefVector::operator=(const CShoveHaPennyRegionDefVector& other)
{
    ++g_vecCalls; for (int i = 0; i < 3; ++i) m_words[i] = other.m_words[i]; return *this;
}
int main()
{
    CTavernGameShoveHaPennyDef a, b;
    std::memset(&a, 0, sizeof a); std::memset(&b, 0, sizeof b);
    for (int i = 0; i < 0x1a4; ++i) b._body[i] = (unsigned char)i;
    b.Regions.m_words[0] = 11; b.Regions.m_words[2] = 33;
    b.BoardDefIndex = 7; b.ArrowDefIndex = 8; b.PositionCoinSpeedXbox = 1.5f; b.ImpulseScale = 2.25f;
    b.JoystickMagBelow = -3.0f; b.RestSpeed = 0.125f; b.Rounds = 5;
    CTavernGameShoveHaPennyDef& r = (a = b);
    bool ok = &r == &a && g_baseCalls == 1 && g_vecCalls == 1 && a._body[0x1a3] == 0xa3 && a.Regions.m_words[2] == 33
        && a.BoardDefIndex == 7 && a.ArrowDefIndex == 8 && a.PositionCoinSpeedXbox == 1.5f && a.ImpulseScale == 2.25f
        && a.JoystickMagBelow == -3.0f && a.RestSpeed == 0.125f && a.Rounds == 5 && a.MaxPower == 0.0f;
    if (!ok) { std::printf("004ead07_TEST FAIL\n"); return 1; }
    std::printf("004ead07_TEST PASS\n");
    return 0;
}