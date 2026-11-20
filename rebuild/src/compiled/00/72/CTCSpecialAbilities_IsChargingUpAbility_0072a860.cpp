// CTCSpecialAbilities::IsChargingUpAbility() const -- true while an ability holds the ability mutex (the charge-up phase of the hero's
//   current special ability)
// Leaf: ?IsChargingUpAbility@CTCSpecialAbilities@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072A860, 9 bytes: `mov al,[ecx+0Ch] ; test al,al ; setne al ; ret` (AbilityMutexLock +0x0C).
// Name (NF118, PRIORITY q6369 / q6388; the queue's GetAbilityMutexLock is rejected, REJECTED.tsv): Ego_r.pdb 0x00741700 / FableWin.pdb
//   0x0254F790 ?IsChargingUpAbility@CTCSpecialAbilities@@QBE_NXZ. The Ego_r donor run IsAbleToCastRaiseDead 0x007416D0 (private) /
//   AddHarvestedSoul 0x007416E0 / SetupRaiseDeadSpell 0x007416F0 (private) / IsChargingUpAbility 0x00741700 is retail's TU head:
//   0x0072A830 `xor al,al ; ret`, 0x0072A840 `ret` (called by CCreatureAction_CastWillHarvestSoulSpell::PerformAction 0x008BA970),
//   0x0072A850 `xor al,al ; ret`, then this body; the other const bool getter of the run is private and these callers are other classes. Ego_d.pdb
//   carves it as a tc_special_abilities.obj member (a .cpp body), while GetAbilityMutexLock is a header inline (Ego_d COMDAT under
//   tc_hero_stats.obj; FableWin only, at a mid-function address; expanded in place at 0x0062C001). Callers: the will-charge-up watcher
//   CInputProcessWatchForWillChargeUpThirdPersonModeTermination::Update 0x0068F2A0 (holds the mode while it is true),
//   CInputProcessSpecialAbilities::ProcessInput 0x00690030 (no ability cycling while charging) and CModeStrafe::AnimUpdate 0x008E9B70
//   (before GetCurrentAbilityStrafeAnim 0x0072B870; FableWin places the two members together). Was the generated
//   CASuspendableProcess::CheckTermination (unsigned char) on a padded local class.
// Inferred: the if / return true / return false form (retail normalises the byte with test / setne; a plain `return AbilityMutexLock;`
//   emits `mov al,[ecx+0Ch] ; ret`, and `!= false` adds `xor eax,eax`).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCSPECIALABILITIES_ISCHARGINGUPABILITY
#include "fable_string.h"
#include "engine/CTCSpecialAbilities.h"
bool CTCSpecialAbilities::IsChargingUpAbility() const
{
    if (AbilityMutexLock)
        return true;
    return false;
}
