// CGuardDef::Transfer(CPersistContext& context) -- persist the CGuardDef's fields (10 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CGuardDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DE926, 123 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CGuardDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x0123C75C, whose RTTI type descriptor is .?AVCGuardDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CGuardDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CGuardDef@@UAEXAAVCPersistContext@@@Z" agrees; family H058 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(long&) 0x00431102.
// Layout: Ego_r.pdb CGuardDef members at their PDB offsets; every transferred offset hits the PDB member of the asserted type.
//   engine/CGuardDef.h exists but is the flattened data-only view (no virtual Transfer to define), so a local class is used -- Review at landing.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
class CPersistContext {
public:
    void Transfer(long& value);                                  // 0x00431102
};
class CGuardDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x0123C75C slot 18
    unsigned char _base[0x28 - 4];                        // def base body
    long BribeCostWeaponOut;                           // +0x28
    long BribeCostTrespass;                            // +0x2C
    long BribeCostVandalism;                           // +0x30
    long BribeCostShoplifter;                          // +0x34
    long BribeCostPickPocket;                          // +0x38
    long BribeCostAssault;                             // +0x3C
    long BribeCostGuardAssault;                        // +0x40
    long BribeCostGBH;                                 // +0x44
    long BribeCostGuardGBH;                            // +0x48
    long BribeCostMurder;                              // +0x4C
};

void CGuardDef::Transfer(CPersistContext& context)
{
    context.Transfer(BribeCostWeaponOut);
    context.Transfer(BribeCostTrespass);
    context.Transfer(BribeCostVandalism);
    context.Transfer(BribeCostShoplifter);
    context.Transfer(BribeCostPickPocket);
    context.Transfer(BribeCostAssault);
    context.Transfer(BribeCostGuardAssault);
    context.Transfer(BribeCostGBH);
    context.Transfer(BribeCostGuardGBH);
    context.Transfer(BribeCostMurder);
}