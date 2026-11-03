// PutSheatheObjectInCarrySlot(CThing& owner, CThing& weapon, float& offset) -- file-static helper of the CTCInventoryWeapons TU: put the sheathe object of an active weapon into its passive carry slot on the owner (the weapon itself when its def's SheatheObject is negative, a new CThingObject of that def when positive); the sheathe takes the running stack offset, which then grows by its carryable def's OffsetAmountForThingsOnTopOfMe, and is scaled by PassiveCarrySlotScale; whatever already sits in the slot goes back to the inventory (or is killed)
// Leaf: ?PutSheatheObjectInCarrySlot@
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C36C2, 645 bytes, `ret 4` (__fastcall: ecx owner, edx weapon, the float& on the stack).
// Name: Ego_r.pdb ?PutSheatheObjectInCarrySlot@?A0x0a34196c@@YIXAAVCThing@@0AAM@Z (anonymous namespace of the CTCInventoryWeapons TU,
//   between GetActiveMeleeWeaponDamage and FinalConstructWeaponItemEntries in donor order); its only caller is
//   CTCInventoryWeapons::PutWeaponsInSheathes 0x005C8101 (`mov edx,eax; mov ecx,ebx; call` for the melee and ranged weapon).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCHARSTRING
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

class CCarryableDef : public CDefClassBase { public:            // Ego_r.pdb layout (no engine header)
    unsigned char _pad_carryable[0x28 - sizeof(CDefClassBase)];
    long ActiveCarrySlot;                                         // +0x28
    long SecondaryActiveCarrySlot;                                // +0x2C
    long PassiveCarrySlot;                                        // +0x30
    long OnKillFX;                                                // +0x34
    float OffsetCoordRelativeToAttachToDummy[3];                  // +0x38 C3DVector
    float OffsetAmountForThingsOnTopOfMe;                         // +0x44
    float PassiveCarrySlotScale;                                  // +0x48
    bool IncludeInThingScansWhenCarried;                          // +0x4C
    static CCharString __fastcall StatGetTypeName() { return CCharString("CCarryableDef", -1); }
};
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
class CTCCarrying;
class CTCCarryable : public CTCBase { public:                   // Ego_r.pdb layout (no engine header)
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CARRYABLE; }   // ?StatGetInterfaceType@CTCCarryable@@SI?AW4ETCInterfaceType@@XZ
    bool ForceFragility;                                          // +0x0C
    CTCCarrying* PCarryingTC;                                     // +0x10
    float OffsetCoordRelativeToAttachToDummy[3];                  // +0x14 C3DVector
    float OffsetAmount;                                           // +0x20
    long GetCarrySlotPassive() const;                             // 0x00705D00
};
class CRightHandedSet;
class CThingObject : public CThing { public:
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& pos, long playerNumber, const CRightHandedSet* rhs, unsigned long flags, const CCharString& scriptName);   // 0x00703210
};
class CTCCarrying : public CTCBase { public:                    // no engine header; Ego_r.pdb member spellings
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CARRYING; }   // ?StatGetInterfaceType@CTCCarrying@@SI?AW4ETCInterfaceType@@XZ
    bool IsCarryingThing(const CThing& thing) const;              // 0x006A1060
    CThing* GetThingInCarrySlot(long slot) const;                 // 0x006A11F0
    void PutThingInCarrySlot(CThing& thing, long slot, bool primary);   // 0x006A1F80
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

namespace {
void __fastcall PutSheatheObjectInCarrySlot(CThing& owner, CThing& weapon, float& offset)
{
    CTCCarrying* carrying = 0;
    owner.GetTC(&carrying);
    long sheatheObject = -1;
    CDefPointer<const CWeaponDef> weaponDef;
    bool gotWeaponDef = !weapon.GetPBaseDef().IsNull() && weapon.GetPBaseDef()->GetSubDef(&weaponDef);
    if (gotWeaponDef)
        sheatheObject = weaponDef->SheatheObject;
    if (!carrying->IsCarryingThing(weapon) || sheatheObject > 0) {
        CThing* sheathe = &weapon;
        if (sheatheObject > 0)
            sheathe = CThingObject::Create(weaponDef->SheatheObject, owner.GetPos(), owner.GetPlayerNumber(), 0, 0, "");
        else if (sheatheObject == 0)
            sheathe = 0;
        CTCCarryable* carryable;
        if (IS_THING_ALIVE(sheathe) && sheathe->GetTC(&carryable)) {
            carryable->OffsetAmount = offset;
            CDefPointer<const CCarryableDef> carryableDef;
            bool gotCarryableDef = sheathe->GetPBaseDef()->GetSubDef(&carryableDef);
            if (gotCarryableDef) {
                offset += carryableDef->OffsetAmountForThingsOnTopOfMe;
                sheathe->GetTCGraphicAppearance()->SetScale(carryableDef->PassiveCarrySlotScale * sheathe->GetTCGraphicAppearance()->MainGraphic.RenderSizeX);
            }
            long slot = carryable->GetCarrySlotPassive();
            CThing* inSlot = carrying->GetThingInCarrySlot(slot);
            if (IS_THING_ALIVE(inSlot)) {
                CTCInventoryItem* item;
                if (inSlot->GetTC(&item))
                    item->ReturnToInventory();
                else
                    inSlot->Kill(true);
            }
            sheathe->SetInLimbo(false);
            carrying->PutThingInCarrySlot(*sheathe, slot, true);
        }
    }
}
}
