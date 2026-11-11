// CTCCreatureNavigation::HasFrameUpdate() const -- navigation has a frame update
// Leaf: ?HasFrameUpdate@CTCCreatureNavigation@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CD560, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 8 of CTCCreatureNavigation's vtable 0x0125EC24 (the slot holds this body in 1 vtable(s), owner CTCCreatureNavigation); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?HasFrameUpdate@CTCCreatureNavigation@@UBE_NXZ; `ret` matches the signature.
// Was: CTCCreatureNavigation::vfunc_8 (rebuild/src/compiled/00/6c/gapscan_sub006cd560_006cd560.cpp).
#define FABLE_RETAIL_CTCCREATURENAVIGATION_VSLOTS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CNAVIGATORBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#include "engine/ETCInterfaceType.h"
#include "engine/CNavigatorBase.h"
#include "engine/CTCCreatureNavigation.h"
bool CTCCreatureNavigation::HasFrameUpdate() const
{
    return true;
}
