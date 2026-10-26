#include <cstdio>
// behaviour test: the implicit CSpecialAbilitiesBulletTimeDef::operator= copies base, arrays, colours, vectors and scalars


#pragma pack(push, 1)
class CParentDefClassBase { public:
    virtual void v00() {}
    unsigned char _defClassBase[0x35];
    CParentDefClassBase& operator=(const CParentDefClassBase& other) { for (int i = 0; i < 0x35; ++i) _defClassBase[i] = other._defClassBase[i]; return *this; }
};
#pragma pack(pop)
class CDefString { public: long m_index; };
class C3DVector { public: float X, Y, Z; };
struct CFloatVector {
    float* m_first; float* m_last; float* m_end;
    CFloatVector& operator=(const CFloatVector& other) { m_first = other.m_first; m_last = other.m_last; m_end = other.m_end; return *this; }
};
class CSpecialAbilitiesBulletTimeDef : public CParentDefClassBase { public:
    float SlowTimeDuration[4];                       // +0x3C
    long BulletTimeSlowdownMultiplier[4];            // +0x4C
    float LevelUpTimings[3];                         // +0x5C
    float CamDist[4];                                // +0x68
    float CamHeight[4];                              // +0x78
    float CamFOV[4];                                 // +0x88
    float CamSwitchTime[4];                          // +0x98
    float CastCamDist;                               // +0xA8
    float CastCamHeight;                             // +0xAC
    float CastCamFOV;                                // +0xB0
    float CastCamSwitchTime;                         // +0xB4
    CDefString ChargeSound;                          // +0xB8
    CDefString PowerUp1Sound;                        // +0xBC
    CDefString PowerUp2Sound;                        // +0xC0
    CDefString PowerUp3Sound;                        // +0xC4
    CDefString InitSound;                            // +0xC8
    CDefString LoopSound;                            // +0xCC
    CDefString EndSound;                             // +0xD0
    C3DVector RadialBlurThresholdCol;                // +0xD4
    float RadialBlurLength;                          // +0xE0
    C3DVector RadialBlurIntensityCol;                // +0xE4
    float RadialBlurFade;                            // +0xF0
    float RadialBlurRange;                           // +0xF4
    float RadialBlurTwist;                           // +0xF8
    float RadialBlurFadeTimeSecs;                    // +0xFC
    C3DVector DamageRadialBlurThresholdCol;          // +0x100
    float DamageRadialBlurLength;                    // +0x10C
    C3DVector DamageRadialBlurIntensityCol;          // +0x110
    float DamageRadialBlurFade;                      // +0x11C
    float DamageRadialBlurRange;                     // +0x120
    float DamageRadialBlurTwist;                     // +0x124
    float DamageRadialBlurFadeTimeSecs;              // +0x128
    float HeroCombatSlowness;                        // +0x12C
    CFloatVector ScreenFilterAlpha;                  // +0x130
    CFloatVector ScreenFilterSaturation;             // +0x13C
    CFloatVector ScreenFilterContrast;               // +0x148
    CFloatVector ScreenFilterBrightness;             // +0x154
    CFloatVector ScreenFilterTintR;                  // +0x160
    CFloatVector ScreenFilterTintG;                  // +0x16C
    CFloatVector ScreenFilterTintB;                  // +0x178
    float ScreenFilterFadeInSecs;                    // +0x184
    float ScreenFilterFadeOutSecs;                   // +0x188
};

void CopyBulletTimeDef(CSpecialAbilitiesBulletTimeDef& to, const CSpecialAbilitiesBulletTimeDef& from)
{
    to = from;
}

int main()
{
    static CSpecialAbilitiesBulletTimeDef a, b;
    for (int i = 0; i < 0x35; ++i) b._defClassBase[i] = (unsigned char)(i + 1);
    for (int i = 0; i < 4; ++i) { b.SlowTimeDuration[i] = 1.5f + i; b.BulletTimeSlowdownMultiplier[i] = 10 + i; b.CamDist[i] = 2.0f * i; b.CamHeight[i] = 3.0f * i; b.CamFOV[i] = 4.0f * i; b.CamSwitchTime[i] = 5.0f * i + 1; }
    for (int i = 0; i < 3; ++i) b.LevelUpTimings[i] = 7.0f + i;
    b.CastCamDist = 1; b.CastCamHeight = 2; b.CastCamFOV = 3; b.CastCamSwitchTime = 4;
    b.ChargeSound.m_index = 5; b.EndSound.m_index = 11;
    b.RadialBlurThresholdCol.X = 0.25f; b.RadialBlurThresholdCol.Z = 0.75f; b.DamageRadialBlurIntensityCol.Y = 9.5f;
    b.HeroCombatSlowness = 0.125f; b.ScreenFilterTintB.m_last = (float*)&b; b.ScreenFilterFadeInSecs = 6; b.ScreenFilterFadeOutSecs = 8;
    CSpecialAbilitiesBulletTimeDef& r = (a = b);
    bool ok = &r == &a;
    for (int i = 0; ok && i < 0x35; ++i) ok = a._defClassBase[i] == i + 1;
    for (int i = 0; ok && i < 4; ++i) ok = a.SlowTimeDuration[i] == 1.5f + i && a.BulletTimeSlowdownMultiplier[i] == 10 + i && a.CamSwitchTime[i] == 5.0f * i + 1 && a.CamFOV[i] == 4.0f * i;
    ok = ok && a.LevelUpTimings[2] == 9.0f && a.CastCamSwitchTime == 4 && a.ChargeSound.m_index == 5 && a.EndSound.m_index == 11;
    ok = ok && a.RadialBlurThresholdCol.X == 0.25f && a.RadialBlurThresholdCol.Z == 0.75f && a.DamageRadialBlurIntensityCol.Y == 9.5f;
    ok = ok && a.HeroCombatSlowness == 0.125f && a.ScreenFilterTintB.m_last == (float*)&b && a.ScreenFilterFadeInSecs == 6 && a.ScreenFilterFadeOutSecs == 8;
    if (!ok) { std::printf("0045fac1_TEST FAIL\n"); return 1; }
    std::printf("0045fac1_TEST PASS\n");
    return 0;
}