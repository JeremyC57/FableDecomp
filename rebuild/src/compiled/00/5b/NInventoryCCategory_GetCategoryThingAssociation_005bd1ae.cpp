// NInventory::CCategory::GetCategoryThingAssociation() const -- the thing the category is associated with (PAssociatedThing's item)
// Leaf: ?GetCategoryThingAssociation@CCategory@NInventory@@QBEPAVCThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD1AE, 5 bytes: `jmp CBaseIntelligentPointer::GetPItem` (0x00A01B50) with `this`
//   unchanged (PAssociatedThing is the first member, +0x00).
// Name fix (agentNF89, q11165): Ego_r.pdb (0x006200E1) + FableWin.pdb (0x02362EED) ?GetCategoryThingAssociation@CCategory@NInventory@@QBEPAVCThing@@XZ,
//   the only NInventory::CCategory member of either PDB returning a CThing*; the landed 0x006223C0 binds it. Defined on engine/CCategory.h
//   (PASS22 class, overlay opt-in FABLE_RETAIL_CCATEGORY_CATEGORYTHINGASSOCIATION) through CIntelligentPointer<CThing>::Get (FableWin, inline:
//   GetPItem then a zero-offset static_cast). Was the placeholder _global::TailJmp_GetPItem_005bd1ae (leaf _Forward).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_CATEGORYTHINGASSOCIATION
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/NInventory_CTCInventoryBase.h"
namespace NInventory {
CThing* CCategory::GetCategoryThingAssociation() const
{
    return PAssociatedThing.Get();
}
}
