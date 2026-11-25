// Behaviour test for 0x006F8280 CCreatureAction_CastWillDrainLifeSpell::Clone (NF425; NF427 round 2: on the shared headers): links the row object.
// Classes: the row's prelude below (its #defines and #includes) gives CCombatActionBase from engine/CCombatActionBase.h; CCreatureAction_CastWillDrainLifeSpell is the row's own
//   class (no engine/CCreatureAction_CastWillDrainLifeSpell.h; ruling q1), so the test names it only through its Clone symbol and the retail allocation size 0x11C; no class body is defined here.
// Reached without a class body: the base copy constructor ??0CCombatActionBase@@QAE@ABV0@@Z and the row's virtual Clone are mapped by
//   /alternatename onto extern "C" __fastcall stand-ins (ecx = this, edx unused, stack arguments popped by the callee: the
//   __thiscall frame); operator new is defined here.
// Checks: one allocation of 0x11C bytes; the base copy constructor runs once into the block from the source; the block gets the
//   class's vftable pointer; the base part after the vptr equals the source's (by the constructor) and each own member (Level, DrainDefIndex) is
//   copied by the inlined copy (padding is not copied); 0 with no construction when new fails.
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
#include <stdio.h>
#include <string.h>
#include <stddef.h>

static int g_ctors = 0; static const void* g_this = 0; static const void* g_other = 0;
extern "C" void* __fastcall nf427_base_copy(void* self, int edx_unused, const void* other)
{
    g_this = self; g_other = other; ++g_ctors;
    memcpy((unsigned char*)self + 4, (const unsigned char*)other + 4, sizeof(CCombatActionBase) - 4);
    return self;
}
#pragma comment(linker, "/alternatename:??0CCombatActionBase@@QAE@ABV0@@Z=@nf427_base_copy@12")
extern "C" CCreatureActionBase* __fastcall nf427_clone(const void* self);   // the row's Clone
#pragma comment(linker, "/alternatename:@nf427_clone@4=?Clone@CCreatureAction_CastWillDrainLifeSpell@@UBEPAVCCreatureActionBase@@XZ")
// The row object emits the class's vftable (its inlined copy constructor stores it); the test never calls through it, so
// every slot target the object leaves unresolved is mapped onto one unused stub for the link.
extern "C" void nf425_unused_slot(void) { }
#pragma comment(linker, "/alternatename:??1CCombatActionBase@@UAE@XZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:??3@YAXPAX@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:??_GCCreatureAction_CastWillDrainLifeSpell@@UAEPAXI@Z=_nf425_unused_slot")
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
#pragma comment(linker, "/alternatename:?Initialise@CCreatureAction_CastWillDrainLifeSpell@@UAEXXZ=_nf425_unused_slot")
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
#pragma comment(linker, "/alternatename:?IsToFaceTarget@CCombatActionBase@@UBE_NXZ=_nf425_unused_slot")
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
#pragma comment(linker, "/alternatename:?PerformAction@CCreatureAction_CastWillDrainLifeSpell@@UAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?PreventSneaking@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?SetHitDirection@CCreatureActionBase@@UAEXVC3DVector@@@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?SetHitPosition@CCreatureActionBase@@UAEXVC3DVector@@@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?SetHitter@CCreatureActionBase@@UAEXABVCThing@@@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?UpdateActionMoveToPos@CCombatActionBase@@MAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?UpdateActionRunningTime@CCombatActionBase@@UAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?Validate@CBaseObject@@UBEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?WaitForMeshToLoad@CCreatureActionBase@@MBE_NXZ=_nf425_unused_slot")
static unsigned int g_size = 0; static int g_news = 0, g_fail = 0;
static long g_block[0x11C / 4 + 4];
void* operator new(size_t n) { g_size = (unsigned int)n; ++g_news; return g_fail ? 0 : (void*)g_block; }

int main(void)
{
    static long src_store[0x11C / 4 + 4];
    unsigned char* s = (unsigned char*)src_store;
    for (int i = 4; i < (int)0x11C; ++i) s[i] = (unsigned char)(i * 7 + 3);
    CCreatureActionBase* r = nf427_clone(src_store);
    unsigned char* d = (unsigned char*)g_block;
    int ok = r == (CCreatureActionBase*)(void*)g_block && g_news == 1 && g_size == 0x11C && g_ctors == 1 && g_this == (void*)g_block
        && g_other == (const void*)src_store && memcmp(d + 4, s + 4, sizeof(CCombatActionBase) - 4) == 0 && *(void**)d != 0
        && memcmp(d + 0x114, s + 0x114, 4) == 0   // Level
        && memcmp(d + 0x118, s + 0x118, 4) == 0;   // DrainDefIndex
    g_fail = 1;
    r = nf427_clone(src_store);
    ok = ok && r == 0 && g_news == 2 && g_ctors == 1;
    if (!ok) {
        printf("006f8280_TEST FAIL news=%d ctors=%d size=%u\n", g_news, g_ctors, g_size);
        return 1;
    }
    printf("006f8280_TEST PASS\n");
    return 0;
}
