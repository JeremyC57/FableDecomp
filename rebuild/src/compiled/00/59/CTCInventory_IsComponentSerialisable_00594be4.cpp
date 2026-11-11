// CTCInventory::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCInventory@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00594BE4, thiscall, `ret`.
// Identity: slot 23 of CTCInventory's vtable 0x01250DE4 (defined here: the slot holds this body in 1 vtable(s), all of CTCInventory or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 33 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCInventory@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCInventory.h (opt-ins as in the landed row CTCInventory_OpenInventoryShortcut_0058e99f.cpp).
#define FABLE_RETAIL_CTCINVENTORY_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CMenuEntry.h"
bool CTCInventory::IsComponentSerialisable() const
{
    return true;
}
