// CHeroTitleDef::Transfer(CPersistContext& context) -- persist the CHeroTitleDef's fields (11 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CHeroTitleDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00452884, 134 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CHeroTitleDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x0123234C, whose RTTI type descriptor is .?AVCHeroTitleDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CHeroTitleDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CHeroTitleDef@@UAEXAAVCPersistContext@@@Z" agrees; family H052 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(EHeroTitle&) 0x0045290A, Transfer(long&) 0x00431102, Transfer(bool&) 0x0043314A.
// Layout: Ego_r.pdb CHeroTitleDef members at their PDB offsets; every transferred offset hits the PDB member of the asserted type.
//   engine/CHeroTitleDef.h exists but is the flattened data-only view (no virtual Transfer to define), so a local class is used -- Review at landing.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
enum EHeroTitle { };                                        // enumerators not needed
class CPersistContext {
public:
    void Transfer(EHeroTitle& value);                            // 0x0045290A
    void Transfer(long& value);                                  // 0x00431102
    void Transfer(bool& value);                                  // 0x0043314A
};
class CHeroTitleDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x0123234C slot 18
    unsigned char _base[0x28 - 4];                        // def base body
    EHeroTitle Title;                                  // +0x28
    long HeroTitleGuiTag;                              // +0x2C
    long HeroTitleGreetToHeroTag;                      // +0x30
    long HeroTitleCommentAtHeroTag;                    // +0x34
    long HeroTitleCommentToSelfTag;                    // +0x38
    long RequiredRenownLevel;                          // +0x3C
    long RequiredStrengthExpLevel;                     // +0x40
    long RequiredSkillExpLevel;                        // +0x44
    long RequiredWillExpLevel;                         // +0x48
    bool IsBuyable;                                    // +0x4C
    bool IsAutomatic;                                  // +0x4D
};

void CHeroTitleDef::Transfer(CPersistContext& context)
{
    context.Transfer(Title);
    context.Transfer(HeroTitleGuiTag);
    context.Transfer(HeroTitleGreetToHeroTag);
    context.Transfer(HeroTitleCommentAtHeroTag);
    context.Transfer(HeroTitleCommentToSelfTag);
    context.Transfer(RequiredRenownLevel);
    context.Transfer(RequiredStrengthExpLevel);
    context.Transfer(RequiredSkillExpLevel);
    context.Transfer(RequiredWillExpLevel);
    context.Transfer(IsBuyable);
    context.Transfer(IsAutomatic);
}