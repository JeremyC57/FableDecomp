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

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

// def vtable 0x01232A1C: slot 0 0x00450049 (implicit inline dtor); real overrides from the def vtable scan
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
    virtual void Transfer(CPersistContext& context);         // slot 18 0x00453014
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x004541DA
    virtual unsigned long GetSizeofClass();                  // slot 20 0x0044C669
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