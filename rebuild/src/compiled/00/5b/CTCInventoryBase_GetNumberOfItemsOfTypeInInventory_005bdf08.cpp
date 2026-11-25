// CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long itemType) -- how many items of the definition `itemType` the inventory holds: looks the definition up, takes its inventory-item sub-definition, finds the category that holds that item category and asks it; 0 when any step fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDF08, length 169 bytes. Frame-pointer size TU: optimize("s", on).
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0 (`__fastcall`, no arguments), CDefinitionManager::GetDef<CThingBaseDef>(long,
// CDefPointer<const CThingBaseDef>*) const @ 0x0042AF3C (FableWin.pdb template), CDefClassBase::GetSubDef<CInventoryItemDef>(CDefPointer<
// const CInventoryItemDef>*) const @ 0x005BADE1 (catalog GetSubDef<CInventoryItemDef> --
// its InventoryCategory at +0x3C is what the category search keys on), the SGI find_if over Categories @ 0x005C08C0 (`__fastcall`
// /Gr template: first in ecx, last in edx, the "GlobalDefIndex == x" predicate's long on the stack; the landed GetBoughtPrice model),
// CCategory::GetNumberOfItemsOfTypeInCategory(long) @ 0x005BDE96.
// Layout: retail CTCInventoryBase Categories {first, last, end} at +0x20 (PDB +0x24, the Components vector before it is 12 bytes).
// Shape: the two definition pointers are zero-initialised out slots ([ebp-4], [ebp-8]) filled by the out-parameter template calls (the
// bool results are ignored); with GetDef's body visible (noinline, FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY) and GetSubDef's
// (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE), VC7.1 knows neither handle escapes, so the destructors release without a null test, in
// reverse order, and cross-jump the not-found and no-sub-def tails (grade 4 without the GetDef body: 191 bytes, null-tested release). The
// predicate is a named object built before the end pointer (retail reads InventoryCategory first, then the end, a named local in ebx that
// feeds both the search and the "not found" compare; NF88 grades 5..7).
// Leaf: ?GetNumberOfItemsOfTypeInInventory@CTCInventoryBase@NInventory@@QBEJJ@Z
// NF113 (q5104): Leaf line made exact (it carried a trailing "(Ego_r.pdb; unchanged)" note, so the grader matched no symbol and graded the largest function); body unchanged.
// Port (NF88, round 7, q11159 binder: it bound 0x005C08C0 as the free stand-in ?FindCategoryWithDefIndex@NInventory@@YIPAVCCategory@1@PAV21@0J@Z):
//   the local CDefPointeeBase / CDefPointer / CDefClassBase / CThingBaseDef / CInventoryItemDef / CDefinitionManager / CGameDefinitionManager /
//   NInventory::CCategory / CTCInventoryBase models and the same-TU stand-in GetDef / GetSubDef bodies are gone: the row is on the def headers
//   (engine/CDefPointer.h, CDefinitionManager.h, CGameDefinitionManager.h, CDefClassBase_GetSubDef.h NOINLINE, CThingBaseDef.h,
//   CInventoryItemDef.h), engine/NInventory_CTCInventoryBase.h (Categories typed by ALL_ITEMS_CHAIN_PASS22), engine/CCategory.h (NF88 opt-in
//   FABLE_RETAIL_CCATEGORY_ITEMQUERIES: CIsInventoryCategory and GetNumberOfItemsOfTypeInCategory) and stlport/fable_algorithm.h (find_if declared).
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD   // NF346: GetDef<T> copies = retail (the re-reading SetDynamicCast, owner 0x0066C340; as 0x0066B370)
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CLOTHING_DEFS
#define FABLE_RETAIL_CINVENTORYITEMDEF_STATGETTYPENAME
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_CALLED_PASS23
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CInventoryItemDef.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "stlport/fable_algorithm.h"

namespace NInventory {
long CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long itemDefIndex) const
{
    if (itemDefIndex > 0) {
        CDefPointer<const CThingBaseDef> def;
        CGameDefinitionManager::Get().GetDef(itemDefIndex, &def);
        if (!def.IsNull()) {
            CDefPointer<const CInventoryItemDef> itemDef;
            def->GetSubDef(&itemDef);
            if (!itemDef.IsNull()) {
                CIsInventoryCategory isInventoryCategory(itemDef->InventoryCategory);
                std::vector<CCategory>::const_iterator last = Categories.end();
                std::vector<CCategory>::const_iterator category = std::find_if(Categories.begin(), last, isInventoryCategory);
                if (category != last)
                    return category->GetNumberOfItemsOfTypeInCategory(itemDefIndex);
            }
        }
    }
    return 0;
}
}
