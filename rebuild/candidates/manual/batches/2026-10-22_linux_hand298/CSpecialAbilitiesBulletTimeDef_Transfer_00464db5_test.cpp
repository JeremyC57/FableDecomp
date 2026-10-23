#include <cstdio>
// behaviour test: CSpecialAbilitiesBulletTimeDef::Transfer visits every member once, with retail's callee-type sequence


struct CDefString { long m_index; };
struct C3DVector { float X, Y, Z; };
template <class T> struct CStlVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };

struct CPersistContext {
    const void* order[128]; int kind[128]; int count;
    void Transfer(CDefString& v) { kind[count] = 1; order[count++] = &v; }
    void Transfer(CStlVector<float>& v) { kind[count] = 2; order[count++] = &v; }
    void Transfer(float& v) { kind[count] = 3; order[count++] = &v; }
    void Transfer(long& v) { kind[count] = 4; order[count++] = &v; }
};

struct CSpecialAbilitiesBulletTimeDef {
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x38];                       // CSpecialAbilitiesDef base fields (+0x04)
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
    CStlVector<float> ScreenFilterAlpha;             // +0x130
    CStlVector<float> ScreenFilterSaturation;        // +0x13C
    CStlVector<float> ScreenFilterContrast;          // +0x148
    CStlVector<float> ScreenFilterBrightness;        // +0x154
    CStlVector<float> ScreenFilterTintR;             // +0x160
    CStlVector<float> ScreenFilterTintG;             // +0x16C
    CStlVector<float> ScreenFilterTintB;             // +0x178
    float ScreenFilterFadeInSecs;                    // +0x184
    float ScreenFilterFadeOutSecs;                   // +0x188
};

void CSpecialAbilitiesBulletTimeDef::Transfer(CPersistContext& context)
{
    context.Transfer(CamDist[0]);       context.Transfer(CamDist[1]);
    context.Transfer(CamDist[2]);       context.Transfer(CamDist[3]);
    context.Transfer(CamHeight[0]);     context.Transfer(CamHeight[1]);
    context.Transfer(CamHeight[2]);     context.Transfer(CamHeight[3]);
    context.Transfer(CamFOV[0]);        context.Transfer(CamFOV[1]);
    context.Transfer(CamFOV[2]);        context.Transfer(CamFOV[3]);
    context.Transfer(CamSwitchTime[0]); context.Transfer(CamSwitchTime[1]);
    context.Transfer(CamSwitchTime[2]); context.Transfer(CamSwitchTime[3]);
    context.Transfer(SlowTimeDuration[0]); context.Transfer(SlowTimeDuration[1]);
    context.Transfer(SlowTimeDuration[2]); context.Transfer(SlowTimeDuration[3]);
    context.Transfer(BulletTimeSlowdownMultiplier[0]); context.Transfer(BulletTimeSlowdownMultiplier[1]);
    context.Transfer(BulletTimeSlowdownMultiplier[2]); context.Transfer(BulletTimeSlowdownMultiplier[3]);
    context.Transfer(LevelUpTimings[0]); context.Transfer(LevelUpTimings[1]); context.Transfer(LevelUpTimings[2]);
    context.Transfer(CastCamDist);
    context.Transfer(CastCamHeight);
    context.Transfer(CastCamFOV);
    context.Transfer(CastCamSwitchTime);
    context.Transfer(ChargeSound);
    context.Transfer(PowerUp1Sound);
    context.Transfer(PowerUp2Sound);
    context.Transfer(PowerUp3Sound);
    context.Transfer(InitSound);
    context.Transfer(LoopSound);
    context.Transfer(EndSound);
    context.Transfer(RadialBlurThresholdCol.X); context.Transfer(RadialBlurThresholdCol.Y);
    context.Transfer(RadialBlurThresholdCol.Z);
    context.Transfer(RadialBlurLength);
    context.Transfer(RadialBlurIntensityCol.X); context.Transfer(RadialBlurIntensityCol.Y);
    context.Transfer(RadialBlurIntensityCol.Z);
    context.Transfer(RadialBlurFade);
    context.Transfer(RadialBlurRange);
    context.Transfer(RadialBlurTwist);
    context.Transfer(RadialBlurFadeTimeSecs);
    context.Transfer(DamageRadialBlurThresholdCol.X); context.Transfer(DamageRadialBlurThresholdCol.Y);
    context.Transfer(DamageRadialBlurThresholdCol.Z);
    context.Transfer(DamageRadialBlurLength);
    context.Transfer(DamageRadialBlurIntensityCol.X); context.Transfer(DamageRadialBlurIntensityCol.Y);
    context.Transfer(DamageRadialBlurIntensityCol.Z);
    context.Transfer(DamageRadialBlurFade);
    context.Transfer(DamageRadialBlurRange);
    context.Transfer(DamageRadialBlurTwist);
    context.Transfer(DamageRadialBlurFadeTimeSecs);
    context.Transfer(HeroCombatSlowness);
    context.Transfer(ScreenFilterAlpha);
    context.Transfer(ScreenFilterSaturation);
    context.Transfer(ScreenFilterContrast);
    context.Transfer(ScreenFilterBrightness);
    context.Transfer(ScreenFilterTintR);
    context.Transfer(ScreenFilterTintG);
    context.Transfer(ScreenFilterTintB);
    context.Transfer(ScreenFilterFadeOutSecs);
    context.Transfer(ScreenFilterFadeInSecs);
}

int main()
{
    static CSpecialAbilitiesBulletTimeDef def;
    static CPersistContext ctx; ctx.count = 0;
    def.Transfer(ctx);
    static const int kinds[] = { 3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,3,3,3,3,3,3,3,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,3,3 };
    const int n = sizeof(kinds) / sizeof(kinds[0]);
    bool ok = ctx.count == n;
    for (int i = 0; ok && i < n; ++i) {
        ok = ctx.kind[i] == kinds[i];
        const char* p = (const char*)ctx.order[i];
        if (p < (const char*)&def || p >= (const char*)&def + sizeof(def)) ok = false;
        for (int j = 0; ok && j < i; ++j) if (ctx.order[j] == ctx.order[i]) ok = false;
    }
    if (!ok) { std::printf("00464db5_TEST FAIL\n"); return 1; }
    std::printf("00464db5_TEST PASS\n");
    return 0;
}
