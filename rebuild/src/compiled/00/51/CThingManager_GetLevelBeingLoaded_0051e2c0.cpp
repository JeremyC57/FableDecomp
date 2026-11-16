// CThingManager::GetLevelBeingLoaded() const -- LevelBeingLoaded +0xDC (`mov eax,[ecx+0DCh] ; ret`), the out-of-line copy.
// Leaf: ?GetLevelBeingLoaded@CThingManager@@QBEJXZ
// NF37 (2026-11-16): moved off the free DrawGetEnvironment stand-in onto the shared engine/CThingManager.h (LOADSINGLE +
// LEVELBEINGLOADED_OUTOFLINE, which names 0x0051E2C0; Ego_r.pdb 0x005281F0 donor order; Ego_r.pdb / FableWin.pdb ?GetLevelBeingLoaded@CThingManager@@QBEJXZ).
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_LOADSINGLE
#define FABLE_RETAIL_CTHINGMANAGER_LEVELBEINGLOADED_OUTOFLINE
#include "engine/CThingManager.h"

long CThingManager::GetLevelBeingLoaded() const
{
    return LevelBeingLoaded;
}
