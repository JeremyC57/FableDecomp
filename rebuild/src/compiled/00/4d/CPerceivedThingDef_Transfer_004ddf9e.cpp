// CPerceivedThingDef::Transfer(CPersistContext& context) -- persist the CPerceivedThingDef's fields (10 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CPerceivedThingDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DDF9E, 123 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CPerceivedThingDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x0123AA9C, whose RTTI type descriptor is .?AVCPerceivedThingDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CPerceivedThingDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CPerceivedThingDef@@UAEXAAVCPersistContext@@@Z" agrees; family H058 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(float&) 0x00431061, Transfer(EPerceivedThingType&) 0x004DE019.
// Layout: Ego_r.pdb CPerceivedThingDef members at their PDB offsets; every transferred offset hits the PDB member of the asserted type.
//   engine/CPerceivedThingDef.h exists but is the flattened data-only view (no virtual Transfer to define), so a local class is used -- Review at landing.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
enum EPerceivedThingType { };                               // enumerators not needed
class CPersistContext {
public:
    void Transfer(float& value);                                 // 0x00431061
    void Transfer(EPerceivedThingType& value);                   // 0x004DE019
};
class CPerceivedThingDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x0123AA9C slot 18
    unsigned char _base[0x28 - 4];                        // def base body
    float FieldOfView;                                 // +0x28
    float GiveUpChaseRadius;                           // +0x2C
    float SightRadius;                                 // +0x30
    float ExtendedSightRadius;                         // +0x34
    float SoundRadius;                                 // +0x38
    float SmellRadius;                                 // +0x3C
    EPerceivedThingType Type;                          // +0x40
    float AwarenessTimer;                              // +0x44
    float DayTimeVisibilityMultiplier;                 // +0x48
    float NightTimeVisibilityMultiplier;               // +0x4C
};

void CPerceivedThingDef::Transfer(CPersistContext& context)
{
    context.Transfer(FieldOfView);
    context.Transfer(ExtendedSightRadius);
    context.Transfer(SightRadius);
    context.Transfer(SoundRadius);
    context.Transfer(SmellRadius);
    context.Transfer(Type);
    context.Transfer(AwarenessTimer);
    context.Transfer(GiveUpChaseRadius);
    context.Transfer(DayTimeVisibilityMultiplier);
    context.Transfer(NightTimeVisibilityMultiplier);
}