// CThingManager::GetSerialiseVersion() const -- the SerialiseVersionNumber +0xD8.
// Leaf: ?GetSerialiseVersion@CThingManager@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051E2D0, `mov eax,[ecx+0D8h] ; ret`, the out-of-line copy.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?GetSerialiseVersion@CThingManager@@QBEJXZ; Ego_r.pdb 0x005281F0 donor order
//   GetLevelBeingLoaded / GetSerialiseVersion / IsSerialisingGameState = retail 0x0051E2C0 / 0x0051E2D0 / 0x0051E2E0 (both neighbours
//   landed under those names); CTCBase::GetSerialiseVersion 0x00686A90 tail-calls it (landed, already binding this spelling); it reads
//   SerialiseVersionNumber (+0xD8, engine/CThingManager.h).
// Was: the free stand-in DrawGetEnvironment (rebuild/src/compiled/00/51/global_DrawGetEnvironment_0051e2d0.cpp).
// Header: engine/CThingManager.h (ACCESSORS opt-in, which names 0x0051E2D0), as the sibling 0x0051E2E0.
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_ACCESSORS
#include "engine/CThingManager.h"

long CThingManager::GetSerialiseVersion() const
{
    return SerialiseVersionNumber;
}
