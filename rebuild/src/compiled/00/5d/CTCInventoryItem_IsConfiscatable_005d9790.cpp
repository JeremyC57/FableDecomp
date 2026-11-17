// CTCInventoryItem::IsConfiscatable(long defIndex) -- static: cache the defs for defIndex (CacheDef 0x005D94B0) and return the cached
//   CInventoryItemDef's IsConfiscatable +0x5E, false when the def does not load
// Leaf: ?IsConfiscatable@CTCInventoryItem@@SI_NJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D9790, static `__fastcall` (defIndex in ecx straight on to CacheDef).
// Identity (NF59, round 7, queue q10532): Ego_r.pdb 0x0055A6F0 + FableWin.pdb 0x01E02690 ?IsConfiscatable@CTCInventoryItem@@SI_NJ@Z; the Ego_r / FableWin run of the static
//   accessors (CacheDef .. GetContextSensitiveType) is the retail order 0x005D94B0 .. 0x005D9B90, and the body reads the def field named
//   above (engine/CInventoryItemDef.h). Re-authored on engine/CTCInventoryItem.h: the landed July row was a extern "C" FableIsBuyable_5d9790 / FableCheck_5d9790 (catalog IsBuyable),
//   which bound CacheDef under a made-up symbol (the private static ?CacheDef@CTCInventoryItem@@CI_NJ@Z is reachable only from a member).
// Callees: CTCInventoryItem::CacheDef(long) 0x005D94B0 and the cached CInventoryItemDef 0x013B8C18 (opt-in FABLE_RETAIL_CTCINVENTORYITEM_CACHEDEF).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CACHEDEF
#define FABLE_RETAIL_CTCINVENTORYITEM_ISCONFISCATABLE
#define FABLE_RETAIL_PASS22_INVENTORY_ITEMS_ORIGINAL_ENUM
#define FABLE_RETAIL_CLOTHING_DEFS
#include "fable_compiler.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefPointer.h"
#include "engine/CInventoryItemDef.h"
#include "engine/CTCInventoryItem.h"

bool __fastcall CTCInventoryItem::IsConfiscatable(long defIndex)
{
    if (CacheDef(defIndex))
        return s_CachedItemDef->IsConfiscatable;
    return false;
}
