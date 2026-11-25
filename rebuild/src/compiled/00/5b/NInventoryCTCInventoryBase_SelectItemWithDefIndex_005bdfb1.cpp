// NInventory::CTCInventoryBase::SelectItemWithDefIndex(long) -- finds the category of a thing def's inventory item and selects that item
//   type in it
// Leaf: ?SelectItemWithDefIndex@CTCInventoryBase@NInventory@@IAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDFB1, 117 bytes (+2 int3); frame-pointer size TU
// (`#pragma optimize("s", on)`), `ret 4`.
// Identity: Ego_r.pdb (0x00620D0F) + FableWin.pdb ?SelectItemWithDefIndex@CTCInventoryBase@NInventory@@IAEXJ@Z (protected), declared by
//   engine/NInventory_CTCInventoryBase.h FABLE_RETAIL_CTCINVENTORYBASE_SELECTION.
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0 (__fastcall), CDefinitionManager::GetDef<CThingBaseDef> const @ 0x0042AF3C,
//          CDefClassBase::GetSubDef<CInventoryItemDef> const @ 0x005BADE1, STLport find_if<CCategory*, CIsInventoryCategory> @ 0x005C08D3
//          (__fastcall: first in ecx, last in edx, the one-word predicate by value), NInventory::CCategory::SelectItemOfType(long)
//          @ 0x005BDEC1 (Ego_r.pdb / FableWin.pdb, NF93 opt-in FABLE_RETAIL_CCATEGORY_SELECTITEMOFTYPE).
// Port (NF93, q11239): was a local CTCInventoryBase / CCategory / CDefPointer / def model with the find_if stand-in
//   find_if@NInventory(.., CCategoryHasIdentifier); now on the shared headers, the GetNumberOfItemsOfTypeInInventory 0x005BDF08 recipe
//   (the predicate a named object built before the end pointer keeps retail's load order).
#include "fable_compiler.h"
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
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTION
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_CCATEGORY_SELECTITEMOFTYPE
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
void CTCInventoryBase::SelectItemWithDefIndex(long itemDefIndex)
{
    CDefPointer<const CThingBaseDef> def;
    CGameDefinitionManager::Get().GetDef(itemDefIndex, &def);
    CDefPointer<const CInventoryItemDef> itemDef;
    def->GetSubDef(&itemDef);
    CIsInventoryCategory isInventoryCategory(itemDef->InventoryCategory);
    std::vector<CCategory>::iterator last = Categories.end();
    std::vector<CCategory>::iterator category = std::find_if(Categories.begin(), last, isInventoryCategory);
    if (category != last)
        category->SelectItemOfType(itemDefIndex);
}
}
