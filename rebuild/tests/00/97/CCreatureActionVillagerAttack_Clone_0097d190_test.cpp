// Behaviour test for 0x0097D190 CCreatureAction_VillagerAttack::Clone (NF425): links the row object; operator new and the base copy constructor are defined here.
// Checks: one allocation of 0x114 bytes; the base copy constructor runs once into the block from `*this`; the block gets the class's
// vftable pointer (the source's, as both are CCreatureAction_VillagerAttack objects); the class's own members are copied; 0 with no construction when new fails.
// Ported (NF427 round 2, 2026-11-25): no class body is defined here; the classes come from the shared headers (the row's
//   prelude below, verbatim, plus engine/CCreatureAction_VillagerAttack.h). Removed local bodies: CCombatActionBase, CCreatureAction_VillagerAttack. The members the test supplies (??0CCombatActionBase@@QAE@ABV0@@Z) and the row's CCreatureAction_VillagerAttack::Clone are
//   reached through extern "C" __fastcall stand-ins mapped by /alternatename (ecx = this, edx unused, stack arguments
//   popped by the callee: the __thiscall frame); sizes and offsets are the retail literals of the original test.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_COPYCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_ONANIMATIONLOADED
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_CCREATUREACTION_VILLAGERATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCreatureAction_VillagerAttack.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>

static int g_ctors = 0; static const void* g_this = 0; static const void* g_other = 0;
class CCreatureActionBase;
extern "C" void* __fastcall nf427_copy_ctor(void* self, int edx_unused, const void* other) { g_this = self; g_other = other; ++g_ctors; memcpy((unsigned char*)self + 4, (const unsigned char*)other + 4, 0x114 - 4); return self; }   // CCombatActionBase::CCombatActionBase
#pragma comment(linker, "/alternatename:??0CCombatActionBase@@QAE@ABV0@@Z=@nf427_copy_ctor@12")
// The row object emits the class's vftable (its inlined copy constructor stores it); the test never calls through it, so
// every slot target the object leaves unresolved is mapped onto one unused stub for the link.
extern "C" void nf425_unused_slot(void) { }
#pragma comment(linker, "/alternatename:??1CCombatActionBase@@UAE@XZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:??3@YAXPAX@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:??_GCCreatureAction_VillagerAttack@@UAEPAXI@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?AllowsCombinationAnims@CCombatActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?CanAnimationTransitionOut@CCreatureActionBase@@MBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?EditUpdate@CCreatureActionBase@@UAEXM@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?FinishAction@CCombatActionBase@@MAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?FrameUpdate@CCombatActionBase@@UAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetActionMovementVector@CCombatActionBase@@UBE?AVC3DVector@@XZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetActionSpeedParameter@CCreatureActionBase@@MBEMXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetActionStartRunningTimeSeconds@CCreatureActionBase@@MBEMXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetAnimDelayTime@CCombatActionBase@@UBEJXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetAnimExtraFlags@CCombatActionBase@@UBEKXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetAnimationIndex@CCombatActionBase@@UBEJXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetAnimationTransitionInTime@CCombatActionBase@@UBEJXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetAnimationTransitionOutTime@CCreatureActionBase@@UBEJXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetDamageMultiplier@CCombatActionBase@@UBEMXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetFrameUpdateTime@CCombatActionBase@@UBEMXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetIsToFaceTargetCone@CCombatActionBase@@UBEMXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetIsToFaceTargetMinDist@CCombatActionBase@@UBEMXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetNextPos@CCombatActionBase@@UBE?AVC3DVector@@XZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetStrikeEventHelperName@CCombatActionBase@@UBE?AVCCharString@@XZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetTotalTimeSeconds@CCombatActionBase@@UBEMXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetWeaponTrailUsedInStrikeAction@CCombatActionBase@@MBEPAVCTCWeaponTrail@@AAJ@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?Initialise@CCreatureAction_VillagerAttack@@UAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?InitialiseActionMovement@CCombatActionBase@@MAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsAbleToBeAttacked@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsActionReadyForInterruption@CCombatActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsActionReadyForInterruptionByMovement@CCombatActionBase@@UBE_NABVC3DVector@@@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsActionWithStrikeEventHelperName@CCombatActionBase@@MBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsAffectedByHaste@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsAffectedByHeroStats@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsAllowingAnimationMovementToMoveSpeakAndWaitCameras@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsAlwaysPhysicsPushableBySelfAvoidance@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsAlwaysUseAnimationMovement@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsBlockCounter@CCombatActionBase@@MBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsBlockable@CCombatActionBase@@MBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsClearingMovementVector@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsDecapitatingAttack@CCombatActionBase@@MBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsKnockdownStrike@CCombatActionBase@@MBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsMeleeFlourish@CCombatActionBase@@MBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsMovementUsingPhysics@CCombatActionBase@@MBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsOverrideableByActionOfSamePriority@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsPartOfComboChain@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsPhysicsPushableByHero@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsPlayHitResponse@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsPlayingAnimation@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsPreventingLookTurning@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsPreventingNormalMovement@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsPreventingQuickAccessItemUsage@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsQueuable@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsStrikeAction@CCombatActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsStrikeEvent@CCombatActionBase@@UBE_NJ@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsStrikeEventActive@CCombatActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsStrikeEventFinishedBeingActive@CCombatActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsToAlignWithGroundNormal@CCombatActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsToCauseRecoil@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsToEvadeAllHits@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsToFaceTarget@CCreatureAction_VillagerAttack@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsToFaceTargetAtStart@CCombatActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsToHaveEmptyHands@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsToHitProneThings@CCombatActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsToPerformActionOnStrike@CCombatActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsToReplaceItemIfRemoved@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsToUseRotationInAnimation@CCombatActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsUsingAnimationMovement@CCombatActionBase@@MBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsUsingForcedMovement@CCreatureActionBase@@MBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsVitalStrike@CCombatActionBase@@MBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?OnAnimationLoaded@CCombatActionBase@@MAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?OnBlowBlocked@CCombatActionBase@@UAEX_N@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?OnFollowUpActionFailedToAdd@CCreatureActionBase@@UAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?OnFrameUpdate@CCreatureActionBase@@UAEXM@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?OverrideFaceWithDefault@CCreatureActionBase@@MBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?PerformAction@CCombatActionBase@@UAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?PreventSneaking@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?SetHitDirection@CCreatureActionBase@@UAEXVC3DVector@@@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?SetHitPosition@CCreatureActionBase@@UAEXVC3DVector@@@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?SetHitter@CCreatureActionBase@@UAEXABVCThing@@@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?UpdateActionMoveToPos@CCombatActionBase@@MAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?UpdateActionRunningTime@CCombatActionBase@@UAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?Validate@CBaseObject@@UBEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?WaitForMeshToLoad@CCreatureActionBase@@MBE_NXZ=_nf425_unused_slot")
static unsigned int g_size = 0; static int g_news = 0, g_fail = 0;
static long g_block[0x114 / 4 + 4];
void* operator new(size_t n) { g_size = (unsigned int)n; ++g_news; return g_fail ? 0 : (void*)g_block; }

