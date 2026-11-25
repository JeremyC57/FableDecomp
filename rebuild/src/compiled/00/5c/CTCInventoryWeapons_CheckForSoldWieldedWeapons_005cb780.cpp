// CTCInventoryWeapons::CheckForSoldWieldedWeapons() -- when the wielded melee (then ranged) weapon is gone (IS_THING_DEAD: null or dead, e.g. sold), re-arm from the first carried item (count > 0) whose weapon sub-def has a melee (non-melee) weapon class
// Leaf: ?CheckForSoldWieldedWeapons@CTCInventoryWeapons@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CB780, 500 bytes, plain `ret`. Frame-pointer size TU: optimize("s",on).
// Callees: IS_THING_DEAD<CIntelligentPointer<CThing> > @ 0x005CC1B0 (`__fastcall`, out of line), NInventory::CTCInventoryBase::GetAllItems
//   @ 0x005BF482, CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetPDefFromGlobalIndex @ 0x009AD9E0,
//   CDefClassBase::GetSubDef<CWeaponDef> @ 0x0042B1A6, CTCWeapon::IsMeleeWeaponClass @ 0x005DA510, SetThingAsActiveWeapon @ 0x005CB2B0,
//   free @ 0x00BFEA14 (the vector's storage).
// Name fix (agentNF91, q11202): the self name was already Ego_r.pdb + FableWin.pdb ?CheckForSoldWieldedWeapons@CTCInventoryWeapons@@QAEXXZ;
//   the row moved off its row-local CDefPointer / CDefClassBase / CWeaponDef / CDefinitionManager / CItemVector / CTCBase / CTCInventoryBase /
//   CTCInventoryWeapons models (and a fake GetSubDef stand-in body) onto the shared headers, so it binds 0x005BF482 as the const PDB
//   ?GetAllItems@CTCInventoryBase@NInventory@@QBEXAAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@@Z (was AAUCItemVector).
//   The GetSubDef<T> header template body (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY) is what retail's TU saw (the 0x005C8459 recipe).
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_DAMAGE
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_DEAD_DECL_ONLY
#include "engine/CThingBaseDef.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCWeapon.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWeaponDef.h"
#include "engine/ThingAlive.h"
void CTCInventoryWeapons::CheckForSoldWieldedWeapons()
{
    std::vector<std::pair<long, long> > items;
    if (IS_THING_DEAD(PActiveMeleeWeapon) || IS_THING_DEAD(PActiveRangedWeapon))
        GetAllItems(items);
    if (IS_THING_DEAD(PActiveMeleeWeapon)) {
        for (unsigned int i = 0; i < items.size(); ++i) {
            if (items[i].second > 0) {
                CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(items[i].first);
                CDefPointer<const CWeaponDef> weapon;
                def->GetSubDef(&weapon);
                if (!weapon.IsNull() && CTCWeapon::IsMeleeWeaponClass(weapon->Class)) {
                    SetThingAsActiveWeapon(items[i].first, 0);
                    break;
                }
            }
        }
    }
    if (IS_THING_DEAD(PActiveRangedWeapon)) {
        for (unsigned int i = 0; i < items.size(); ++i) {
            if (items[i].second > 0) {
                CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(items[i].first);
                CDefPointer<const CWeaponDef> weapon;
                def->GetSubDef(&weapon);
                if (!weapon.IsNull() && !CTCWeapon::IsMeleeWeaponClass(weapon->Class)) {
                    SetThingAsActiveWeapon(items[i].first, 0);
                    break;
                }
            }
        }
    }
}
