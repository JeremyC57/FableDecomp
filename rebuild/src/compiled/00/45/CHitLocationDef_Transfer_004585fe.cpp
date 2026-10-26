// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
// CHitLocationDef::Transfer(CPersistContext&) -- serialize the hit-location definition fields in the retail persistence order
// Retail: Fable.exe (TLC, VC7.1) @ 0x004585FE, 237 bytes. Size-optimised def TU with a frame pointer.
// Callees (CPersistContext): Transfer(long&) 0x00431102, Transfer(bool&) 0x0043314A, and three member-template
//   instances emitted next to this function: Transfer<CPhysicalPrimitiveInit> 0x00458833 (8-byte local, forwards to
//   0x0067F7D0), Transfer<EActionRegisteredType> 0x004586EB and Transfer<EHitLocationCodelet> 0x0045878F (dword enums,
//   WriteLong 0x00993EB0). Manifest labels on 0x4586EB/0x45878F/0x43314A ("UpdateShadowScene") are propagated.
// Layout: PDB (Ego_r) names and types; the TLC def base is 0x3c bytes (PDB 0x40), so retail offset = PDB offset - 4.
//   Flags (EHitLocationFlags) goes through a long temporary, as retail copies +0x68 to a stack slot and back.
//   Local model: no engine/ header exists for CHitLocationDef or CPersistContext.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CPhysicalPrimitiveInit { public: unsigned long _data[2]; };
enum EActionRegisteredType { EActionRegisteredType_Force32 = 0x7fffffff };
enum EHitLocationFlags { EHitLocationFlags_Force32 = 0x7fffffff };
enum EHitLocationCodelet { EHitLocationCodelet_Force32 = 0x7fffffff };
class CPersistContext { public:
    void Transfer(long&);                               // 0x00431102
    void Transfer(bool&);                               // 0x0043314A
    template<class T> void Transfer(T&);                // instances 0x00458833 / 0x004586EB / 0x0045878F
};
class CHitLocationDef { public:
    virtual void VFunc00();                             // CDefClassBase vtable
    unsigned char _base[0x38];                          // +0x04..+0x3b
    CPhysicalPrimitiveInit PhysicalPrimitive;           // +0x3c
    long Armour;                                        // +0x44
    EActionRegisteredType HitResponseAction;            // +0x48
    EActionRegisteredType HitResponseToEpicSpellsAction; // +0x4c
    EActionRegisteredType HitKnockdownAction;           // +0x50
    EActionRegisteredType HitKnockdownDeathAction;      // +0x54
    EActionRegisteredType BlockResponseAction;          // +0x58
    long HitDamageEffect;                               // +0x5c
    long HitBlockedEffect;                              // +0x60
    long VulnerableEffect;                              // +0x64
    EHitLocationFlags Flags;                            // +0x68
    bool PlayGetHitResponse;                            // +0x6c
    bool DecapitateOnHit;                               // +0x6d
    bool DefendableAgainstShots;                        // +0x6e
    bool DefendableAgainstMelee;                        // +0x6f
    long Priority;                                      // +0x70
    bool Default;                                       // +0x74
    bool EnableOnCreate;                                // +0x75
    EHitLocationCodelet HitLocationCodelet;             // +0x78
    void Transfer(CPersistContext& context);
};
void CHitLocationDef::Transfer(CPersistContext& context) {
    context.Transfer(PhysicalPrimitive);
    context.Transfer(Armour);
    context.Transfer(HitDamageEffect);
    context.Transfer(HitBlockedEffect);
    context.Transfer(VulnerableEffect);
    context.Transfer(HitResponseAction);
    context.Transfer(HitResponseToEpicSpellsAction);
    context.Transfer(HitKnockdownAction);
    context.Transfer(HitKnockdownDeathAction);
    context.Transfer(BlockResponseAction);
    long flags = Flags;
    context.Transfer(flags);
    Flags = (EHitLocationFlags)flags;
    context.Transfer(DefendableAgainstShots);
    context.Transfer(DefendableAgainstMelee);
    context.Transfer(Priority);
    context.Transfer(Default);
    context.Transfer(DecapitateOnHit);
    context.Transfer(PlayGetHitResponse);
    context.Transfer(EnableOnCreate);
    context.Transfer(HitLocationCodelet);
}