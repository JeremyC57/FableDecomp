// CTCDriverBase::CreateAdditionalTCs() -- does nothing
// Leaf: ?CreateAdditionalTCs@CTCDriverBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3F2B, thiscall, `ret`.
// Identity: slot 29 of CTCDriverBase's vtable 0x0126A79C (defined here: the slot holds this body in 9 vtable(s), all of CTCDriverBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts CreateAdditionalTCs(void) at this slot; Ego_r/FableWin name set: ?CreateAdditionalTCs@CTCDriverBase@@UAEXXZ; `ret` matches the signature.
// Header: engine/CTCDriverBase.h (opt-ins as in the landed row CTCDExperienceOrb_CTCDExperienceOrb_00779cd0.cpp).
#define FABLE_RETAIL_CTCDRIVERBASE_VSLOTS
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCDriverBase.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RESET
#include "engine/CDefPointer.h"
#include "engine/CSubDefClassBase.h"
void CTCDriverBase::CreateAdditionalTCs()
{
}
