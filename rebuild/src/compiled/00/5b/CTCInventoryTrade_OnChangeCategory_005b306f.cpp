// CTCInventoryTrade::OnChangeCategory() -- does nothing
// Leaf: ?OnChangeCategory@CTCInventoryTrade@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B306F, thiscall, `ret`.
// Identity: slot 37 of CTCInventoryTrade's vtable 0x01253034 (defined here: the slot holds this body in 1 vtable(s), all of CTCInventoryTrade or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 32 TLC-named slots of that vtable puts OnChangeCategory(void) at this slot; Ego_r/FableWin name set: ?OnChangeCategory@CTCInventoryTrade@@UAEXXZ; `ret` matches the signature.
// Header: engine/CTCInventoryTrade.h (opt-ins as in the landed row CTCInventoryTrade_AttachToFrame_0059bb1b.cpp).
#define FABLE_RETAIL_CTCINVENTORYTRADE_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_PCINVENTORY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_enums.h"
void CTCInventoryTrade::OnChangeCategory()
{
}
