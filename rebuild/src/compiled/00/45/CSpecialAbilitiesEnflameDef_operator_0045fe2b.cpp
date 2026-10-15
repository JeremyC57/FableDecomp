// CSpecialAbilitiesEnflameDef::operator=(const CSpecialAbilitiesEnflameDef&) -- memberwise copy: CParentDefClassBase part, the per-level camera distance/height/FOV/switch-time tables, level-up timings, damage, the radius-of-effect vector and the combat multiplier; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045FE2B, length 154 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (`push 4 ; pop edx` counts).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<float>::operator= @ 0x0045EF2D.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CSpecialAbilitiesEnflameDef's vtable 0x01234334 is the 5-byte `jmp 0x0045FE2B` right before this body;
// PDB members +0x40..+0xac sit at retail +0x3c..+0xa4 (retail base 0x39 bytes, vector 12 bytes).
// Shape: memberwise assignment in declaration order; each float array is the compiler's element loop
// (`fld [eax+ecx] ; fstp [ecx]` with eax = source - this, shared by all six loops).
#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

struct CFloatVector {                   // retail std::vector<float> (12 bytes)
    float* m_first;
    float* m_last;
    float* m_end;
    CFloatVector& operator=(const CFloatVector& other);   // 0x0045EF2D
};

struct CSpecialAbilitiesEnflameDef : CParentDefClassBase {  // vtable 0x01234334
    float CamDist[4];                           // +0x3c
    float CamHeight[4];                         // +0x4c
    float CamFOV[4];                            // +0x5c
    float CamSwitchTime[4];                     // +0x6c
    float LevelUpTimings[3];                    // +0x7c
    float Damage[4];                            // +0x88
    CFloatVector RadiusOfEffect;                // +0x98
    float EnflameCombatMultiplierFactor;        // +0xa4
    CSpecialAbilitiesEnflameDef& operator=(const CSpecialAbilitiesEnflameDef& other);
};

CSpecialAbilitiesEnflameDef& CSpecialAbilitiesEnflameDef::operator=(const CSpecialAbilitiesEnflameDef& other)
{
    CParentDefClassBase::operator=(other);
    long i;
    for (i = 0; i < 4; ++i) CamDist[i] = other.CamDist[i];
    for (i = 0; i < 4; ++i) CamHeight[i] = other.CamHeight[i];
    for (i = 0; i < 4; ++i) CamFOV[i] = other.CamFOV[i];
    for (i = 0; i < 4; ++i) CamSwitchTime[i] = other.CamSwitchTime[i];
    for (i = 0; i < 3; ++i) LevelUpTimings[i] = other.LevelUpTimings[i];
    for (i = 0; i < 4; ++i) Damage[i] = other.Damage[i];
    RadiusOfEffect = other.RadiusOfEffect;
    EnflameCombatMultiplierFactor = other.EnflameCombatMultiplierFactor;
    return *this;
}