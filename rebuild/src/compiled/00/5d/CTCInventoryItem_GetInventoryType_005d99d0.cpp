// CTCInventoryItem::GetInventoryType(long defIndex) -- static: cache the defs for defIndex and return the TC interface of the inventory component that holds the item (the def's InventoryType), TCI_NULL when the def does not load
// Leaf: ?GetInventoryType@CTCInventoryItem@@SI?AW4ETCInterfaceType@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D99D0, 21 bytes, static `__fastcall` (defIndex in ecx straight on to CacheDef), `ret`:
//   `call CacheDef ; test al,al ; je ; mov eax,[0x013B8C18] ; mov eax,[eax+0x50] ; ret`.
// Identity (NF56, round 7, queue q10471): Ego_r.pdb 0x0055A9A0 + FableWin.pdb 0x01E03620 ?GetInventoryType@CTCInventoryItem@@SI?AW4ETCInterfaceType@@J@Z; engine/CTCInventoryItem.h (FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY) declares it at this
//   address. Re-authored: the landed July row was a free stand-in (GetInventoryCategory / FableGetInv) over made-up globals.
// Callees: CTCInventoryItem::CacheDef(long) 0x005D94B0 and the cached CInventoryItemDef 0x013B8C18 (overlay opt-in
//   FABLE_RETAIL_CTCINVENTORYITEM_CACHEDEF), its InventoryType field +0x50 (engine/CInventoryItemDef.h).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CTCINVENTORYITEM_CACHEDEF
#define FABLE_RETAIL_PASS22_INVENTORY_ITEMS_ORIGINAL_ENUM
#define FABLE_RETAIL_CLOTHING_DEFS
#include "fable_compiler.h"
#include "engine/ETCInterfaceType.h"
#include "engine/EUIInventoryCategory.h"
#include "engine/CDefPointer.h"
#include "engine/CInventoryItemDef.h"
#include "engine/CTCInventoryItem.h"

ETCInterfaceType __fastcall CTCInventoryItem::GetInventoryType(long defIndex)
{
    if (CacheDef(defIndex))
        return *(const ETCInterfaceType*)s_CachedItemDef->InventoryType;
    return TCI_NULL;
}