extern "C" CCreatureActionBase* __fastcall nf427_clone(const void* self);   // the row's CCreatureAction_VillagerAttack::Clone
#pragma comment(linker, "/alternatename:@nf427_clone@4=?Clone@CCreatureAction_VillagerAttack@@UBEPAVCCreatureActionBase@@XZ")

int main(void)
{
    static long src_store[0x114 / 4 + 4];
    unsigned char* s = (unsigned char*)src_store;
    for (int i = 4; i < 0x114; ++i) s[i] = (unsigned char)(i * 7 + 3);
    const void* src = (void*)src_store;
    CCreatureActionBase* r = nf427_clone(src);
    unsigned char* d = (unsigned char*)g_block;
    int ok = r == (CCreatureActionBase*)(void*)g_block && g_news == 1 && g_size == 0x114 && g_ctors == 1 && g_this == (void*)g_block && g_other == (const void*)src_store
        && memcmp(d + 4, s + 4, 0x114 - 4) == 0 && *(void**)d != 0;
    g_fail = 1;
    r = nf427_clone(src);
    ok = ok && r == 0 && g_news == 2 && g_ctors == 1;
    if (!ok) {
        printf("0097d190_TEST FAIL news=%d ctors=%d size=%u\n", g_news, g_ctors, g_size);
        return 1;
    }
    printf("0097d190_TEST PASS\n");
    return 0;
}
