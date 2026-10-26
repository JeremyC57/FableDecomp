// CTattooDef::Transfer(CPersistContext& context) -- persist the CTattooDef's fields (15 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CTattooDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004529AE, 178 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CTattooDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x012323B4, whose RTTI type descriptor is .?AVCTattooDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CTattooDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CTattooDef@@UAEXAAVCPersistContext@@@Z" agrees; family H054 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(CCharString&) 0x00431143, Transfer(long&) 0x00431102, Transfer(ECompositeBlendType&) 0x00452A60, Transfer(bool&) 0x0043314A, Transfer(float&) 0x00431061.
// Layout: Ego_r.pdb CTattooDef members at their PDB offsets; every transferred offset hits the PDB member of the asserted type.
//   engine/CTattooDef.h exists but is the flattened data-only view (no virtual Transfer to define), so a local class is used -- Review at landing.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
class CCharString {                                        // 4 bytes: the shared string data pointer
public:
    void* PStringData;
};
enum ECompositeBlendType { };                               // enumerators not needed
class CPersistContext {
public:
    template <class T> void Transfer(T& value);                           // 0x00431143
    void Transfer(ECompositeBlendType& value);                   // 0x00452A60
};
class CTattooDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x012323B4 slot 18
    unsigned char _base[0x28 - 4];                        // def base body
    CCharString TattooName;                            // +0x28
    long TextureLayer;                                 // +0x2C
    long ReplacingTextureIndex;                        // +0x30
    long BankIndex;                                    // +0x34
    ECompositeBlendType Blend;                         // +0x38
    bool RemovesAllTattoos;                            // +0x3C
    bool Permanent;                                    // +0x3D
    long CoversBodyAreaFlags;                          // +0x40
    float VisibilityMultiplier;                        // +0x44
    long SpecificCoversBodyAreaFlags;                  // +0x48
    CCharString CustomFileName;                        // +0x4C
    CCharString CustomFileNameBase;                    // +0x50
    float Attractiveness;                              // +0x54
    float Scariness;                                   // +0x58
    float Goodstrength;                                // +0x5C
};

void CTattooDef::Transfer(CPersistContext& context)
{
    context.Transfer(TattooName);
    context.Transfer(TextureLayer);
    context.Transfer(ReplacingTextureIndex);
    context.Transfer(BankIndex);
    context.Transfer(Blend);
    context.Transfer(RemovesAllTattoos);
    context.Transfer(Permanent);
    context.Transfer(CoversBodyAreaFlags);
    context.Transfer(Scariness);
    context.Transfer(Attractiveness);
    context.Transfer(Goodstrength);
    context.Transfer(VisibilityMultiplier);
    context.Transfer(SpecificCoversBodyAreaFlags);
    context.Transfer(CustomFileName);
    context.Transfer(CustomFileNameBase);
}