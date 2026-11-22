// 006ac200 CThingPlayerCreature::CreateAndAddObjectToInventory
// Leaf: ?CreateAndAddObjectToInventory@CThingPlayerCreature@@QAEPAVCThingObject@@JJ_NABVCCharString@@@Z
// Flags: none
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_CREATEANDADD
#define FABLE_RETAIL_CTCINVENTORYITEM_INVENTORYACCESS
#define FABLE_RETAIL_CTCINVENTORY_BUTTON_INDEX_PASS26
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CDEFSTRING_EQUALS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER_CLASSTYPENAME
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_EXPRESSIONS
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CEXPRESSIONDEF_DEFINDEX
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORY_ASSIGNTHINGPC
#define FABLE_RETAIL_CTCINVENTORY_NEXTFREESLOT
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_ADDTOINVENTORY
#define FABLE_RETAIL_CTCINVENTORYITEM_TUTORIALCATEGORY
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_EXPRESSIONS
#define FABLE_RETAIL_CTCINVENTORYABILITIES_LEARNEDEXPRESSION
#define FABLE_RETAIL_CTCHERO_TUTORIAL
#define FABLE_RETAIL_CTCHERO_TITLE
#include "stlport/fable_containers.h"
#include "engine/fable_nscript_forward.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingObject.h"
#include "engine/CExpressionDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefString.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CTCHeroReceiveItems.h"
CThingObject* CThingPlayerCreature::CreateAndAddObjectToInventory(long def_index, long quick_access_slot,
                                                                  bool silent, const CCharString& script_name)
{
    CThingObject* pobject = CThingObject::Create(def_index, GetPos(), GetPlayerNumber(), 0, 0, script_name);
    if (pobject) {
        CTCInventoryItem* ptc_inventory_item = 0;
        pobject->GetTC(&ptc_inventory_item);
        if (ptc_inventory_item) {
            NInventory::CTCInventoryBase* ptc_inventory = 0;
            GetTC(ptc_inventory_item->GetInventoryType(), &ptc_inventory);
            bool added = false;
            if (silent) {
                bool add_to_quick_access = quick_access_slot != -1;
                added = ptc_inventory_item->AddToInventory(*ptc_inventory, false, add_to_quick_access, 0, silent, false);
            } else {
                CTCHeroReceiveItems* ptc_hero_receive_items = 0;
                GetTC(&ptc_hero_receive_items);
                ptc_hero_receive_items->GiveItem(*pobject, false);
                added = true;
            }
            if (added && quick_access_slot >= 0 && ptc_inventory->GetInterfaceType() == TCI_INVENTORY) {
                CTCInventory* inventory = static_cast<CTCInventory*>(ptc_inventory);
                if (inventory->GetQuickAccessItemButtonIndex(pobject->GetDefGlobalIndex()) == -1)
                    inventory->SetThingAsQuickAccessItem(pobject->GetDefGlobalIndex(), quick_access_slot, false);
            }
        }
    }
    return pobject;
}