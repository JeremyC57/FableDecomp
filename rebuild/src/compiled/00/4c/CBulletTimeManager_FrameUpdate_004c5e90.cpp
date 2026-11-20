// CBulletTimeManager::FrameUpdate() -- an empty per-frame update (a lone `ret` in retail).
// Retail: Fable.exe @ 0x004C5E90, 1 byte (`ret`). Ego_r.pdb / FableWin.pdb ?FrameUpdate@CBulletTimeManager@@QAEXXZ; called by
//   CWorld::UpdateFrame 0x004A5A40 (landed, binds this spelling).
// NF108 (2026-11-19, q4759): re-authored on the shared engine/CBulletTimeManager.h (was a stand-in C004c5e90::M).
// Leaf: ?FrameUpdate@CBulletTimeManager@@QAEXXZ
#define FABLE_RETAIL_CBULLETTIMEMANAGER
#include "engine/CBulletTimeManager.h"

void CBulletTimeManager::FrameUpdate()
{
}
