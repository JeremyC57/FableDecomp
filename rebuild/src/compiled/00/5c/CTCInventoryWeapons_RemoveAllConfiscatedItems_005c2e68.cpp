#pragma optimize("s", on)
// CTCInventoryWeapons::RemoveAllConfiscatedItems() -- empties the base inventory's confiscated items, then forgets the confiscated active melee/ranged weapon defs
// Leaf: RemoveAllConfiscatedItems
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C2E68, length 24 bytes. Size TU (`and dword ptr [esi+160h],0` zero
// stores): `optimize("s",on)`.
// Label: the catalog name "CTCVillage::InitialiseSecuritySweeps" is propagated. 0x005C2E68 is slot 74 of the
// CTCInventoryWeapons vtable 0x01253FF4; CTCInventoryClothing's slot 74 is the base body 0x005BF969 called here
// (ConfiscatedItems.clear() on the vector at +0xAC, next to the landed slot-73 ReturnAllConfiscatedItems 0x005BF91A),
// and the PDB lists `?RemoveAllConfiscatedItems@CTCInventoryWeapons@@UAEXXZ`.
// Callees: CTCInventoryBase::RemoveAllConfiscatedItems() @ 0x005BF969 (the base override called directly; manifest
// "Unwind@00fbdab0" is a stale label).
// Layout: Ego_r.pdb CTCInventoryWeapons ConfiscatedActiveMeleeWeaponDefIndex / ConfiscatedActiveRangedWeaponDefIndex
// (+0x17C / +0x180) sit at retail +0x160 / +0x164 (narrower retail containers before them). No engine header.
struct CTCInventoryBase {
    virtual void v00();
    unsigned char _pad_0x04[0x15C];
    virtual void RemoveAllConfiscatedItems();   // slot 74, base body 0x005BF969
};

struct CTCInventoryWeapons : CTCInventoryBase {
    long ConfiscatedActiveMeleeWeaponDefIndex;    // +0x160
    long ConfiscatedActiveRangedWeaponDefIndex;   // +0x164
    virtual void RemoveAllConfiscatedItems();
};

void CTCInventoryWeapons::RemoveAllConfiscatedItems()
{
    CTCInventoryBase::RemoveAllConfiscatedItems();
    ConfiscatedActiveMeleeWeaponDefIndex = 0;
    ConfiscatedActiveRangedWeaponDefIndex = 0;
}