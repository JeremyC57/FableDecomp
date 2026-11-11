// CTCInventoryExperience::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCInventoryExperience@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057A25D, thiscall, `ret`.
// Identity: slot 23 of CTCInventoryExperience's vtable 0x0124F11C (defined here: the slot holds this body in 1 vtable(s), all of CTCInventoryExperience or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCInventoryExperience@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCInventoryExperience.h (opt-ins as in the landed row CTCInventoryExperience_UpdateMenuScrollButtonsItem_0056ef2d.cpp).
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_VSLOTS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_MENULIST
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_VIRTUALS
#include "engine/CTCInventoryExperience.h"
bool CTCInventoryExperience::IsComponentSerialisable() const
{
    return true;
}
