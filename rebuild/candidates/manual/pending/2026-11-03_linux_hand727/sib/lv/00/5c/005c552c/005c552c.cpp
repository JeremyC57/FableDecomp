// CTCInventoryWeapons::RemoveWeaponsFromSheathes() -- take the wielded weapons out of their sheathes: for the active melee and ranged weapon that the carrier does not hold in a primary slot, find the weapon def's sheathe object (carried in the slot SheatheObject names, or the weapon itself when it is 0), restore its full scale, drop it from the carrier and end its show (or kill it when it is not showable)
// Leaf: ?RemoveWeaponsFromSheathes@CTCInventoryWeapons@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C552C, 546 bytes, plain `ret`. Frame-pointer size TU: optimize("y",off) + ("s",on).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefClassBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CArray.h"
#include "engine/CTCInventoryItem.h"

class CWeaponDef : public CDefClassBase { public:
    unsigned char _pad_weapon[0x38 - sizeof(CDefClassBase)];
    long SheatheObject;                                           // +0x38
    static CCharString __fastcall StatGetTypeName() { return CCharString("CWeaponDef", -1); }
};
class CEngineGraphic { public:                                   // Ego_r.pdb CEngineGraphic (14 bytes)
    unsigned long BankIndex;                                      // +0x00
    float AnimStep;                                               // +0x04
    float RenderSizeX;                                            // +0x08
    bool AdditiveAlpha;                                           // +0x0C
    unsigned char Type;                                           // +0x0D
};
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:                              // retail offsets (STLport lists 4 bytes: Ego_r -0x18 from Scale on)
    unsigned char _pad00[0x44];
    CEngineGraphic MainGraphic;                                   // +0x44 (Ego_r +0x5C)
    void SetScale(float scale);                                   // 0x004BFA50 ?SetScale@CTCGraphicAppearance@NGraphicAppearance@@QAEXM@Z
};
}
class CTCCarrying : public CTCBase { public:                    // no engine header; Ego_r.pdb member spellings
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CARRYING; }   // ?StatGetInterfaceType@CTCCarrying@@SI?AW4ETCInterfaceType@@XZ
    bool IsCarryingThing(const CThing& thing) const;              // 0x006A1060
    bool IsCarriedThingInPrimarySlot(const CThing& thing) const;  // 0x006A1490
    CThing* GetCarriedThing(long slot) const;                     // 0x006A1120
    void RemoveThing(CThing& thing);                              // 0x006A19B0
};
// The TU's own out-of-line template bodies (COMDATs retail calls: IS_THING_ALIVE<CThing*> 0x0040DCE0,
// IS_THING_ALIVE<CIntelligentPointer<CThing> > 0x004CC340, GetSubDef<CWeaponDef> 0x0042B1A6).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing && !thing->IsDead();
}
template <> __declspec(noinline) bool __fastcall IS_THING_ALIVE<CIntelligentPointer<CThing> >(const CIntelligentPointer<CThing>& thing)
{
    return thing.Get() && !thing->IsDead();
}
class CDefinitionManager { friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#pragma auto_inline(on)

#pragma pack(push, 1)
class CTCInventoryWeapons : public CTCBase { public:
    unsigned char _pad_inv[0x134 - sizeof(CTCBase)];
    CIntelligentPointer<CThing> PActiveMeleeWeapon;               // +0x134
    long ActiveMeleeWeaponDefIndex;                               // +0x13C
    bool MeleeWeaponCarriedOnLoad;                                // +0x140
    CIntelligentPointer<CThing> PActiveRangedWeapon;              // +0x141
    void RemoveWeaponsFromSheathes();
};
#pragma pack(pop)

void CTCInventoryWeapons::RemoveWeaponsFromSheathes()
{
    CTCCarrying* carrying = 0;
    ParentThing->GetTC(&carrying);
    CArray<CThing*> weapons;
    if (IS_THING_ALIVE(PActiveMeleeWeapon))
        weapons.push_back(PActiveMeleeWeapon.Get());
    if (IS_THING_ALIVE(PActiveRangedWeapon))
        weapons.push_back(PActiveRangedWeapon.Get());
    for (CThing** it = weapons.begin(); it != weapons.end(); ++it) {
        CThing* weapon = *it;
        if (carrying->IsCarryingThing(*weapon) && carrying->IsCarriedThingInPrimarySlot(*weapon))
            continue;
        CDefPointer<const CWeaponDef> weaponDef;
        if (!weapon->GetPBaseDef().IsNull() && weapon->GetPBaseDef()->GetSubDef(&weaponDef) && weapon->HasTC(TCI_WEAPON)) {
            CThing* sheathe = 0;
            if (weaponDef->SheatheObject > 0)
                sheathe = carrying->GetCarriedThing(weaponDef->SheatheObject);
            else if (weaponDef->SheatheObject != 0)
                sheathe = weapon;
            if (IS_THING_ALIVE(sheathe)) {
                sheathe->GetTCGraphicAppearance()->SetScale(sheathe->GetTCGraphicAppearance()->MainGraphic.RenderSizeX);
                if (carrying->IsCarryingThing(*sheathe))
                    carrying->RemoveThing(*sheathe);
                CTCInventoryItem* item;
                if (sheathe->GetTC(&item))
                    item->ReturnToInventory();
                else
                    sheathe->Kill(true);
            }
        }
    }
}