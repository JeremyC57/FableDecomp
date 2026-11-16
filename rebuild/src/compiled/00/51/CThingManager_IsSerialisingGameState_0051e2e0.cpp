// CThingManager::IsSerialisingGameState() const -- the SerialisingGameState flag +0x9A.
// Leaf: ?IsSerialisingGameState@CThingManager@@QBE_NXZ
// NF37 (2026-11-16): moved off the CTCInventoryBase_GetCanItemBeQuickAssigned free stand-in onto the shared engine/CThingManager.h (ACCESSORS
// opt-in, which names 0x0051E2E0; Ego_r.pdb 0x005281F0 donor order GetLevelBeingLoaded / GetSerialiseVersion / IsSerialisingGameState =
// retail 0x0051E2C0 / .. / 0x0051E2E0; Ego_r.pdb / FableWin.pdb ?IsSerialisingGameState@CThingManager@@QBE_NXZ).
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_ACCESSORS
#include "engine/CThingManager.h"

bool CThingManager::IsSerialisingGameState() const
{
    return SerialisingGameState;
}
