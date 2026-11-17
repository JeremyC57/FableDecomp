// CTCHeroStats::GetFavouriteWeapon(long) const -- the description of the hero's favourite weapon (kind 1: melee weapons only, kind 2: ranged only, else any): the weapon item with the largest per-item count, or the "none" text
// Leaf: ?GetFavouriteWeapon@CTCHeroStats@@QBE?AVCWideString@@J@Z
// Retail: Fable.exe (TLC, VC7.1) 0x0057AEB4, 427 bytes, thiscall `ret 8` (hidden CWideString result + type), size TU (frame pointer).
// Inline-budget: the existing shared opt-ins FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY / FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
//   make the GetDef<CThingBaseDef> / GetSubDef<CWeaponDef> bodies visible but __declspec(noinline): retail calls both out of line
//   (0x0042AF3C / 0x0042B1A6) and the visibility is the lever (no escape of the handles' addresses). GetSubDef<CWeaponDef> grades
//   RELOCATION_MATCH at 0x0042B1A6 from this file; the shared GetDef text grades DIFFER(101v98) at 0x0042AF3C (disclosed in NOTES.md).
// Names: Ego_r.pdb / FableWin.pdb ?GetFavouriteWeapon@CTCHeroStats@@QBE?AVCWideString@@J@Z (public const); Ego_d tc_hero_stats.obj:
//   parameter `type`, locals ptc_inventory_weapons, text_bank (function scope), items / max_time_wielded / favourite (the if block),
//   item (long, the loop index) and pbase_def / pweapon_def / pper_item_data / time_wielded_weapon (the IsWeapon block).
// Callees: bitset test 0x00410DE0 + const CVectorMap::LowerBound 0x004365B0 (inline PeekTC<CTCInventoryWeapons>, TCI 19),
//   CTCInventoryWeapons::RefreshActiveWeaponData 0x005C30D3, CTCInventoryBase::GetAllItems 0x005BF482, CTCInventoryItem::IsWeapon
//   0x005D97F0, CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C,
//   CDefClassBase::GetSubDef<CWeaponDef> 0x0042B1A6, CTCWeapon::IsMeleeWeaponClass 0x005DA510, CTCInventoryBase::GetPerItemDataForItem
//   0x005BE8D3, CTCInventoryItem::GetDescription 0x005D95C0, free 0x00BFEA14 (vector storage), CMainGameComponent::PMainGameComponent
//   0x013B86A0 (inline GetTextBank), CCharString 0x0099EBF0 / ~ 0x0099EAE0, NGameText::CDataBank::GetTextBySymbol 0x009C95E0.
// Shape: the shared GetDef / GetSubDef bodies are visible (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY /
//   FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY; noinline, emitted as COMDATs): VC7.1 then keeps both def handles in edi / ebx across
//   the loop body and releases them with `dec [reg+4]` at the block end, as retail (declaration only: frame copies and reloads).
//   The loop index is a long compared with the unsigned size() (`sar ecx,3 ; je` entry test, `jb` exit) as Ego_d's `long item`.
// Member: retail reads the per-item record's +0x0C, NumberOfKills in engine/NInventory_CTCInventoryBase.h (Ego_r / Ego_d
//   CPerItemData: Augmentations, NumberOfKills, TimeWielded, ...; the 12-byte STLport vector puts NumberOfKills at +0x0C), into the
//   local Ego_d names time_wielded_weapon.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_ACTIVEMELEE
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_PERSONALITY
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_DAMAGE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CThingBaseDef.h"
#include "engine/CWeaponDef.h"
#include "engine/CTCWeapon.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
CWideString CTCHeroStats::GetFavouriteWeapon(long type) const
{
    const CTCInventoryWeapons* ptc_inventory_weapons;
    if (GetParentThing().PeekTC(&ptc_inventory_weapons)) {
        ptc_inventory_weapons->RefreshActiveWeaponData();
        std::vector<std::pair<long, long> > items;
        ptc_inventory_weapons->GetAllItems(items);
        long favourite = -1;
        long max_time_wielded = 0;
        for (long item = 0; item < items.size(); ++item) {
            if (CTCInventoryItem::IsWeapon(items[item].first)) {
                CDefPointer<const CThingBaseDef> pbase_def;
                GetDefinitionManager().GetDef(items[item].first, &pbase_def);
                CDefPointer<const CWeaponDef> pweapon_def;
                pbase_def->GetSubDef(&pweapon_def);
                if (type == 1) {
                    if (!CTCWeapon::IsMeleeWeaponClass(pweapon_def->Class))
                        continue;
                }
                else if (type == 2) {
                    if (CTCWeapon::IsMeleeWeaponClass(pweapon_def->Class))
                        continue;
                }
                const NInventory::CPerItemData* pper_item_data = ptc_inventory_weapons->GetPerItemDataForItem(items[item].first);
                if (pper_item_data) {
                    long time_wielded_weapon = pper_item_data->NumberOfKills;
                    if (time_wielded_weapon > max_time_wielded) {
                        max_time_wielded = time_wielded_weapon;
                        favourite = items[item].first;
                    }
                }
            }
        }
        if (favourite > 0)
            return CTCInventoryItem::GetDescription(favourite);
    }
    const NGameText::CDataBank& text_bank = CMainGameComponent::Get()->GetTextBank();
    return text_bank.GetTextBySymbol("TEXT_GUI_STATS_LABEL_NONE");
}
