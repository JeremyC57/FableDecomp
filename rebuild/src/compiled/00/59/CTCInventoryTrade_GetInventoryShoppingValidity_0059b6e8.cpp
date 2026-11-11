// CTCInventoryTrade::GetInventoryShoppingValidity() -- the trade screen's shopping validity is the inventory base's
// Leaf: ?GetInventoryShoppingValidity@CTCInventoryTrade@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059B6E8, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 48 of CTCInventoryTrade's vtable 0x01253034 (the slot holds this body in 1 vtable(s), owner CTCInventoryTrade); the Xbox 360 Test PDB virtual order (third source) aligned on 32 TLC-named slots of that vtable puts GetInventoryShoppingValidity(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetInventoryShoppingValidity@CTCInventoryTrade@@UAE_NXZ; no `ret` to check (NORET).
// Was: CTCInventoryTrade::vfunc_48 (rebuild/src/compiled/00/59/gapscan_sub0059b6e8_0059b6e8.cpp).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCINVENTORYTRADE_VSLOTS3
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_PCINVENTORY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
bool CTCInventoryTrade::GetInventoryShoppingValidity()
{
    return NInventory::CTCInventoryBase::GetInventoryShoppingValidity();
}
