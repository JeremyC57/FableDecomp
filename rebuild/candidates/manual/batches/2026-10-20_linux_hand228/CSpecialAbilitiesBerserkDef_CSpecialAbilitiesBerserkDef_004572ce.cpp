// CSpecialAbilitiesBerserkDef::CSpecialAbilitiesBerserkDef() -- base CParentDefClassBase(), own vtable, then the nineteen retail vectors built empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004572CE, length 269 bytes.
// Callees: CParentDefClassBase::CParentDefClassBase @ 0x00430370. Vtable 0x012341FC = CSpecialAbilitiesBerserkDef.
// Layout: Ego_r.pdb names; the TLC def root ends at +0x3c (PDB 0x40) and retail STLport vectors are 12 bytes (PDB 16), so
// BerserkDuration +0x7c ... TintColorB +0x174, CanBlockWhileBerserked +0x180, CrazyIdleIntervalSecs +0x184 (0x188 bytes).
// Shape: the vtable store precedes the zero stores (the vectors are member objects with inline constructors); each vector's
// address is formed with `lea ecx,[esi+N]` and its three words are cleared from the shared eax zero. The plain floats/longs
// have no constructor. Needs the def module's `#pragma optimize("s", on)` (speed folds every address into disp32 stores). Local model: engine/CParentDefClassBase.h is a flat generated layout without the ctor/vtable.
#pragma optimize("s", on)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _base[0x38];                  // +0x04..+0x3b: the def root's own members
    CParentDefClassBase();                      // 0x00430370
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
    virtual void v00();                                 // own vtable 0x012341FC
    CSpecialAbilitiesBerserkDef();
};

CSpecialAbilitiesBerserkDef::CSpecialAbilitiesBerserkDef()
    : CParentDefClassBase()
{
}
