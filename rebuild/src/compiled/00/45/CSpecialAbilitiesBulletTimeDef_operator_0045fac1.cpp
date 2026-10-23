// CSpecialAbilitiesBulletTimeDef::operator=(const CSpecialAbilitiesBulletTimeDef&) -- implicit memberwise copy of the bullet-time def
// Leaf: ??4CSpecialAbilitiesBulletTimeDef
// Retail 0x0045FAC1, 628 bytes, size-optimised def TU (#pragma optimize("s")): unaligned, ebp frame.
// Callees: CParentDefClassBase::operator= 0x00431ED0 (base part), vector<float>::operator= 0x0045EF2D.
// Shape: this is the COMPILER-GENERATED operator=. Its tells: the fixed float arrays are copied by
//   counted fld/fstp loops (element-typed array copy, the long array by a mov loop with the counter in
//   [ebp-4]), the C3DVector colours by `movsd` x3 (trivial struct copy), the scalars as dwords through eax
//   and the seven screen-filter vectors through their out-of-line operator=. A hand-written copy would need
//   explicit loops; the class declares no operator= and a never-landed helper forces its emission.
// Layout: the CParentDefClassBase base is 0x39 retail bytes (12-byte vector map; the PDB says 0x3d), padded
//   to +0x3C; derived members as in the def's Transfer (0x00464DB5): retail vectors are 12 bytes.
#pragma optimize("s", on)
#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _defClassBase[0x35];
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)
struct CDefString { long m_index; };
struct C3DVector { float X, Y, Z; };
struct CFloatVector {
    float* m_first; float* m_last; float* m_end;
    CFloatVector& operator=(const CFloatVector& other);   // 0x0045EF2D
};
struct CSpecialAbilitiesBulletTimeDef : CParentDefClassBase {
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
// Emits the implicit operator= (auto_inline off keeps the helper from absorbing it).
#pragma auto_inline(off)
void CopyBulletTimeDef(CSpecialAbilitiesBulletTimeDef& to, const CSpecialAbilitiesBulletTimeDef& from)
{
    to = from;
}