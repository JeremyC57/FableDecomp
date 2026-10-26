// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
// CSpecialAbilitiesBulletTimeDef::CSpecialAbilitiesBulletTimeDef() -- construct the parent def base, install the vtable,
//   default the seven sound CDefStrings to -1 and the seven screen-filter vectors to empty
// Retail: Fable.exe (TLC, VC7.1) @ 0x00457537, 163 bytes. Base ctor CParentDefClassBase::CParentDefClassBase @ 0x00430370;
//   vtable 0x01234264 = CSpecialAbilitiesBulletTimeDef (rtti-vtables.tsv).
// Layout: PDB (Ego_r) names; the TLC def base is 0x3c bytes (PDB 0x40), so every retail offset is the PDB offset - 4.
//   Retail vectors are 12-byte STLport vectors. Local model: engine/CParentDefClassBase.h is a flat generated layout
//   without the virtual table or constructor needed here.
#pragma optimize("s", on)
class CParentDefClassBase { public:
    virtual void VFunc00();                             // polymorphic root; slot contents irrelevant here
    unsigned char _base[0x38];                          // +0x04..+0x3b
    CParentDefClassBase();                              // 0x00430370
};
class CDefString { public: long TablePos; CDefString() : TablePos(-1) {} };
template<class T> struct CRetailVector {                // STLport vector: begin/end/capacity
    T* m_first; T* m_last; T* m_end;
    CRetailVector() : m_first(0), m_last(0), m_end(0) {}
};
class C3DVector { public: float X, Y, Z; };
class CSpecialAbilitiesBulletTimeDef : public CParentDefClassBase { public:
    float SlowTimeDuration[4];                          // +0x3c
    long  BulletTimeSlowdownMultiplier[4];              // +0x4c
    float LevelUpTimings[3];                            // +0x5c
    float CamDist[4], CamHeight[4], CamFOV[4], CamSwitchTime[4]; // +0x68..+0xa7
    float CastCamDist, CastCamHeight, CastCamFOV, CastCamSwitchTime; // +0xa8..+0xb7
    CDefString ChargeSound;                             // +0xb8
    CDefString PowerUp1Sound, PowerUp2Sound, PowerUp3Sound; // +0xbc..+0xc4
    CDefString InitSound, LoopSound, EndSound;          // +0xc8..+0xd0
    C3DVector RadialBlurThresholdCol; float RadialBlurLength;          // +0xd4
    C3DVector RadialBlurIntensityCol; float RadialBlurFade, RadialBlurRange, RadialBlurTwist, RadialBlurFadeTimeSecs; // +0xe4
    C3DVector DamageRadialBlurThresholdCol; float DamageRadialBlurLength;    // +0x100
    C3DVector DamageRadialBlurIntensityCol; float DamageRadialBlurFade, DamageRadialBlurRange, DamageRadialBlurTwist, DamageRadialBlurFadeTimeSecs; // +0x110
    float HeroCombatSlowness;                           // +0x12c
    CRetailVector<float> ScreenFilterAlpha;             // +0x130
    CRetailVector<float> ScreenFilterSaturation;        // +0x13c
    CRetailVector<float> ScreenFilterContrast;          // +0x148
    CRetailVector<float> ScreenFilterBrightness;        // +0x154
    CRetailVector<float> ScreenFilterTintR;             // +0x160
    CRetailVector<float> ScreenFilterTintG;             // +0x16c
    CRetailVector<float> ScreenFilterTintB;             // +0x178
    float ScreenFilterFadeInSecs, ScreenFilterFadeOutSecs; // +0x184, +0x188
    CSpecialAbilitiesBulletTimeDef();
};
CSpecialAbilitiesBulletTimeDef::CSpecialAbilitiesBulletTimeDef() {}