// CTCInventoryWeapons::RefreshActiveWeaponData() -- re-applies the base inventory's per-item data update to each active weapon (melee, then ranged) that is still alive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C30D3, 71 bytes.
// Callees: IS_THING_ALIVE<CIntelligentPointer> @ 0x004CC340 (`__fastcall`, pointer in ecx), CBaseIntelligentPointer::GetPItem
//          @ 0x00A01B50, the base inventory's per-item update @ 0x005BF414 (takes the CThing*, checks its interface 0x1A;
//          manifest label "CTCInventoryStats::BuildRenownString" is propagated -- name here descriptive).
//
// Shape: two identical blocks, each forming the pointer's address once (`lea edi,[esi+134h]` / `lea edi,[esi+141h]`) for
// both the liveness test and GetPItem; the item is pushed before `mov ecx,esi`. Size TU (`#pragma optimize("s", on)`).
// PActiveMeleeWeapon / PActiveRangedWeapon are PDB +0x14C / +0x159, retail +0x134 / +0x141 (unaligned in both).
// Local model (no engine header for CTCInventoryWeapons).
#pragma optimize("s", on)
struct CThing;
struct CIntelligentPointer {
    unsigned char _body[8];
    CThing* GetPItem();                       // 0x00A01B50
};
bool __fastcall IS_THING_ALIVE(CIntelligentPointer* pointer);   // 0x004CC340
struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char _pad_0x04[0x130 - 4];
    void RefreshItemData(CThing* item);       // 0x005BF414
};
namespace NHeroInformationScreens {
struct CBase { virtual void v00(); };
}
struct CTCInventoryWeapons : CTCInventoryBase_Retail, NHeroInformationScreens::CBase {
    CIntelligentPointer PActiveMeleeWeapon;   // +0x134
    long ActiveMeleeWeaponDefIndex;           // +0x13C
    bool MeleeWeaponCarriedOnLoad;            // +0x140
    CIntelligentPointer PActiveRangedWeapon;  // +0x141
    void RefreshActiveWeaponData();
};
void CTCInventoryWeapons::RefreshActiveWeaponData()
{
    if (IS_THING_ALIVE(&PActiveMeleeWeapon))
        RefreshItemData(PActiveMeleeWeapon.GetPItem());
    if (IS_THING_ALIVE(&PActiveRangedWeapon))
        RefreshItemData(PActiveRangedWeapon.GetPItem());
}