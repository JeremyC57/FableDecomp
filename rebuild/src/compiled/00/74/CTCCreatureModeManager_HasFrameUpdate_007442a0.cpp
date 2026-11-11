// CTCCreatureModeManager::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCCreatureModeManager@@MBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007442A0, thiscall, `ret`.
// Identity: slot 8 of CTCCreatureModeManager's vtable 0x01268854 (defined here: the slot holds this body in 1 vtable(s), all of CTCCreatureModeManager or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCCreatureModeManager@@MBE_NXZ; `ret` matches the signature.
// Header: engine/CTCCreatureModeManager.h (opt-ins as in the landed row CModeStandBase_OnDeactivate_00861d00.cpp).
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_DEACTIVATE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
bool CTCCreatureModeManager::HasFrameUpdate() const
{
    return true;
}
