// CTCInventoryWeapons::ReturnAllConfiscatedItems() -- returns the base inventory's confiscated items, then re-equips each confiscated active weapon (melee, ranged) and forgets it
// Leaf: ?ReturnAllConfiscatedItems@CTCInventoryWeapons
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CB742, 62 bytes.
// Callees: CTCInventoryBase::ReturnAllConfiscatedItems @ 0x005BF91A, CTCInventoryWeapons::SetThingAsActiveWeapon(long,
//          NUISystem::CComponent*) @ 0x005CB2B0.
//
// Shape: the explicit base call first; then two identical blocks, each addressing its member once through `lea edi`
// (`mov eax,[edi] ; test eax,eax ; jle`), the call with a null menu entry (`push 0 ; push eax`), and the size-TU
// zero store `and dword ptr [edi],0` (`#pragma optimize("s", on)`). The `jle` is a signed `> 0` test (def indices are
// -1/0 when unset). ConfiscatedActive{Melee,Ranged}WeaponDefIndex are PDB +0x17C/+0x180, retail +0x160/+0x164.
// Local model (no engine header for CTCInventoryWeapons).
#pragma optimize("s", on)
namespace NUISystem { struct CComponent; }
struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char _pad_0x04[0x130 - 4];
    void ReturnAllConfiscatedItems();         // 0x005BF91A
};
namespace NHeroInformationScreens {
struct CBase { virtual void v00(); };
}
struct CTCInventoryWeapons : CTCInventoryBase_Retail, NHeroInformationScreens::CBase {
    unsigned char _pad_0x134[0x160 - 0x134];
    long ConfiscatedActiveMeleeWeaponDefIndex;    // +0x160
    long ConfiscatedActiveRangedWeaponDefIndex;   // +0x164
    void SetThingAsActiveWeapon(long defIndex, NUISystem::CComponent* menuEntry);   // 0x005CB2B0
    void ReturnAllConfiscatedItems();
};
void CTCInventoryWeapons::ReturnAllConfiscatedItems()
{
    CTCInventoryBase_Retail::ReturnAllConfiscatedItems();
    if (ConfiscatedActiveMeleeWeaponDefIndex > 0) {
        SetThingAsActiveWeapon(ConfiscatedActiveMeleeWeaponDefIndex, 0);
        ConfiscatedActiveMeleeWeaponDefIndex = 0;
    }
    if (ConfiscatedActiveRangedWeaponDefIndex > 0) {
        SetThingAsActiveWeapon(ConfiscatedActiveRangedWeaponDefIndex, 0);
        ConfiscatedActiveRangedWeaponDefIndex = 0;
    }
}
