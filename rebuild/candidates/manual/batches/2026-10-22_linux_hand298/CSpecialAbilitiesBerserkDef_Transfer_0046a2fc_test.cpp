#include <cstdio>
// behaviour test: CSpecialAbilitiesBerserkDef::Transfer visits every member once, with retail's callee-type sequence


struct CDefString { long m_index; };
template <class T> struct CStlVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };

struct CPersistContext {
    const void* order[128]; int kind[128]; int count;
    void Transfer(CStlVector<float>& v) { kind[count] = 1; order[count++] = &v; }
    void Transfer(CStlVector<long>& v) { kind[count] = 2; order[count++] = &v; }
    void Transfer(bool& v) { kind[count] = 3; order[count++] = &v; }
    void Transfer(float& v) { kind[count] = 4; order[count++] = &v; }
    void Transfer(long& v) { kind[count] = 5; order[count++] = &v; }
};

struct CSpecialAbilitiesBerserkDef {
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x38];                                   // CSpecialAbilitiesDef base fields (+0x04)
    float SpeedMultiplier;                                       // +0x3C
    long FramesToFadeOut;                                        // +0x40
    float LowerBerserkPercent;                                   // +0x44
    float TimeForThemeTransition;                                // +0x48
    float BlurCenterFromFloor;                                   // +0x4C
    float BerserkThreshold;                                      // +0x50
    float BerserkLength;                                         // +0x54
    float BerserkIntensity;                                      // +0x58
    float BerserkFade;                                           // +0x5C
    float BerserkRange;                                          // +0x60
    float BerserkTwist;                                          // +0x64
    float TimeForRadialBlurToFadeOut;                            // +0x68
    float CameraStartDist;                                       // +0x6C
    float CameraEndDist;                                         // +0x70
    float CameraStartDeltaHeight;                                // +0x74
    float CameraEndDeltaHeight;                                  // +0x78
    CStlVector<float> BerserkDuration;                           // +0x7C
    CStlVector<float> BerserkDamageToEnemyMultiplier;            // +0x88
    float BerserkDamageToEnemyMultiplierAdditionPerHit;          // +0x94
    CStlVector<float> BerserkDamageToMeMultiplier;               // +0x98
    CStlVector<float> MaxIncreaseInStrength;                     // +0xA4
    CStlVector<float> BerserkScale;                              // +0xB0
    float BerserkBlastRadius;                                    // +0xBC
    CStlVector<float> CamDist;                                   // +0xC0
    CStlVector<float> CamHeight;                                 // +0xCC
    CStlVector<float> CamFOV;                                    // +0xD8
    CStlVector<float> CamSwitchTime;                             // +0xE4
    CStlVector<float> RumbleLevel;                               // +0xF0
    CStlVector<float> RumbleSmoothness;                          // +0xFC
    CStlVector<float> RumbleAttack;                              // +0x108
    CStlVector<float> RumbleDecay;                               // +0x114
    float RumbleReleaseLevelDelta;                               // +0x120
    float RumbleReleaseSmoothnessDelta;                          // +0x124
    float RumbleReleaseAttackDelta;                              // +0x128
    float RumbleReleaseDecayDelta;                               // +0x12C
    float CrazyActionRadius;                                     // +0x130
    float CrazyActionDamage;                                     // +0x134
    CStlVector<float> DecapitateMaxProb;                         // +0x138
    float TargetNearestEnemyDistance;                            // +0x144
    long WeaponTrailAttack;                                      // +0x148
    long WeaponTrailKnockDown;                                   // +0x14C
    CStlVector<float> LevelUpTimings;                            // +0x150
    CStlVector<long> TintColorR;                                 // +0x15C
    CStlVector<long> TintColorG;                                 // +0x168
    CStlVector<long> TintColorB;                                 // +0x174
    bool CanBlockWhileBerserked;                                 // +0x180
    float CrazyIdleIntervalSecs;                                 // +0x184
};

void CSpecialAbilitiesBerserkDef::Transfer(CPersistContext& context)
{
    context.Transfer(SpeedMultiplier);
    context.Transfer(FramesToFadeOut);
    context.Transfer(LowerBerserkPercent);
    context.Transfer(TimeForThemeTransition);
    context.Transfer(BlurCenterFromFloor);
    context.Transfer(BerserkThreshold);
    context.Transfer(BerserkLength);
    context.Transfer(BerserkIntensity);
    context.Transfer(BerserkFade);
    context.Transfer(BerserkRange);
    context.Transfer(BerserkTwist);
    context.Transfer(TimeForRadialBlurToFadeOut);
    context.Transfer(CameraStartDist);
    context.Transfer(CameraEndDist);
    context.Transfer(CameraStartDeltaHeight);
    context.Transfer(CameraEndDeltaHeight);
    context.Transfer(LevelUpTimings);
    context.Transfer(BerserkDamageToEnemyMultiplier);
    context.Transfer(BerserkDamageToEnemyMultiplierAdditionPerHit);
    context.Transfer(BerserkDamageToMeMultiplier);
    context.Transfer(BerserkDuration);
    context.Transfer(MaxIncreaseInStrength);
    context.Transfer(BerserkScale);
    context.Transfer(BerserkBlastRadius);
    context.Transfer(CamDist);
    context.Transfer(CamHeight);
    context.Transfer(CamFOV);
    context.Transfer(CamSwitchTime);
    context.Transfer(RumbleLevel);
    context.Transfer(RumbleSmoothness);
    context.Transfer(RumbleAttack);
    context.Transfer(RumbleDecay);
    context.Transfer(RumbleReleaseLevelDelta);
    context.Transfer(RumbleReleaseSmoothnessDelta);
    context.Transfer(RumbleReleaseAttackDelta);
    context.Transfer(RumbleReleaseDecayDelta);
    context.Transfer(DecapitateMaxProb);
    context.Transfer(CrazyActionRadius);
    context.Transfer(CrazyActionDamage);
    context.Transfer(TargetNearestEnemyDistance);
    context.Transfer(WeaponTrailAttack);
    context.Transfer(WeaponTrailKnockDown);
    context.Transfer(TintColorR);
    context.Transfer(TintColorG);
    context.Transfer(TintColorB);
    context.Transfer(CanBlockWhileBerserked);
    context.Transfer(CrazyIdleIntervalSecs);
}

int main()
{
    static CSpecialAbilitiesBerserkDef def;
    static CPersistContext ctx; ctx.count = 0;
    def.Transfer(ctx);
    static const int kinds[] = { 4,5,4,4,4,4,4,4,4,4,4,4,4,4,4,4,1,1,4,1,1,1,1,4,1,1,1,1,1,1,1,1,4,4,4,4,1,4,4,4,5,5,2,2,2,3,4 };
    const int n = sizeof(kinds) / sizeof(kinds[0]);
    bool ok = ctx.count == n;
    for (int i = 0; ok && i < n; ++i) {
        ok = ctx.kind[i] == kinds[i];
        const char* p = (const char*)ctx.order[i];
        if (p < (const char*)&def || p >= (const char*)&def + sizeof(def)) ok = false;
        for (int j = 0; ok && j < i; ++j) if (ctx.order[j] == ctx.order[i]) ok = false;
    }
    if (!ok) { std::printf("0046a2fc_TEST FAIL\n"); return 1; }
    std::printf("0046a2fc_TEST PASS\n");
    return 0;
}
