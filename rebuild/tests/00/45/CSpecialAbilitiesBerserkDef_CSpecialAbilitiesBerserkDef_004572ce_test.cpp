#include <cstdio>
#include <cstring>
#include <new>
static int g_baseCalls = 0;
struct CParentDefClassBase {
    virtual void v00() {}
    unsigned char _base[0x38];
    CParentDefClassBase() { ++g_baseCalls; }
};
template <class T>
struct CAllocProxy {                            // STLport _STLP_alloc_proxy: the end-of-storage pointer with its own ctor
    T* p;
    CAllocProxy(T* v) : p(v) { }
};
template <class T>
struct CRetailVector {                          // STLport _Vector_base: _M_start / _M_finish / _M_end_of_storage
    T* first;
    T* last;
    CAllocProxy<T> end;
    CRetailVector() : first(0), last(0), end(0) { }
};

struct CSpecialAbilitiesBerserkDef : CParentDefClassBase {
    float SpeedMultiplier;                              // +0x3c
    long  FramesToFadeOut;                              // +0x40
    float LowerBerserkPercent;                          // +0x44
    float TimeForThemeTransition;                       // +0x48
    float BlurCenterFromFloor;                          // +0x4c
    float BerserkThreshold;                             // +0x50
    float BerserkLength;                                // +0x54
    float BerserkIntensity;                             // +0x58
    float BerserkFade;                                  // +0x5c
    float BerserkRange;                                 // +0x60
    float BerserkTwist;                                 // +0x64
    float TimeForRadialBlurToFadeOut;                   // +0x68
    float CameraStartDist;                              // +0x6c
    float CameraEndDist;                                // +0x70
    float CameraStartDeltaHeight;                       // +0x74
    float CameraEndDeltaHeight;                         // +0x78
    CRetailVector<float> BerserkDuration;               // +0x7c
    CRetailVector<float> BerserkDamageToEnemyMultiplier;// +0x88
    float BerserkDamageToEnemyMultiplierAdditionPerHit; // +0x94
    CRetailVector<float> BerserkDamageToMeMultiplier;   // +0x98
    CRetailVector<float> MaxIncreaseInStrength;         // +0xa4
    CRetailVector<float> BerserkScale;                  // +0xb0
    float BerserkBlastRadius;                           // +0xbc
    CRetailVector<float> CamDist;                       // +0xc0
    CRetailVector<float> CamHeight;                     // +0xcc
    CRetailVector<float> CamFOV;                        // +0xd8
    CRetailVector<float> CamSwitchTime;                 // +0xe4
    CRetailVector<float> RumbleLevel;                   // +0xf0
    CRetailVector<float> RumbleSmoothness;              // +0xfc
    CRetailVector<float> RumbleAttack;                  // +0x108
    CRetailVector<float> RumbleDecay;                   // +0x114
    float RumbleReleaseLevelDelta;                      // +0x120
    float RumbleReleaseSmoothnessDelta;                 // +0x124
    float RumbleReleaseAttackDelta;                     // +0x128
    float RumbleReleaseDecayDelta;                      // +0x12c
    float CrazyActionRadius;                            // +0x130
    float CrazyActionDamage;                            // +0x134
    CRetailVector<float> DecapitateMaxProb;             // +0x138
    float TargetNearestEnemyDistance;                   // +0x144
    long  WeaponTrailAttack;                            // +0x148
    long  WeaponTrailKnockDown;                         // +0x14c
    CRetailVector<float> LevelUpTimings;                // +0x150
    CRetailVector<long>  TintColorR;                    // +0x15c
    CRetailVector<long>  TintColorG;                    // +0x168
    CRetailVector<long>  TintColorB;                    // +0x174
    bool  CanBlockWhileBerserked;                       // +0x180
    float CrazyIdleIntervalSecs;                        // +0x184
    virtual void v00() {}
    CSpecialAbilitiesBerserkDef();
};

CSpecialAbilitiesBerserkDef::CSpecialAbilitiesBerserkDef()
    : CParentDefClassBase()
{
}
template <class T> static int Empty(const CRetailVector<T>& v) { return v.first == 0 && v.last == 0 && v.end.p == 0; }
int main()
{
    static unsigned char buf[sizeof(CSpecialAbilitiesBerserkDef) + 16];
    std::memset(buf, 0xcd, sizeof(buf));
    CSpecialAbilitiesBerserkDef* d = new (buf) CSpecialAbilitiesBerserkDef();
    int ok = g_baseCalls == 1;
    ok = ok && Empty(d->BerserkDuration) && Empty(d->BerserkDamageToEnemyMultiplier) && Empty(d->BerserkDamageToMeMultiplier)
            && Empty(d->MaxIncreaseInStrength) && Empty(d->BerserkScale) && Empty(d->CamDist) && Empty(d->CamHeight)
            && Empty(d->CamFOV) && Empty(d->CamSwitchTime) && Empty(d->RumbleLevel) && Empty(d->RumbleSmoothness)
            && Empty(d->RumbleAttack) && Empty(d->RumbleDecay) && Empty(d->DecapitateMaxProb) && Empty(d->LevelUpTimings)
            && Empty(d->TintColorR) && Empty(d->TintColorG) && Empty(d->TintColorB);
    unsigned char raw[4]; std::memcpy(raw, &d->SpeedMultiplier, 4);
    ok = ok && raw[0] == 0xcd;                       // plain members stay uninitialised
    std::memcpy(raw, &d->WeaponTrailAttack, 4);
    ok = ok && raw[0] == 0xcd;
    if (!ok) { std::printf("004572ce_TEST FAIL\n"); return 1; }
    std::printf("004572ce_TEST PASS\n");
    return 0;
}