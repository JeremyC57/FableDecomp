// CSnowTrollDef::operator=(const CSnowTrollDef&) -- def copy: CDefClassBase part, then the CSnowTrollDef fields memberwise in declaration order
// Leaf: ??4CSnowTrollDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004541DF, 105 bytes, size-optimised def TU.
// Identity: reached through the 5-byte `jmp` thunk at 0x004541DA in slot 19 of vtable 0x01232A1C = CSnowTrollDef
//   (rtti-vtables.tsv); the manifest label "CThingBuildingDef::operator=" is propagated over every def assignment.
// Callees: CDefClassBase::operator= @ 0x00431F10.
// Layout (engine/CSnowTrollDef.h, matches retail): IcicleAttackIndex +0x28, SecondsIcicleAttackTargetPositionPredictionTime +0x2c, DistancePoundGroundRange +0x30, SecondsForIcicleAssault +0x34, SecondsLeadIntoIcicles +0x38, SecondsBetweenIcicles +0x3c, SecondsBetweenWarningFXAndTrigger +0x40, SecondsForTrapToRemain +0x44, DistanceRange +0x48, DistanceAroundTarget +0x4c, DistanceExclusionAroundIcicles +0x50, IcicleAssaultWarningFXIndex +0x54, IcicleAssaultTrapIndex +0x58, IceTrailDistanceLimit +0x5c.
// Shape: the implicit memberwise assignment written out (the def module out-lines it); the generated engine header
//   flattens the base (no base operator= to call), so the classes are modelled locally like the landed 0x00463BA2.
#pragma optimize("s", on)

class CDefClassBase { public:
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

class CSnowTrollDef : public CDefClassBase { public:
    long                     IcicleAttackIndex;           // +0x28
    float                    SecondsIcicleAttackTargetPositionPredictionTime; // +0x2c
    float                    DistancePoundGroundRange;    // +0x30
    float                    SecondsForIcicleAssault;     // +0x34
    float                    SecondsLeadIntoIcicles;      // +0x38
    float                    SecondsBetweenIcicles;       // +0x3c
    float                    SecondsBetweenWarningFXAndTrigger; // +0x40
    float                    SecondsForTrapToRemain;      // +0x44
    float                    DistanceRange;               // +0x48
    float                    DistanceAroundTarget;        // +0x4c
    float                    DistanceExclusionAroundIcicles; // +0x50
    long                     IcicleAssaultWarningFXIndex; // +0x54
    long                     IcicleAssaultTrapIndex;      // +0x58
    float                    IceTrailDistanceLimit;       // +0x5c
    CSnowTrollDef& operator=(const CSnowTrollDef& other);
};

CSnowTrollDef& CSnowTrollDef::operator=(const CSnowTrollDef& other)
{
    CDefClassBase::operator=(other);
    IcicleAttackIndex = other.IcicleAttackIndex;
    SecondsIcicleAttackTargetPositionPredictionTime = other.SecondsIcicleAttackTargetPositionPredictionTime;
    DistancePoundGroundRange = other.DistancePoundGroundRange;
    SecondsForIcicleAssault = other.SecondsForIcicleAssault;
    SecondsLeadIntoIcicles = other.SecondsLeadIntoIcicles;
    SecondsBetweenIcicles = other.SecondsBetweenIcicles;
    SecondsBetweenWarningFXAndTrigger = other.SecondsBetweenWarningFXAndTrigger;
    SecondsForTrapToRemain = other.SecondsForTrapToRemain;
    DistanceRange = other.DistanceRange;
    DistanceAroundTarget = other.DistanceAroundTarget;
    DistanceExclusionAroundIcicles = other.DistanceExclusionAroundIcicles;
    IcicleAssaultWarningFXIndex = other.IcicleAssaultWarningFXIndex;
    IcicleAssaultTrapIndex = other.IcicleAssaultTrapIndex;
    IceTrailDistanceLimit = other.IceTrailDistanceLimit;
    return *this;
}