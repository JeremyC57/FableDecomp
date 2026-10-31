// Grade: DIFFER(181v180) (sec1b 2026-10-31) -- same-TU noinline stand-ins for the GetDef/GetSubDef templates keep the weapon def in a register (198->181, 31->15 difflines); left: retail keeps itemType in edi and the def in ebx, ours pushes [ebp+8] each time and puts the def in edi (tried: const long copy, long& alias, dtor release, if/else condition)
// CTCInventoryWeapons::OnItemAdded(long itemType, bool a, bool b) -- after the base bookkeeping, makes a newly added weapon the active one when the hero has no living weapon of its class (melee or ranged), then refreshes the sheathed weapons
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CBC36, length 180 bytes (`ret 0Ch`). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CTCInventoryBase::OnItemAdded(long, bool, bool) @ 0x005BE028, CTCBase::GetDefinitions() @ 0x006869D0, CDefinitionManager::
// GetDef(long, CDefPointer<T>*) @ 0x0042AF3C, CDefClassBase::GetSubDef<CWeaponDef>(CDefPointer<T>*) @ 0x0042B1A6 (catalog GetSubDef<CAbilityDef>,
// a byte twin), IsMeleeWeaponClass(EWeaponClass) @ 0x005DA510 and IsRangedWeaponClass(EWeaponClass) @ 0x005DA530 (`__fastcall`; the catalog's
// "HasState" / "CTCHeroOnlineScoreboard::Close" labels are propagated), IS_THING_ALIVE<CIntelligentPointer<const CThingPhysical>>
// @ 0x004CC340 (`__fastcall`, by reference), CTCInventoryWeapons::SetThingAsActiveWeapon(long, bool) @ 0x005CB2B0 and
// RefreshSheathedWeapons() @ 0x005C9962, the defs' virtual slot 1 on release.
// Layout: retail CTCInventoryWeapons PActiveMeleeWeapon +0x134 and PActiveRangedWeapon +0x141 (PDB +0x14C/+0x159; 8-byte intelligent
// pointers, the second at an odd offset, hence pack(1)); CWeaponDef::Class +0x2C (PDB).
// Shape: the two def out pointers are zeroed into the dead argument slots (b at [ebp+10h], a at [ebp+0Ch]); the weapon def is read
// into ebx right after the lookup and released from there, the owning def through its slot, both null-tested (`dec [p+4] ; jne`).
#pragma optimize("y", off)
#pragma optimize("s", on)
enum EWeaponClass { WEAPON_CLASS_NULL };
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long m_refCount;                                 // +0x04
};
template <class T> struct CDefPointer {              // the out slot the definition lookups fill (released by hand, as the landed OnCreate twins)
    T* Object;
    T* operator->() const { return Object; }
};
struct CWeaponDef : CDefPointeeBase {
    unsigned char _pad08[0x2c - 0x08];
    EWeaponClass  Class;                             // +0x2c
};
struct CDefClassBase : CDefPointeeBase {
    template <class T> bool GetSubDef(CDefPointer<T>* out);    // 0x0042B1A6 (CWeaponDef)
};
CWeaponDef* FindWeaponSubDef_5cbc36(CDefClassBase* def);
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>* out)
{
    T* found = (T*)FindWeaponSubDef_5cbc36(this);
    out->Object = found;
    return found != 0;
}
#pragma auto_inline(on)
struct CDefinitionManager {
    template <class T> bool GetDef(long index, CDefPointer<T>* out);   // 0x0042AF3C (CDefClassBase)
};
CDefClassBase* FindDef_5cbc36(CDefinitionManager* manager, long index);
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<T>* out)
{
    T* found = (T*)FindDef_5cbc36(this, index);
    out->Object = found;
    return found != 0;
}
#pragma auto_inline(on)
bool __fastcall IsMeleeWeaponClass(EWeaponClass weaponClass);    // 0x005DA510
bool __fastcall IsRangedWeaponClass(EWeaponClass weaponClass);   // 0x005DA530
struct CThing;
struct CIntelligentPointer {                         // CIntelligentPointer<CThing> (8 bytes)
    void*   __vftable;
    CThing* PItem;
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x004CC340 for CIntelligentPointer

namespace NInventory {
#pragma pack(push, 1)
struct CTCInventoryWeapons {
    unsigned char       _pad000[0x130];
    unsigned char       _pad130[0x04];
    CIntelligentPointer PActiveMeleeWeapon;          // +0x134
    long                ActiveMeleeWeaponDefIndex;   // +0x13c
    bool                MeleeWeaponCarriedOnLoad;    // +0x140
    CIntelligentPointer PActiveRangedWeapon;         // +0x141
    long                ActiveRangedWeaponDefIndex;  // +0x149
    bool                RangedWeaponCarriedOnLoad;   // +0x14d
    void OnItemAdded_Base(long itemType, bool a, bool b);   // 0x005BE028 (CTCInventoryBase::OnItemAdded)
    CDefinitionManager* GetDefinitions();            // 0x006869D0 (CTCBase)
    void SetThingAsActiveWeapon(long itemType, bool b);   // 0x005CB2B0
    void RefreshSheathedWeapons();                   // 0x005C9962
    void OnItemAdded(long itemType, bool a, bool b);
};
#pragma pack(pop)
void CTCInventoryWeapons::OnItemAdded(long itemType, bool a, bool b)
{
    OnItemAdded_Base(itemType, a, b);
    CDefPointer<CWeaponDef> weaponDef;
    CDefPointer<CDefClassBase> def;
    def.Object = 0;
    GetDefinitions()->GetDef(itemType, &def);
    weaponDef.Object = 0;
    bool found = def->GetSubDef(&weaponDef);
    CWeaponDef* weapon = weaponDef.Object;
    if (found) {
        if ((IsMeleeWeaponClass(weapon->Class) && !IS_THING_ALIVE(PActiveMeleeWeapon)) ||
            (IsRangedWeaponClass(weapon->Class) && !IS_THING_ALIVE(PActiveRangedWeapon)))
            SetThingAsActiveWeapon(itemType, false);
        RefreshSheathedWeapons();
    }
    if (weapon && --weapon->m_refCount == 0)
        weapon->Destroy();
    CDefClassBase* owner = def.Object;
    if (owner && --owner->m_refCount == 0)
        owner->Destroy();
}
}
