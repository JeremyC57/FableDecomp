// CTCInventoryItem::GetTutorialCategory(long defIndex) -- static: cache the defs for defIndex (CacheDef 0x005D94B0) and return the cached
//   CInventoryItemDef's TutorialCategory +0x68 (ETutorialCategory), (ETutorialCategory)0 when the def does not load
// Leaf: ?GetTutorialCategory@CTCInventoryItem@@SI?AW4ETutorialCategory@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D9B70, static `__fastcall` (defIndex in ecx straight on to CacheDef).
// Identity (NF59, round 7, queue q10532): Ego_r.pdb 0x0055AB40 + FableWin.pdb 0x01E03E00 ?GetTutorialCategory@CTCInventoryItem@@SI?AW4ETutorialCategory@@J@Z; the Ego_r / FableWin run of the static
//   accessors (CacheDef .. GetContextSensitiveType) is the retail order 0x005D94B0 .. 0x005D9B90, and the body reads the def field named
//   above (engine/CInventoryItemDef.h). Re-authored on engine/CTCInventoryItem.h: the landed July row was a extern "C" FableGetInv_5d9b70 / FableCheck_5d9b70 (catalog GetInventoryCategory),
//   which bound CacheDef under a made-up symbol (the private static ?CacheDef@CTCInventoryItem@@CI_NJ@Z is reachable only from a member).
// Callees: CTCInventoryItem::CacheDef(long) 0x005D94B0 and the cached CInventoryItemDef 0x013B8C18 (opt-in FABLE_RETAIL_CTCINVENTORYITEM_CACHEDEF).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CACHEDEF
#define FABLE_RETAIL_CTCINVENTORYITEM_GETTUTORIALCATEGORY_STATIC
#define FABLE_RETAIL_PASS22_INVENTORY_ITEMS_ORIGINAL_ENUM
#define FABLE_RETAIL_CLOTHING_DEFS
#include "fable_compiler.h"
#include "engine/ETCInterfaceType.h"
enum ETutorialCategory;                                   // MS forward enum (the PDBs keep no enumerator spellings)
#include "engine/CDefPointer.h"
#include "engine/CInventoryItemDef.h"
#include "engine/CTCInventoryItem.h"

ETutorialCategory __fastcall CTCInventoryItem::GetTutorialCategory(long defIndex)
{
    if (CacheDef(defIndex))
        return *(const ETutorialCategory*)s_CachedItemDef->TutorialCategory;
    return (ETutorialCategory)0;
}
