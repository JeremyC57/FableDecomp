// CTCInventoryWeapons::GetItemDescriptionDamage(CWideString& description, long defIndex) const -- for a weapon item: the live active melee weapon's damage and the live active ranged weapon's damage (its ability def's damage def value times the object-augmentations damage multiplier), then ConstructWeaponDescriptionDamage with the one matching the item's weapon class; other items go to the base description
// Leaf: ?GetItemDescriptionDamage@CTCInventoryWeapons@@QBEXAAVCWideString@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C7F07, 506 bytes, `ret 8`; Ego_r.pdb ?GetItemDescriptionDamage@CTCInventoryWeapons@@QBEXAAVCWideString@@J@Z.
//   Frame-pointer size TU: optimize("y",off) + ("s",on) (`fldz ; fstp`, `leave`). One of four byte-twins (0x005C7913 GetItemDescription, 0x005C7B10 GetItemDescriptionNoDamageNorWeight, 0x005C7D0D GetItemDescriptionWeight) that
//   differ only in the final Construct* callee and its arguments.
// Callees: CTCInventoryItem::IsWeapon @ 0x005D97F0 (static __fastcall), IS_THING_ALIVE<CIntelligentPointer<CThing> > @ 0x004CC340,
//   GetPItem @ 0x00A01B50, the expanded CThing::PeekTC (bitset::test 0x00410DE0, const CVectorMap::LowerBound 0x004365B0) for
//   CTCWeapon (0x48) and CTCObjectAugmentations (0xBF), CTCWeapon::GetDamage @ 0x005DAAA0, GetSubDef<CWeaponDef> @ 0x0042B1A6,
//   CTCBase::GetDefinitionManager() const @ 0x006869D0, CDefinitionManager::GetPDefFromGlobalIndex @ 0x009AD9E0 (hidden result) /
//   GetDef<CThingBaseDef> @ 0x0042AF3C, CTCObjectAugmentations::GetDamageMultiplier @ 0x00766140, CTCWeapon::IsMeleeWeaponClass @
//   0x005DA510, ConstructWeaponDescriptionDamage @ 0x005C7452, NInventory::CTCInventoryBase::GetItemDescription @ 0x005BC6B3 (called qualified).
// Shape: the active-weapon damage blocks as written out in ConstructScreen 0x005C8459 / RefreshWeaponDescriptions 0x005CACB9 (the melee
//   weapon pointer uninitialised: PeekTC leaves it unchanged on a miss; the ranged shot def adopted from GetPDefFromGlobalIndex's
//   temporary); the def lookups through the headers' noinline GetDef / GetSubDef bodies.
// Layout: engine/CTCInventoryWeapons.h (PActiveMeleeWeapon +0x134, PActiveRangedWeapon +0x141); CWeaponDef Shot +0xA0, Class +0x2C;
//   CThingShotDef HitThingDamage +0x54; CThing PBaseDef +0x70.
// Name fix (agentNF10 2026-11-11): moved off the local CDefPointeeBase / CDefPointer / CWeaponDef / CAbilityDef / CDamageDef /
//   CDefClassBase / CDefinitionManager / CThing / CBaseIntelligentPointer / CTCInventoryBase stand-ins (and their same-TU GetDef /
//   GetSubDef stand-in bodies) onto the shared headers (CTCInventoryWeapons.h FABLE_RETAIL_CTCINVENTORYWEAPONS_ITEMDESCRIPTION,
//   CTCObjectAugmentations.h FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_DAMAGEMULTIPLIER). 0x0042AF3C binds GetDef<CThingBaseDef> (was
//   ?GetDef@CDefinitionManager@@QAE_NJPAV?$CDefPointer@VCDefClassBase@@@@@Z), 0x006869D0 the PDB const GetDefinitionManager (was
//   ?GetDefinitionManager@CTCInventoryBase@NInventory@@QBEPAVCDefinitionManager@@XZ), 0x0042B1A6 GetSubDef<CWeaponDef>, 0x005D97F0
//   ?IsWeapon@CTCInventoryItem@@SI_NJ@Z (was ?IsWeaponItem@@YI_NJ@Z), 0x005DA510 ?IsMeleeWeaponClass@CTCWeapon@@SI_NW4EWeaponClass@@@Z
//   (was ?IsMeleeWeaponClass@@YI_NJ@Z), 0x004CC340 the IS_THING_ALIVE template, 0x005BC6B3 the PDB virtual UBE base spelling.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_ITEMDESCRIPTION
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_DAMAGE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_DAMAGEMULTIPLIER
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCObjectAugmentations.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CWeaponDef.h"
#include "engine/CThingShotDef.h"

template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);          // 0x004CC340

void CTCInventoryWeapons::GetItemDescriptionDamage(CWideString& text, long item_def_index) const
{
    if (CTCInventoryItem::IsWeapon(item_def_index)) {
        float melee_damage = 0.0f;
        float ranged_damage = 0.0f;
        if (IS_THING_ALIVE(PActiveMeleeWeapon)) {
            const CTCWeapon* weapon;
            static_cast<CThing*>(PActiveMeleeWeapon.GetPItem())->PeekTC(&weapon);
            melee_damage = weapon->GetDamage();
        }
        if (IS_THING_ALIVE(PActiveRangedWeapon)) {
            CDefPointer<const CWeaponDef> weapon_def;
            static_cast<CThing*>(PActiveRangedWeapon.GetPItem())->GetPBaseDef()->GetSubDef(&weapon_def);
            CDefPointer<const CThingShotDef> shot(GetDefinitionManager().GetPDefFromGlobalIndex(weapon_def->Shot));
            ranged_damage = shot->HitThingDamage;
            const CTCObjectAugmentations* augmentations;
            if (static_cast<CThing*>(PActiveRangedWeapon.GetPItem())->PeekTC(&augmentations))
                ranged_damage *= augmentations->GetDamageMultiplier();
        }
        CDefPointer<const CThingBaseDef> def;
        GetDefinitionManager().GetDef(item_def_index, &def);
        CDefPointer<const CWeaponDef> weapon_def;
        def->GetSubDef(&weapon_def);
        float damage = CTCWeapon::IsMeleeWeaponClass(weapon_def->Class) ? melee_damage : ranged_damage;
        ConstructWeaponDescriptionDamage(text, item_def_index, damage);
    } else
        NInventory::CTCInventoryBase::GetItemDescription(text, item_def_index, false);
}
