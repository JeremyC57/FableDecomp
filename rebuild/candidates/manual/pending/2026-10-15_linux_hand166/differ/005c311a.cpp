// CTCInventoryWeapons::GetItemOfTypeInInventory(long itemType) -- the active melee or ranged weapon thing when it is alive and of that type, else the base inventory's freshly created item
// Grade: DIFFER(102v102) -- register swap only: retail keeps `this` in edi and the weapon address in esi, ours the reverse; a single weapon-pointer local gets the registers right but moves the merged GetPItem tail below the base call (tried: two plain arms, weapon local + negated nested ifs, `||` with assignment, weapon local + separate returns)
// Leaf: GetItemOfTypeInInventory@CTCInventoryWeapons
// Leaf: GetItemOfTypeInInventory@CTCInventoryWeapons
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C311A, length 102 bytes (`ret 4`). Size TU (`#pragma optimize("s", on)`, as the
// landed neighbour RefreshActiveWeaponData 0x005C30D3).
// Callees: IS_THING_ALIVE<CIntelligentPointer> @ 0x004CC340 (`__fastcall`, pointer in ecx), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50, CTCInventoryBase::GetItemOfTypeInInventory(long) @ 0x005BF3F1 (qualified base call).
// Shape: each weapon's address is formed once in esi (`lea esi,[edi+134h]` / `lea esi,[edi+141h]`); the two hit arms tail-merge
// into one `GetPItem` call on esi; the def index is CThing's unsigned short +0x8C.
// Layout: PActiveMeleeWeapon / PActiveRangedWeapon at retail +0x134 / +0x141 (PDB +0x14C / +0x159; unaligned in both).
#pragma optimize("s", on)
struct CThing {
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;                  // +0x8c
};
struct CIntelligentPointer {
    unsigned char _body[8];
    CThing* GetPItem();                       // 0x00A01B50
};
bool __fastcall IS_THING_ALIVE(CIntelligentPointer* pointer);   // 0x004CC340
struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char _pad_0x04[0x130 - 4];
    CThing* GetItemOfTypeInInventory(long itemType);   // 0x005BF3F1
};
namespace NHeroInformationScreens {
struct CBase { virtual void v00(); };
}
struct CTCInventoryWeapons : CTCInventoryBase_Retail, NHeroInformationScreens::CBase {
    CIntelligentPointer PActiveMeleeWeapon;   // +0x134
    long ActiveMeleeWeaponDefIndex;           // +0x13C
    bool MeleeWeaponCarriedOnLoad;            // +0x140
    CIntelligentPointer PActiveRangedWeapon;  // +0x141
    CThing* GetItemOfTypeInInventory(long itemType);
};
CThing* CTCInventoryWeapons::GetItemOfTypeInInventory(long itemType)
{
    if (IS_THING_ALIVE(&PActiveMeleeWeapon) && PActiveMeleeWeapon.GetPItem()->DefIndex == itemType)
        return PActiveMeleeWeapon.GetPItem();
    if (IS_THING_ALIVE(&PActiveRangedWeapon) && PActiveRangedWeapon.GetPItem()->DefIndex == itemType)
        return PActiveRangedWeapon.GetPItem();
    return CTCInventoryBase_Retail::GetItemOfTypeInInventory(itemType);
}
