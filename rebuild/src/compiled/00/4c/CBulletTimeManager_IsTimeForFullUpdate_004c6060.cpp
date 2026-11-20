// CBulletTimeManager::IsTimeForFullUpdate() const -- true on the frames that are a multiple of the (truncated) bullet-time
// slowdown multiplier.
// Retail: Fable.exe @ 0x004C6060, 30 bytes. Ego_r.pdb / FableWin.pdb ?IsTimeForFullUpdate@CBulletTimeManager@@QBE_NXZ.
// Callees: GetBulletTimeSlowdownMultiplier 0x004C5FA0 (?GetBulletTimeSlowdownMultiplier@CBulletTimeManager@@QBEMXZ), _ftol 0x00BFEA70,
//   CWorld::GetFrame 0x0049D870 (static, ?GetFrame@CWorld@@SIJXZ).
// NF108 (2026-11-19, q4760): re-authored on the shared engine/CBulletTimeManager.h as the const member (was an extern "C"
//   __fastcall stand-in).
// Leaf: ?IsTimeForFullUpdate@CBulletTimeManager@@QBE_NXZ
#define FABLE_RETAIL_CBULLETTIMEMANAGER
#define FABLE_RETAIL_CBULLETTIMEMANAGER_SLOWDOWNMULTIPLIER
#include "engine/CBulletTimeManager.h"
#include "engine/CWorld.h"

bool CBulletTimeManager::IsTimeForFullUpdate() const
{
    return CWorld::GetFrame() % (long)GetBulletTimeSlowdownMultiplier() == 0;
}
