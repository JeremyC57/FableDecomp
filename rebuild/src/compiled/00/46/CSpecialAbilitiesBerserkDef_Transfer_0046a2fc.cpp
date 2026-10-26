// CSpecialAbilitiesBerserkDef::Transfer(CPersistContext&) -- persist the berserk ability def's fields
// Leaf: ?Transfer@CSpecialAbilitiesBerserkDef@@UAEXAAVCPersistContext@@@Z
// Retail 0x0046A2FC, 620 bytes, size-optimised def TU (#pragma optimize("s")): unaligned address, no padding.
// One CPersistContext::Transfer<T> instantiation per member type:
//   0x00431061 Transfer(float&), 0x00431102 Transfer(long&), 0x0043314A Transfer(bool&),
//   0x00464CBF Transfer(vector<float>&), 0x00466A47 Transfer(vector<long>&).
// Layout: retail STLport vectors are 12 bytes (the PDB's are 16) and the CSpecialAbilitiesDef base ends at
//   +0x3C (PDB +0x40), so SpeedMultiplier sits at +0x3C and every later member below its PDB offset
//   (BerserkDuration +0x7C, LevelUpTimings +0x150, TintColorR/G/B +0x15C/+0x168/+0x174, size 0x188).
// Shape: the head floats in declaration order, then LevelUpTimings, the damage group with BerserkDuration
//   moved behind BerserkDamageToMeMultiplier, the camera/rumble vectors, DecapitateMaxProb ahead of the
//   two crazy-action floats, and the tail in declaration order (the def's TRANSFER list order).
#pragma optimize("s", on)

class CDefString { public: long m_index; };
template <class T> struct CStlVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };

class CPersistContext { public:
    template <class T> void Transfer(T& value);                // 0x00431061
    void Transfer(CStlVector<float>& v);    // 0x00464CBF
    void Transfer(CStlVector<long>& v);     // 0x00466A47
};

class CSpecialAbilitiesBerserkDef { public:
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