// CTCInventoryWeapons::SetThingAsActiveWeapon(long itemDefIndex, NUISystem::CComponent* entry) -- makes a weapon of that def the active melee or ranged weapon (unless it already is): creates the inventory object in limbo, sets it as the active pointer, moves the menu's assigned-weapon icon to the entry (with the item's graphic) and makes the entry the list default, unwields the replaced weapon and kills its sheathe object on the owner's carrier, and re-dresses the carrier
// Leaf: ?SetThingAsActiveWeapon@CTCInventoryWeapons@@QAEXJPAVCComponent@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005CB2B0, 1168 bytes, thiscall `ret 8`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Identity: Ego_r.pdb / FableWin.pdb ?SetThingAsActiveWeapon@CTCInventoryWeapons@@QAEXJPAVCComponent@NUISystem@@@Z (Ego_r donor 0x0062B692,
//   after RemoveItemFromInventory 0x005CB128, the retail order); callers OnInitialActivate 0x005CB974 and the select handlers.
// Callees: NUISystem::CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D, IS_THING_ALIVE 0x004CC340 / <CThing*> 0x0040DCE0,
//   GetPItem 0x00A01B50, CTCInventoryBase::CreateInventoryObject 0x005BE8F3, CThing::SetInLimbo 0x004C8CF0, bitset test 0x00410DE0,
//   LowerBound 0x0040F020 / const 0x004365B0, CTCWeapon::IsMeleeWeapon 0x005DAA00, CIntelligentPointer<CThing>(CThing*) 0x004A96F0 /
//   operator= 0x004A9760 / ~CBaseIntelligentPointer 0x00A01C10, CDeletion() 0x0042BED4 / copy 0x0042CD84 / its list dtor 0x0042ABCA,
//   CTCInventoryItem::GetEngineGraphic 0x005D92D0, CCountedPointer<CComponent>(T*) 0x00429C15 / ReleaseData 0x004291DE,
//   CDefClassBase::GetSubDef<CWeaponDef> 0x0042B1A6, CTCCarrying::IsCarryingThing(long) 0x006A10A0 / GetCarriedThing 0x006A1120 /
//   RemoveThing 0x006A19B0, CThing::Kill 0x004C9B80, SetupActiveWeaponsOnCarrier 0x005CA68E; virtual CComponent::SetDeletion (slot 62),
//   AddChildDynamically (slot 60), CSprite::SetGraphicIndex (slot 145), CList::SetDefaultChild(CComponent*) (slot 157, 0x005362AD).
// Shape: the visible IS_THING_ALIVE<CThing*> body (below), the visible noinline CDeletion constructors (FABLE_RETAIL_CDELETION_BODIES:
//   the Method store sinks below the copy's pushes), the out-of-line CIntelligentPointer (T*) / operator= pair for `= thing`, the
//   expanded GetTC / PeekTC; the hero's WeaponsUsable flag (+0x0C) gates the re-dress (no hero component: always).
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
#define FABLE_RETAIL_CDELETION_BODIES
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_MEMBERS
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_ENGINEGRAPHIC
#define FABLE_RETAIL_CTCCARRYING_REMOVETHING
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
// Same-TU body of IS_THING_ALIVE<CThing*> (out of line in retail: 0x0040DCE0, the landed `thing != 0 && !IsDead()` body): VC7.1 sees that
// it does not keep the reference, so the created thing stays in esi and is passed through a copy in the dead argument slot
// (`mov [ebp+8],esi ; lea ecx,[ebp+8]`), as retail does. The CIntelligentPointer instantiation 0x004CC340 stays a declaration.
#pragma auto_inline(off)
template <> inline bool FABLE_FASTCALL IS_THING_ALIVE<CThing*>(CThing* const& thing)
{
    if (thing != 0 && thing->IsDead() == 0)
        return true;
    return false;
}
#pragma auto_inline(on)

