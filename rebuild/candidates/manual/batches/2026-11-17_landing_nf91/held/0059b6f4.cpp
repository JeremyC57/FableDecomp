// CTCInventoryTrade::GetCategoryIndexForTradeType() -- the trade category index of the screen's CategoryIdent ({11 -> 0, 12 -> 1} from the two-row table at 0x0137CCDC), else -1
// Leaf: ?GetCategoryIndexForTradeType@CTCInventoryTrade@@QAEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059B6F4, 35 bytes; size TU.
//   mov ecx,[ecx+170h] ; xor eax,eax ; L: cmp [eax*8+0137CCDCh],ecx ; je F ; inc eax ; cmp eax,2 ; jl L ; or eax,-1 ; ret ; F: mov eax,[eax*8+0137CCE0h] ; ret
// Callers (E8): its TU-mates AddItemOfTypeToInventory 0x0059C6B0, AddItemToInventory 0x0059C6DD, 0x0059C710, RemoveItemFromInventory 0x0059C747,
//   RemoveAllItemsFromCategory 0x0059CF49.
// Name fix (agentNF91, q11201): Ego_r.pdb + FableWin.pdb ?GetCategoryIndexForTradeType@CTCInventoryTrade@@QAEJXZ (public; engine/CTCInventoryTrade.h
//   TRADECATEGORY declares it); the old row defined ?GetCategoryIndexForTradeType@CTCInventoryTrade_Retail@@QAEJXZ on a local
//   CTCInventoryTrade_Retail view. The body now lives in engine/CTCInventoryTrade.h (NF91 overlay opt-in
//   FABLE_RETAIL_CTCINVENTORYTRADE_TRADECATEGORY_BODY, inline + noinline); this row only takes its address so VC7.1 emits it here as a
//   pick-any COMDAT (selection 2; the nf86 / nf88 scaffolding). q11204 is NOT closed by this: the landed callers 0x0059C6B0 / 0x0059C6DD /
//   0x0059C710 still define strong same-TU copies (0x0059C747 one under the _Retail name), which retail's edx-preserving call needs (see the
//   header note); the table keeps the landed descriptive spelling g_TradeTypeCategories (no PDB, label or FSE name).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_CATEGORYIDENTIFIER
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_TRADECATEGORY
#define FABLE_RETAIL_CTCINVENTORYTRADE_TRADECATEGORY_BODY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryTrade.h"
namespace {
// Fidelity: scaffolding, not retail -- takes the address of the header's inline GetCategoryIndexForTradeType so VC7.1 emits its COMDAT here.
long (CTCInventoryTrade::*EmitGetCategoryIndexForTradeType())()
{
    return &CTCInventoryTrade::GetCategoryIndexForTradeType;
}
}
