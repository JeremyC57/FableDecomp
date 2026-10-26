// CSnowTrollDef::Transfer(CPersistContext& context) -- persist the CSnowTrollDef's fields (14 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CSnowTrollDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00453014, 167 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CSnowTrollDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x01232A1C, whose RTTI type descriptor is .?AVCSnowTrollDef@@ (bases CSubDefClassBase <- CDefClassBase); the Ego_r/FableWin.pdb name sets hold ?Transfer@CSnowTrollDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CSnowTrollDef@@UAEXAAVCPersistContext@@@Z" agrees with the RTTI (family J057: one-transfer-per-member def bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(long&) 0x00431102, Transfer(float&) 0x00431061.
// Layout: Ego_r.pdb CSnowTrollDef members at their PDB offsets; every transferred offset hits the PDB member of the asserted type.
//   engine/CSnowTrollDef.h is the flattened data-only view (vptr as a member, no virtual Transfer to define), so the class is local, on the real base chain.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
class CResourceList;
class CResource {                                        // Ego_r.pdb 0x1C (CIVCountedPointeeBase: vptr, IVRefCount)
public:
    virtual ~CResource();                                // 0x009FC550
    unsigned long  IVRefCount;                           // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0C
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
};
class CDefPointeeBase : public CResource {               // vtable 0x01230BA0
public:
    virtual ~CDefPointeeBase() { }
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {           // vtable 0x01230BCC
public:
    class CSubDefInfo { public:
        long DefIndex;
        long OriginalParentDefIndex;
    };
    CDefinitionManager* PDefManager;                     // +0x1C
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
class CSubDefClassBase : public CDefClassBase {         // vtable 0x01231D54, Ego_r.pdb 0x25 (packed, no members of its own)
};
#pragma pack(pop)
class CPersistContext {
public:
    void Transfer(long& value);                                  // 0x00431102
    void Transfer(float& value);                                 // 0x00431061
};
class CSnowTrollDef : public CSubDefClassBase {    // vtable 0x01232A1C
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x01232A1C slot 18
    long IcicleAttackIndex;                            // +0x28
    float SecondsIcicleAttackTargetPositionPredictionTime; // +0x2C
    float DistancePoundGroundRange;                    // +0x30
    float SecondsForIcicleAssault;                     // +0x34
    float SecondsLeadIntoIcicles;                      // +0x38
    float SecondsBetweenIcicles;                       // +0x3C
    float SecondsBetweenWarningFXAndTrigger;           // +0x40
    float SecondsForTrapToRemain;                      // +0x44
    float DistanceRange;                               // +0x48
    float DistanceAroundTarget;                        // +0x4C
    float DistanceExclusionAroundIcicles;              // +0x50
    long IcicleAssaultWarningFXIndex;                  // +0x54
    long IcicleAssaultTrapIndex;                       // +0x58
    float IceTrailDistanceLimit;                       // +0x5C
};

void CSnowTrollDef::Transfer(CPersistContext& context)
{
    context.Transfer(IcicleAttackIndex);
    context.Transfer(SecondsIcicleAttackTargetPositionPredictionTime);
    context.Transfer(DistancePoundGroundRange);
    context.Transfer(SecondsForIcicleAssault);
    context.Transfer(SecondsLeadIntoIcicles);
    context.Transfer(SecondsBetweenIcicles);
    context.Transfer(SecondsBetweenWarningFXAndTrigger);
    context.Transfer(SecondsForTrapToRemain);
    context.Transfer(DistanceRange);
    context.Transfer(DistanceAroundTarget);
    context.Transfer(DistanceExclusionAroundIcicles);
    context.Transfer(IcicleAssaultWarningFXIndex);
    context.Transfer(IcicleAssaultTrapIndex);
    context.Transfer(IceTrailDistanceLimit);
}