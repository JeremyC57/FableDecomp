// CLightDef::Transfer(CPersistContext& context) -- persist the CLightDef's fields (15 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CLightDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DDCFA, 178 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CLightDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x0123A814, whose RTTI type descriptor is .?AVCLightDef@@; the Ego_r/FableWin.pdb name sets hold ?Transfer@CLightDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CLightDef@@UAEXAAVCPersistContext@@@Z" agrees; family H054 (one-transfer-per-member bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(CRGBColour&) 0x004595A3, Transfer(long&) 0x00431102, Transfer(float&) 0x00431061, Transfer(bool&) 0x0043314A, Transfer(EPointLightChannelEffect&) 0x004DDDAC.
// Layout: Ego_r.pdb CLightDef members at their PDB offsets; every transferred offset hits the PDB member of the asserted type.
//   engine/CLightDef.h exists but is the flattened data-only view (no virtual Transfer to define), so a local class is used -- Review at landing.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
class CRGBColour {                                                 // Ego_r.pdb, 4 bytes (opaque here)
public:
    unsigned char _body[4];
};
enum EPointLightChannelEffect { };                          // enumerators not needed
class CPersistContext {
public:
    void Transfer(CRGBColour& value);                            // 0x004595A3
    void Transfer(long& value);                                  // 0x00431102
    void Transfer(float& value);                                 // 0x00431061
    void Transfer(bool& value);                                  // 0x0043314A
    void Transfer(EPointLightChannelEffect& value);              // 0x004DDDAC
};
class CLightDef {
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x0123A814 slot 18
    unsigned char _base[0x28 - 4];                        // def base body
    CRGBColour Colour;                                 // +0x28
    long HaloGraphic;                                  // +0x2C
    float InnerRadius;                                 // +0x30
    float OuterRadius;                                 // +0x34
    float Flicker;                                     // +0x38
    float FlickerSpeed;                                // +0x3C
    bool StartActive;                                  // +0x40
    bool SunlightAttenuator;                           // +0x41
    bool DaylightAttenuator;                           // +0x42
    float DaylightAttenuatorFadeInStart;               // +0x44
    float DaylightAttenuatorFadeInEnd;                 // +0x48
    float DaylightAttenuatorFadeOutStart;              // +0x4C
    float DaylightAttenuatorFadeOutEnd;                // +0x50
    EPointLightChannelEffect EffectChannel;            // +0x54
    bool Inverted;                                     // +0x58
};

void CLightDef::Transfer(CPersistContext& context)
{
    context.Transfer(Colour);
    context.Transfer(HaloGraphic);
    context.Transfer(InnerRadius);
    context.Transfer(OuterRadius);
    context.Transfer(Flicker);
    context.Transfer(FlickerSpeed);
    context.Transfer(StartActive);
    context.Transfer(SunlightAttenuator);
    context.Transfer(DaylightAttenuator);
    context.Transfer(EffectChannel);
    context.Transfer(Inverted);
    context.Transfer(DaylightAttenuatorFadeInStart);
    context.Transfer(DaylightAttenuatorFadeInEnd);
    context.Transfer(DaylightAttenuatorFadeOutStart);
    context.Transfer(DaylightAttenuatorFadeOutEnd);
}