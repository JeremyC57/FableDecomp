// CTCInventoryTrade::IsComponentSerialisable() const -- returns false
// Leaf: ?IsComponentSerialisable@CTCInventoryTrade@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B3070, thiscall, `ret`.
// Identity: slot 23 of CTCInventoryTrade's vtable 0x01253034 (defined here: the slot holds this body in 1 vtable(s), all of CTCInventoryTrade or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 32 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCInventoryTrade@@UBE_NXZ; `ret` matches the signature.
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
bool CTCInventoryTrade::IsComponentSerialisable() const
{
    return false;
}
