// CTCEntityEvents::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCEntityEvents@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E3D12, thiscall, `ret`.
// Identity: slot 8 of CTCEntityEvents's vtable 0x01242FDC (defined here: the slot holds this body in 1 vtable(s), all of CTCEntityEvents or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCEntityEvents@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCEntityEvents.h (opt-ins as in the landed row CTCEntityEvents_FrameUpdate_00811450.cpp).
#define FABLE_RETAIL_CTCENTITYEVENTS_VSLOTS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_FRAMEUPDATE
#define FABLE_RETAIL_STLP_LIST_REMOVE_IF_OUTOFLINE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CWorld.h"
bool CTCEntityEvents::HasFrameUpdate() const
{
    return true;
}
