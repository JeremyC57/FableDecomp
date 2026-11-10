// CTCInventoryWeapons::OnInitialActivate() -- on activation re-creates the active melee and ranged weapons loaded from the save (by def index), puts a weapon that was carried in the hands back into its active carry slot on the owner (when the hero's weapons are usable), then runs the base inventory activation
// Leaf: ?OnInitialActivate@CTCInventoryWeapons@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005CB974, 706 bytes, thiscall plain `ret`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Identity: slot 6 of the CTCInventoryWeapons vtable 0x01253FF4 (CTCBase::OnInitialActivate; checked in Fable.exe); Ego_r.pdb / FableWin.pdb
//   ?OnInitialActivate@CTCInventoryWeapons@@UAEXXZ (Ego_r donor 0x0062C46A); it ends in NInventory::CTCInventoryBase::OnInitialActivate
//   0x005BD1EB, called qualified.
// Callees: SetThingAsActiveWeapon 0x005CB2B0, IS_THING_ALIVE 0x004CC340, GetPItem 0x00A01B50, bitset test 0x00410DE0, LowerBound const
//   0x004365B0 (PeekTC<const CTCHero> / <const CTCCarryable>) / 0x0040F020 (GetTC<CTCCarrying>), CTCCarryable::GetPCarriedByThing
//   0x00705170 / GetCarrySlotActive 0x00705CC0, CTCCarrying::RemoveThing 0x006A19B0 / PutThingInCarrySlot 0x006A1F80.
// Shape: the owner side of the carrier test is the inline accessor GetParentThing() (it loads ParentThing into ebx before the call,
//   `mov ebx,[esi+4] ; ... call ; cmp eax,ebx`); a direct `ParentThing` read is compared from memory after the call and moves the
//   carrying component out of its frame slot (retail keeps it in edi).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ISOPEN
#define FABLE_RETAIL_CTCINVENTORYBASE_CREATEOBJECT
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_MEMBERS
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_ENGINEGRAPHIC
#define FABLE_RETAIL_CTCCARRYING_REMOVETHING
#define FABLE_RETAIL_CTCCARRYING_PUTTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_ONINITIALACTIVATE
#define FABLE_RETAIL_CTCHERO_TITLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCHero.h"
#include "engine/CTCCarryable.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/CThingBaseDef.h"
#include "engine/CWeaponDef.h"
#include "engine/CDefinitionManager.h"

template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);          // 0x004CC340 (CIntelligentPointer<CThing>), 0x0040DCE0 (CThing*)

// Same-TU body of the def lookup retail's TU holds (out of line in retail: GetSubDef<CWeaponDef> 0x0042B1A6).
#pragma auto_inline(off)
template <class T>
bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
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

void CTCInventoryWeapons::OnInitialActivate()
{
    // the weapons serialised as active (OnSerialise) are made active again; one carried in the hands on load is put back into its
    // active carry slot when the hero may use weapons
    if (ActiveMeleeWeaponDefIndex.Val > 0) {
        SetThingAsActiveWeapon(ActiveMeleeWeaponDefIndex.Val, 0);
        ActiveMeleeWeaponDefIndex.Val = 0;
        if (MeleeWeaponCarriedOnLoad && IS_THING_ALIVE(PActiveMeleeWeapon)) {
            const CTCHero* hero;
            if (ParentThing->PeekTC(&hero) && hero && hero->WeaponsUsable) {
                const CTCCarryable* carryable;
                if (PActiveMeleeWeapon.Get()->PeekTC(&carryable)) {
                    CTCCarrying* carrying;
                    if (ParentThing->GetTC(&carrying)) {
                        if (carryable->PCarryingTC && carryable->GetPCarriedByThing() == &GetParentThing())
                            carrying->RemoveThing(*PActiveMeleeWeapon.Get());
                        long slot = carryable->GetCarrySlotActive();
                        if (slot > 0)
                            carrying->PutThingInCarrySlot(*PActiveMeleeWeapon.Get(), slot, true);
                    }
                }
            }
            MeleeWeaponCarriedOnLoad = false;
        }
    }
    if (ActiveRangedWeaponDefIndex.Val > 0) {
        SetThingAsActiveWeapon(ActiveRangedWeaponDefIndex.Val, 0);
        ActiveRangedWeaponDefIndex.Val = 0;
        if (RangedWeaponCarriedOnLoad && IS_THING_ALIVE(PActiveRangedWeapon)) {
            const CTCHero* hero;
            if (ParentThing->PeekTC(&hero) && hero && hero->WeaponsUsable) {
                const CTCCarryable* carryable;
                if (PActiveRangedWeapon.Get()->PeekTC(&carryable)) {
                    CTCCarrying* carrying;
                    if (ParentThing->GetTC(&carrying)) {
                        if (carryable->PCarryingTC && carryable->GetPCarriedByThing() == &GetParentThing())
                            carrying->RemoveThing(*PActiveRangedWeapon.Get());
                        long slot = carryable->GetCarrySlotActive();
                        if (slot > 0)
                            carrying->PutThingInCarrySlot(*PActiveRangedWeapon.Get(), slot, true);
                    }
                }
            }
            RangedWeaponCarriedOnLoad = false;
        }
    }
    NInventory::CTCInventoryBase::OnInitialActivate();
}