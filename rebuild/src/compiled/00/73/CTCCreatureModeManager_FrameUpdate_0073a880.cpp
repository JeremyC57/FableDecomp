// CTCCreatureModeManager::FrameUpdate() -- frame-updates the current (front) creature mode, if any
// Leaf: ?FrameUpdate@CTCCreatureModeManager@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073A880, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 9 of CTCCreatureModeManager's vtable 0x01268854 (the slot holds this body in 1 vtable(s), owner CTCCreatureModeManager); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts FrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?FrameUpdate@CTCCreatureModeManager@@MAEXXZ; `ret` matches the signature.
// Was: CTCCreatureModeManager::vfunc_9 (rebuild/src/compiled/00/73/CTCCreatureModeManager_vfunc9_0073a880.cpp).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_FRAMEUPDATE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"
void CTCCreatureModeManager::FrameUpdate()
{
    if (!PModes.empty())
        PModes.front()->FrameUpdate();
}
