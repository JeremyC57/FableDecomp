// CKrakenTentacleDef::Transfer(CPersistContext& context) -- persist the CKrakenTentacleDef's fields (14 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CKrakenTentacleDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DF050, 167 bytes, virtual thiscall `ret 4`; size-optimised def TU (every member address through eax; the entry is 16-aligned by chance).
// Identity (proven): CKrakenTentacleDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x0123DA24, whose RTTI type descriptor is .?AVCKrakenTentacleDef@@ (bases CSubDefClassBase <- CDefClassBase); the Ego_r/FableWin.pdb name sets hold ?Transfer@CKrakenTentacleDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CKrakenTentacleDef@@UAEXAAVCPersistContext@@@Z" agrees with the RTTI (family J057: one-transfer-per-member def bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(float&) 0x00431061.
// Layout: Ego_r.pdb CKrakenTentacleDef members at their PDB offsets; every transferred offset hits the PDB member of the asserted type.
//   engine/CKrakenTentacleDef.h is the flattened data-only view (vptr as a member, no virtual Transfer to define), so the class is local, on the real base chain.
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
    void Transfer(float& value);                                 // 0x00431061
};
class CKrakenTentacleDef : public CSubDefClassBase {    // vtable 0x0123DA24
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x0123DA24 slot 18
    float TentacleSmackDownTime;                       // +0x28
    float TentacleTurnSpeed;                           // +0x2C
    float TentacleMoveSpeed;                           // +0x30
    float TentacleStrikeAngleTolerence;                // +0x34
    float TentacleStrikeDistTolerence;                 // +0x38
    float TentacleHeightOffset;                        // +0x3C
    float TentacleTimeBeforeExplosion;                 // +0x40
    float TentacleTimeBetweenAttacks;                  // +0x44
    float TentacleTimeBeforeObstructionRemoval;        // +0x48
    float TentacleObstructionRadius;                   // +0x4C
    float TentacleExplosionBodgeAngle;                 // +0x50
    float TentacleExplosionRadius;                     // +0x54
    float TentacleObstructionRadiusMin;                // +0x58
    float TentacleObstructionRadiusMax;                // +0x5C
};

void CKrakenTentacleDef::Transfer(CPersistContext& context)
{
    context.Transfer(TentacleSmackDownTime);
    context.Transfer(TentacleTurnSpeed);
    context.Transfer(TentacleMoveSpeed);
    context.Transfer(TentacleStrikeAngleTolerence);
    context.Transfer(TentacleStrikeDistTolerence);
    context.Transfer(TentacleHeightOffset);
    context.Transfer(TentacleTimeBeforeExplosion);
    context.Transfer(TentacleTimeBetweenAttacks);
    context.Transfer(TentacleTimeBeforeObstructionRemoval);
    context.Transfer(TentacleObstructionRadius);
    context.Transfer(TentacleExplosionBodgeAngle);
    context.Transfer(TentacleExplosionRadius);
    context.Transfer(TentacleObstructionRadiusMin);
    context.Transfer(TentacleObstructionRadiusMax);
}