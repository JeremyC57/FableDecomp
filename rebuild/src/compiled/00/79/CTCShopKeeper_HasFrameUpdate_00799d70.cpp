// CTCShopKeeper::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCShopKeeper@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00799D70, thiscall, `ret`.
// Identity: slot 8 of CTCShopKeeper's vtable 0x0126C154 (defined here: the slot holds this body in 1 vtable(s), all of CTCShopKeeper or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCShopKeeper@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCShopKeeper.h (opt-ins as in the landed row CTCShopKeeper_GetCustomer_00798d80.cpp).
#define FABLE_RETAIL_CTCSHOPKEEPER_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CTCShopKeeper.h"
bool CTCShopKeeper::HasFrameUpdate() const
{
    return true;
}
