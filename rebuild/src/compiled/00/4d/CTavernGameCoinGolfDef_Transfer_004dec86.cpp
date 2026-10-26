// CTavernGameCoinGolfDef::Transfer(CPersistContext& context) -- persist the CTavernGameCoinGolfDef's fields (13 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CTavernGameCoinGolfDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DEC86, 156 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CTavernGameCoinGolfDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x0123D4D4, whose RTTI type descriptor is .?AVCTavernGameCoinGolfDef@@ (bases CSubDefClassBase <- CDefClassBase); the Ego_r/FableWin.pdb name sets hold ?Transfer@CTavernGameCoinGolfDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CTavernGameCoinGolfDef@@UAEXAAVCPersistContext@@@Z" agrees with the RTTI (family J058: one-transfer-per-member def bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(float&) 0x00431061, Transfer(long&) 0x00431102.
// Layout: Ego_r.pdb CTavernGameCoinGolfDef members at their PDB offsets; every transferred offset hits the PDB member of the asserted type.
//   engine/CTavernGameCoinGolfDef.h is the flattened data-only view (vptr as a member, no virtual Transfer to define), so the class is local, on the real base chain.
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
    void Transfer(long& value);                                  // 0x00431102
};
class CTavernGameCoinGolfDef : public CSubDefClassBase {    // vtable 0x0123D4D4
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x0123D4D4 slot 18
    float TargetDistanceScale;                         // +0x28
    float JoystickScaleXboxX;                          // +0x2C
    float JoystickScaleXboxY;                          // +0x30
    float JoystickScaleX;                              // +0x34
    float JoystickScaleY;                              // +0x38
    float JoystickMagBelow;                            // +0x3C
    float AverageJoystickMagAbove;                     // +0x40
    float AverageDrift;                                // +0x44
    float MaxPower;                                    // +0x48
    float RestSpeed;                                   // +0x4C
    long Courses;                                      // +0x50
    float GoalRadius;                                  // +0x54
    long StartingCourse;                               // +0x58
};

void CTavernGameCoinGolfDef::Transfer(CPersistContext& context)
{
    context.Transfer(TargetDistanceScale);
    context.Transfer(JoystickScaleXboxX);
    context.Transfer(JoystickScaleXboxY);
    context.Transfer(JoystickScaleX);
    context.Transfer(JoystickScaleY);
    context.Transfer(JoystickMagBelow);
    context.Transfer(AverageJoystickMagAbove);
    context.Transfer(AverageDrift);
    context.Transfer(MaxPower);
    context.Transfer(RestSpeed);
    context.Transfer(Courses);
    context.Transfer(GoalRadius);
    context.Transfer(StartingCourse);
}