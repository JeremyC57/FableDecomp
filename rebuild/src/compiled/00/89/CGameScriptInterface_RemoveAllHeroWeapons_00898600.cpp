// CGameScriptInterface::RemoveAllHeroWeapons() const -- snapshot the hero's weapon-inventory entries, remove each, then remove the confiscated items
// Leaf: ?RemoveAllHeroWeapons@CGameScriptInterface@@UBEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00898600, 205 bytes (vtable slot 135 of CGameScriptInterface).
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0, the non-const CVectorMap LowerBound 0x0040F020
//   (the inline GetTC; the bitset test is inline), NInventory::CTCInventoryBase::GetAllItems 0x005BF482, virtual RemoveItemFromInventory (slot 79)
//   and RemoveAllConfiscatedItems (slot 74), the CRT free 0x00BFEA14 (the item vector's storage).
// Name fix (agentNF89, q11171): Ego_r.pdb + FableWin.pdb ?RemoveAllHeroWeapons@CGameScriptInterface@@UBEXXZ (engine/CGameScriptInterface.h slot 135);
//   0x005BF482 is ?GetAllItems@CTCInventoryBase@NInventory@@QBEXAAV?$vector@U?$pair@JJ@std@@..@std@@@Z. Moved off the row-local
//   CGameScriptInterfaceMethods / CThingNative / CPlayer (renaming macro) / CTCInventoryWeaponsNative / vtable-union / item-vector models onto the
//   shared headers (the 0x0089FEB0 recipe for the hero; NInventory_CTCInventoryBase.h VTABLE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::RemoveAllHeroWeapons() const
{
    CThing* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    NInventory::CTCInventoryBase* inventory;
    if (hero && !hero->IsDead() && hero->GetTC(TCI_INVENTORY_WEAPONS, &inventory)) {
        std::vector<std::pair<long, long> > items;
        inventory->GetAllItems(items);
        for (std::vector<std::pair<long, long> >::iterator it = items.begin(); it != items.end(); ++it)
            inventory->RemoveItemFromInventory(it->first, it->second);
        inventory->RemoveAllConfiscatedItems();
    }
}
