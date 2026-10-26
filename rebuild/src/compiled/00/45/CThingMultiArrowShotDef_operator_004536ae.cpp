// CThingMultiArrowShotDef::operator=(const CThingMultiArrowShotDef& other) -- the memberwise def assignment: the CDefClassBase part, then every CThingMultiArrowShotDef member in declaration order; returns *this
// Leaf: ??4CThingMultiArrowShotDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004536AE, 87 bytes, thiscall `ret 4`; size-optimised def TU.
// Identity (proven): CThingMultiArrowShotDef::operator=; evidence: its only reference is the 5-byte `jmp` at 0x004536A9 (the tail-calling virtual in slot 19 of
//   vtable 0x01232BBC, RTTI .?AVCThingMultiArrowShotDef@@; as the landed CHeroExperienceDef::operator= 0x004F914A), it copies exactly the CThingMultiArrowShotDef members
//   (offsets and sizes asserted against Ego_r.pdb, at their PDB offsets), and the PDB name sets hold ??4CThingMultiArrowShotDef@@QAEAAV0@ABV0@@Z.
// Label: the manifest's "CThingBuildingDef_CopyAssign_1" is propagated (family H055: the six H052 defs' assignments, each its own COMDAT).
// Callee: CDefClassBase::operator= 0x00431F10 (copies PDefManager +0x1C, GlobalIndex +0x20 and the three flag bits at +0x24).
// Layout: CDefClassBase is 0x25 bytes (pack(1) in the PDB, members from +0x28); local classes -- the generated engine headers flatten the
//   bases (no base operator= to call), Review at landing.
// Shape: the compiler-generated assignment written out in declaration order; float members copy through eax as integer moves.
#pragma optimize("s", on)
class CDefClassBase {                                         // 37 bytes in retail
public:
    unsigned char _body[0x25];
    CDefClassBase& operator=(const CDefClassBase& other);        // 0x00431F10
};
class CThingMultiArrowShotDef : public CDefClassBase {                       // vtable 0x01232BBC
public:
    float AccelerationScalar;                          // +0x28
    float MaxDistance;                                 // +0x2C
    float MaxAngle;                                    // +0x30
    float TimeToReachTarget;                           // +0x34
    float MaxAngleToTurnDegs;                          // +0x38
    long MultiArrowWeaponTrail;                        // +0x3C
    long MultiArrowHitEffect;                          // +0x40
    long MultiArrowTrailEffect;                        // +0x44
    float MultiArrowSpeed;                             // +0x48
    bool MultiArrowDeflectionByMelee;                  // +0x4C
    float MultiArrowHitEffectPercentage;               // +0x50
    CThingMultiArrowShotDef& operator=(const CThingMultiArrowShotDef& other);
};

CThingMultiArrowShotDef& CThingMultiArrowShotDef::operator=(const CThingMultiArrowShotDef& other)
{
    CDefClassBase::operator=(other);
    AccelerationScalar = other.AccelerationScalar;
    MaxDistance = other.MaxDistance;
    MaxAngle = other.MaxAngle;
    TimeToReachTarget = other.TimeToReachTarget;
    MaxAngleToTurnDegs = other.MaxAngleToTurnDegs;
    MultiArrowWeaponTrail = other.MultiArrowWeaponTrail;
    MultiArrowHitEffect = other.MultiArrowHitEffect;
    MultiArrowTrailEffect = other.MultiArrowTrailEffect;
    MultiArrowSpeed = other.MultiArrowSpeed;
    MultiArrowDeflectionByMelee = other.MultiArrowDeflectionByMelee;
    MultiArrowHitEffectPercentage = other.MultiArrowHitEffectPercentage;
    return *this;
}