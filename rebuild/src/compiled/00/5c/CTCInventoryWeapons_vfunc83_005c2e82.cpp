// CTCInventoryWeapons::vfunc_83(CThing*) -- slot 83: a thing that is one of the active weapons (or matches a confiscated weapon def) is put in limbo; any other thing goes to the base inventory's slot-83 handling
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C2E82, 89 bytes. rtti-vtables: CTCInventoryWeapons 0x01253FF4 slot 83.
// Callees: the weapons screen's static `__fastcall` "is this the pointed-to thing" test @ 0x005CBDFA (ecx = thing, edx =
//          the CIntelligentPointer; `thing == pointer->GetPItem()`; manifest label "CVillageTaskCarryCrate::
//          IsCreatureAssignedToTask" is propagated), CThing::SetInLimbo(bool) @ 0x004C8CF0, the base slot-83 handler
//          @ 0x005BF4ED (CTCInventoryClothing's slot 83; label "CTCInventoryStats::HandleMinigameCompletion" propagated).
//
// Shape: a four-way `||` -- two helper calls (`lea edx,[esi+134h]` / `lea edx,[esi+141h]`, `mov ecx,edi`), then the
// thing's def index read once (`movzx eax,word ptr [edi+8Ch]`) and compared with both confiscated indices -- whose
// all-false fall-through is the base call (`call ; jmp out`) and whose true exit is `push 1 ; mov ecx,edi ; call
// SetInLimbo`. Size TU (`#pragma optimize("s", on)`). Members from the PDB CTCInventoryWeapons (PActiveMeleeWeapon +0x14C,
// MeleeWeaponCarriedOnLoad +0x158, PActiveRangedWeapon +0x159, Confiscated* +0x17C/+0x180) sit 0x18 / 0x1C lower in
// retail; PActiveRangedWeapon really is unaligned (+0x141), so the 8-byte CIntelligentPointer is modelled as a
// byte-aligned blob. The real method name is unknown. Local model (no engine header for CTCInventoryWeapons).
#pragma optimize("s", on)
class CThing { public:
    unsigned char _pad_0x00[0x8c];
    unsigned short DefIndex;                  // +0x8C
    void SetInLimbo(bool inLimbo);            // 0x004C8CF0
};
struct CIntelligentPointer {
    unsigned char _body[8];
};
struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char _pad_0x04[0x130 - 4];
    void vfunc_83(CThing* thing);             // 0x005BF4ED
};
namespace NHeroInformationScreens {
class CBase { public: virtual void v00(); };
}
class CTCInventoryWeapons : public CTCInventoryBase_Retail, public NHeroInformationScreens::CBase { public:
    CIntelligentPointer PActiveMeleeWeapon;   // +0x134
    long ActiveMeleeWeaponDefIndex;           // +0x13C
    bool MeleeWeaponCarriedOnLoad;            // +0x140
    CIntelligentPointer PActiveRangedWeapon;  // +0x141 (unaligned, as in the PDB)
    unsigned char _pad_0x149[0x160 - 0x149];
    long ConfiscatedActiveMeleeWeaponDefIndex;    // +0x160
    long ConfiscatedActiveRangedWeaponDefIndex;   // +0x164
    static bool __fastcall IsThing(CThing* thing, CIntelligentPointer* pointer);   // 0x005CBDFA
    void vfunc_83(CThing* thing);
};
void CTCInventoryWeapons::vfunc_83(CThing* thing)
{
    if (IsThing(thing, &PActiveMeleeWeapon) || IsThing(thing, &PActiveRangedWeapon)
        || thing->DefIndex == ConfiscatedActiveMeleeWeaponDefIndex || thing->DefIndex == ConfiscatedActiveRangedWeaponDefIndex)
        thing->SetInLimbo(true);
    else
        CTCInventoryBase_Retail::vfunc_83(thing);
}