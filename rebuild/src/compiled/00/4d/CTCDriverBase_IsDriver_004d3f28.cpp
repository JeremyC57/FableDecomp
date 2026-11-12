// CTCDriverBase::IsDriver() const -- returns true
// Leaf: ?IsDriver@CTCDriverBase@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3F28, thiscall, `ret`.
// Identity: slot 24 of CTCDExperienceOrb's vtable 0x0126ABFC (defined here: the slot holds this body in 14 vtable(s), all of CTCDriverBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts IsDriver(void) at this slot; Ego_r/FableWin name set: ?IsDriver@CTCDriverBase@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCDriverBase.h (opt-ins as in the landed row CTCDExperienceOrb_CTCDExperienceOrb_00779cd0.cpp).
#define FABLE_RETAIL_CTCDRIVERBASE_VSLOTS
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCDriverBase.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RESET
#include "engine/CDefPointer.h"
#include "engine/CSubDefClassBase.h"
bool CTCDriverBase::IsDriver() const
{
    return true;
}