void CTCInventoryWeapons::SetThingAsActiveWeapon(long itemDefIndex, NUISystem::CComponent* entry)
{
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    if (IS_THING_ALIVE(PActiveMeleeWeapon) && PActiveMeleeWeapon.Get()->GetDefGlobalIndex() == itemDefIndex)
        return;
    if (IS_THING_ALIVE(PActiveRangedWeapon) && PActiveRangedWeapon.Get()->GetDefGlobalIndex() == itemDefIndex)
        return;
    CThing* thing = CreateInventoryObject(itemDefIndex);
    if (!IS_THING_ALIVE(thing))
        return;
    thing->SetInLimbo(true);
    CTCWeapon* weapon;
    if (!thing->GetTC(&weapon))
        return;
    CThing* previous;                           // the weapon this one replaces
    if (weapon->IsMeleeWeapon()) {
        previous = PActiveMeleeWeapon.Get();
        PActiveMeleeWeapon = thing;
        if (entry) {
            PAssignedMeleeMenuEntry = entry;
            if (PAssignedMeleeIcon) {
                NUISystem::CDeletion deletion;
                deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                PAssignedMeleeIcon->SetDeletion(deletion);
            }
            CTCInventoryItem* item = 0;
            thing->GetTC(&item);
            if (item->GetEngineGraphic().BankIndex != 0) {
                CCountedPointer<NUISystem::CComponent> icon(manager->CreateComponent("UI_ASSIGNED_WEAPON_ICON", false));
                static_cast<NUISystem::CSprite*>(icon.Get())->SetGraphicIndex(item->GetEngineGraphic().BankIndex);
                entry->AddChildDynamically(icon);
                PAssignedMeleeIcon = icon.Get();
            }
            if (PMeleeList)
                static_cast<NUISystem::CList*>(PMeleeList)->SetDefaultChild(entry);
        }
    } else {
        previous = PActiveRangedWeapon.Get();
        PActiveRangedWeapon = thing;
        if (entry) {
            PAssignedRangedMenuEntry = entry;
            if (PAssignedRangedIcon) {
                NUISystem::CDeletion deletion;
                deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                PAssignedRangedIcon->SetDeletion(deletion);
            }
            CTCInventoryItem* item = 0;
            thing->GetTC(&item);
            if (item->GetEngineGraphic().BankIndex != 0) {
                CCountedPointer<NUISystem::CComponent> icon(manager->CreateComponent("UI_ASSIGNED_WEAPON_ICON", false));
                static_cast<NUISystem::CSprite*>(icon.Get())->SetGraphicIndex(item->GetEngineGraphic().BankIndex);
                entry->AddChildDynamically(icon);
                PAssignedRangedIcon = icon.Get();
            }
            if (PRangedList)
                static_cast<NUISystem::CList*>(PRangedList)->SetDefaultChild(entry);
        }
    }
    if (IS_THING_ALIVE(previous)) {             // unwield it and drop its sheathe object from the owner
        CTCWeapon* previousWeapon = 0;
        previous->GetTC(&previousWeapon);
        CDefPointer<const CWeaponDef> weaponDef;
        previousWeapon->Wielded = false;
        previous->GetPBaseDef()->GetSubDef(&weaponDef);
        CTCCarrying* carrying = 0;
        ParentThing->GetTC(&carrying);
        if (weaponDef->SheatheObject > 0 && carrying->IsCarryingThing(weaponDef->SheatheObject)) {
            CThing* sheathe = carrying->GetCarriedThing(weaponDef->SheatheObject);
            carrying->RemoveThing(*sheathe);
            sheathe->Kill(true);
        }
    }
    weapon->Wielded = true;
    // dress the carrier again unless the inventory is open or the hero may not use weapons
    if (!IsInventoryOpen()) {
        const CTCHero* hero;
        if (!ParentThing->PeekTC(&hero) || hero->WeaponsUsable)
            SetupActiveWeaponsOnCarrier();
    }
}