// CTCInventoryTrade::GetInterfaceType() const -- the component's interface type, TCI_INVENTORY_TRADE (0x17)
// Leaf: ?GetInterfaceType@CTCInventoryTrade@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B304A, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 21 of CTCInventoryTrade's vtable 0x01253034 (the slot holds this body in 1 vtable(s), owner CTCInventoryTrade); the Xbox 360 Test PDB virtual order (third source) aligned on 32 TLC-named slots of that vtable puts GetInterfaceType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetInterfaceType@CTCInventoryTrade@@UBE?AW4ETCInterfaceType@@XZ; `ret` matches the signature.
// Was: CTCInventoryTrade::vfunc_21 (rebuild/src/compiled/00/5b/CTCInventoryTrade_vfunc21_005b304a.cpp).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCINVENTORYTRADE_VSLOTS3
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
ETCInterfaceType CTCInventoryTrade::GetInterfaceType() const
{
    return TCI_INVENTORY_TRADE;
}
