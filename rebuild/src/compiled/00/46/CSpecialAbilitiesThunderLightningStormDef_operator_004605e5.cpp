// CSpecialAbilitiesThunderLightningStormDef::operator=(const CSpecialAbilitiesThunderLightningStormDef& other) -- the memberwise def assignment: the CParentDefClassBase part, then every CSpecialAbilitiesThunderLightningStormDef member in declaration order; returns *this
// Leaf: ??4CSpecialAbilitiesThunderLightningStormDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004605E5, 87 bytes, thiscall `ret 4`; size-optimised def TU.
// Identity (proven): CSpecialAbilitiesThunderLightningStormDef::operator=; evidence: its only reference is the 5-byte `jmp` at 0x004605E0 (the tail-calling virtual in slot 19 of
//   vtable 0x01234814, RTTI .?AVCSpecialAbilitiesThunderLightningStormDef@@; as the landed CHeroExperienceDef::operator= 0x004F914A), it copies exactly the CSpecialAbilitiesThunderLightningStormDef members
//   (offsets and sizes asserted against Ego_r.pdb, 4 bytes below the PDB: the retail parent-def base is 0x39 bytes), and the PDB name sets hold ??4CSpecialAbilitiesThunderLightningStormDef@@QAEAAV0@ABV0@@Z.
// Label: the manifest's "CParentDefClassBase::operator=" is propagated (family H055: the six H052 defs' assignments, each its own COMDAT).
// Callee: CParentDefClassBase::operator= 0x00431ED0 (CDefClassBase::operator=, then InstantiationName +0x25 and the SubDefInfoMap vector map +0x29).
// Layout: CParentDefClassBase is 0x39 bytes (pack(1) in the PDB, members from +0x3C); local classes -- the generated engine headers flatten the
//   bases (no base operator= to call), Review at landing.
// Shape: the compiler-generated assignment written out in declaration order; float members copy through eax as integer moves.
#pragma optimize("s", on)
class CDefString {                                         // Ego_r.pdb, 4 bytes
public:
    long TablePos;
};
class CParentDefClassBase {                                         // 57 bytes in retail
public:
    unsigned char _body[0x39];
    CParentDefClassBase& operator=(const CParentDefClassBase& other);        // 0x00431ED0
};
class CSpecialAbilitiesThunderLightningStormDef : public CParentDefClassBase {                       // vtable 0x01234814
public:
    long NumBeams;                                     // +0x3C (Ego_r +0x40)
    float StartRadius;                                 // +0x40 (Ego_r +0x44)
    float EndRadius;                                   // +0x44 (Ego_r +0x48)
    float SkyBeamStartTime;                            // +0x48 (Ego_r +0x4C)
    float MainBeamsStartTime;                          // +0x4C (Ego_r +0x50)
    float BeamRotationStartSpeed;                      // +0x50 (Ego_r +0x54)
    float BeamRotationEndSpeed;                        // +0x54 (Ego_r +0x58)
    float TotalLifetime;                               // +0x58 (Ego_r +0x5C)
    float Damage;                                      // +0x5C (Ego_r +0x60)
    CDefString InitSound;                              // +0x60 (Ego_r +0x64)
    CDefString LoopingSound;                           // +0x64 (Ego_r +0x68)
    CSpecialAbilitiesThunderLightningStormDef& operator=(const CSpecialAbilitiesThunderLightningStormDef& other);
};

CSpecialAbilitiesThunderLightningStormDef& CSpecialAbilitiesThunderLightningStormDef::operator=(const CSpecialAbilitiesThunderLightningStormDef& other)
{
    CParentDefClassBase::operator=(other);
    NumBeams = other.NumBeams;
    StartRadius = other.StartRadius;
    EndRadius = other.EndRadius;
    SkyBeamStartTime = other.SkyBeamStartTime;
    MainBeamsStartTime = other.MainBeamsStartTime;
    BeamRotationStartSpeed = other.BeamRotationStartSpeed;
    BeamRotationEndSpeed = other.BeamRotationEndSpeed;
    TotalLifetime = other.TotalLifetime;
    Damage = other.Damage;
    InitSound = other.InitSound;
    LoopingSound = other.LoopingSound;
    return *this;
}