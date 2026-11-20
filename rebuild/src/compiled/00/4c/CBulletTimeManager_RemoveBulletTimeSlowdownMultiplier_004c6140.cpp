// CBulletTimeManager::RemoveBulletTimeSlowdownMultiplier(void* key) -- erase the key's slowdown entry from the bullet-time map; when no entry is left, fall back to the default multiplier; then recalculate the sound pitch
// Leaf: ?RemoveBulletTimeSlowdownMultiplier@CBulletTimeManager@@QAEXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C6140, length 35 bytes (+13 int3 padding); speed-optimised TU.
// Identity: Ego_r.pdb 0x004C8B80 + FableWin.pdb 0x01D28090 ?RemoveBulletTimeSlowdownMultiplier@CBulletTimeManager@@QAEXPAX@Z; the landed
//   0x00859660 CCombatActionBase::ResetMeleeCombatAbilityEffects calls it on GetWorld().GetBulletTimeManager() (+0x68) with `this` (RELOCATION_MATCH).
// Callees: BulletTimeInfoMap's tree erase(const key&) @ 0x004C6B60 (equal_range + range erase, count ignored) and
//   ?RecalculateSoundPitch@CBulletTimeManager@@AAEXXZ @ 0x004C5F10 (Ego_r.pdb, private; landed).
// Layout: engine/CBulletTimeManager.h (Ego_r.pdb): BulletTimeInfoMap map<void*, CBulletTimeInfo> +0x00 (STLport, node count +4),
//   UseDefaultMultiplier +0x0D.
// Shape: the key is passed by reference to the by-value parameter's slot (`lea eax,[esp+8] ; push eax`); the count is read through
//   the inline empty() (`mov eax,[esi+4] ; test eax,eax`).
// NF108 (2026-11-19, q5099): re-authored on the shared header (was the descriptive CTimeMultiplierController_004c6140::RemoveRequest
//   over a local CMultiplierRequestMap stand-in).
#define FABLE_RETAIL_STLP_MAP_EMPTY
#define FABLE_RETAIL_STLP_SET_EMPTY
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#define FABLE_RETAIL_CBULLETTIMEMANAGER
#define FABLE_RETAIL_CBULLETTIMEMANAGER_INTERPOLATE
#define FABLE_RETAIL_CBULLETTIMEMANAGER_REMOVESLOWDOWN
#define FABLE_RETAIL_CBULLETTIMEMANAGER_RECALCULATESOUNDPITCH
#include "engine/CBulletTimeManager.h"

void CBulletTimeManager::RemoveBulletTimeSlowdownMultiplier(void* key)
{
    BulletTimeInfoMap.erase(key);
    if (BulletTimeInfoMap.empty())
        UseDefaultMultiplier = true;
    RecalculateSoundPitch();
}
