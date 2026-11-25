// Behaviour test for 0x008C9720 CCreatureAction_BanditKingChangeWeapons::Clone (NF425): links the row object; operator new and the base copy constructor are defined here.
// Checks: one allocation of 0xAC bytes; the base copy constructor runs once into the block from `*this`; the block gets the class's
// vftable pointer (the source's, as both are CCreatureAction_BanditKingChangeWeapons objects); the class's own members are copied (base bytes, then each member; padding is not copied); 0 with no construction when new fails.
// Ported (NF427 round 2, 2026-11-25): no class body is defined here; the classes come from the shared headers (the row's
//   prelude below, verbatim, plus engine/CCreatureAction_BanditKingChangeWeapons.h). Removed local bodies: CCreatureActionBase, CCreatureAction_BanditKingChangeWeapons. The members the test supplies (??0CCreatureActionBase@@QAE@ABV0@@Z) and the row's CCreatureAction_BanditKingChangeWeapons::Clone are
//   reached through extern "C" __fastcall stand-ins mapped by /alternatename (ecx = this, edx unused, stack arguments
//   popped by the callee: the __thiscall frame); sizes and offsets are the retail literals of the original test.
#define FABLE_RETAIL_CCREATUREACTIONBASE_COPYCTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_BanditKingChangeWeapons.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>

static int g_ctors = 0; static const void* g_this = 0; static const void* g_other = 0;
class CCreatureActionBase;
extern "C" void* __fastcall nf427_copy_ctor(void* self, int edx_unused, const void* other) { g_this = self; g_other = other; ++g_ctors; memcpy((unsigned char*)self + 4, (const unsigned char*)other + 4, 0xA8 - 4); return self; }   // CCreatureActionBase::CCreatureActionBase
#pragma comment(linker, "/alternatename:??0CCreatureActionBase@@QAE@ABV0@@Z=@nf427_copy_ctor@12")
// The row object emits the class's vftable (its inlined copy constructor stores it); the test never calls through it, so
// every slot target the object leaves unresolved is mapped onto one unused stub for the link.
extern "C" void nf425_unused_slot(void) { }
#pragma comment(linker, "/alternatename:??1CCreatureActionBase@@UAE@XZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:??3@YAXPAX@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:??_GCCreatureAction_BanditKingChangeWeapons@@UAEPAXI@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?AllowsCombinationAnims@CCreatureActionBase@@MBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?CanAnimationTransitionOut@CCreatureActionBase@@MBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?EditUpdate@CCreatureActionBase@@UAEXM@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?FinishAction@CCreatureActionBase@@UAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?FrameUpdate@CCreatureActionBase@@MAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetActionSpeedParameter@CCreatureActionBase@@MBEMXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetActionStartRunningTimeSeconds@CCreatureActionBase@@MBEMXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetAnimDelayTime@CCreatureActionBase@@UBEJXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetAnimExtraFlags@CCreatureActionBase@@UBEKXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetAnimationIndex@CCreatureActionBase@@UBEJXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetAnimationTransitionInTime@CCreatureActionBase@@UBEJXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetAnimationTransitionOutTime@CCreatureActionBase@@UBEJXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetFrameUpdateTime@CCreatureActionBase@@MBEMXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetStrikeEventHelperName@CCreatureActionBase@@UBE?AVCCharString@@XZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetTotalTimeSeconds@CCreatureActionBase@@UBEMXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?GetWeaponTrailUsedInStrikeAction@CCreatureActionBase@@UBEPAVCTCWeaponTrail@@AAJ@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?Initialise@CCreatureActionBase@@UAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?InitialiseActionMovement@CCreatureActionBase@@MAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsAbleToBeAttacked@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsActionReadyForInterruption@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsActionReadyForInterruptionByMovement@CCreatureActionBase@@UBE_NABVC3DVector@@@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsActionWithStrikeEventHelperName@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsAffectedByHaste@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsAffectedByHeroStats@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsAllowingAnimationMovementToMoveSpeakAndWaitCameras@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsAlwaysPhysicsPushableBySelfAvoidance@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsAlwaysUseAnimationMovement@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsBlockCounter@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsBlockable@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsClearingMovementVector@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsDecapitatingAttack@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsKnockdownStrike@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsMeleeFlourish@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsMovementUsingPhysics@CCreatureActionBase@@MBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsOverrideableByActionOfSamePriority@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsPartOfComboChain@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsPhysicsPushableByHero@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsPlayHitResponse@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsPlayingAnimation@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsPreventingLookTurning@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsPreventingNormalMovement@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsPreventingQuickAccessItemUsage@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsQueuable@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsStrikeAction@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsStrikeEvent@CCreatureActionBase@@UBE_NJ@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsStrikeEventActive@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsStrikeEventFinishedBeingActive@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsToCauseRecoil@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsToEvadeAllHits@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsToHaveEmptyHands@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsToReplaceItemIfRemoved@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsUsingAnimationMovement@CCreatureActionBase@@MBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsUsingForcedMovement@CCreatureActionBase@@MBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?IsVitalStrike@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?OnAnimationLoaded@CCreatureActionBase@@MAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?OnBlowBlocked@CCreatureActionBase@@UAEX_N@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?OnFollowUpActionFailedToAdd@CCreatureActionBase@@UAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?OnFrameUpdate@CCreatureActionBase@@UAEXM@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?OverrideFaceWithDefault@CCreatureActionBase@@MBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?PerformAction@CCreatureAction_BanditKingChangeWeapons@@UAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?PreventSneaking@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?SetHitDirection@CCreatureActionBase@@UAEXVC3DVector@@@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?SetHitPosition@CCreatureActionBase@@UAEXVC3DVector@@@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?SetHitter@CCreatureActionBase@@UAEXABVCThing@@@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?UpdateActionMoveToPos@CCreatureActionBase@@MAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?UpdateActionRunningTime@CCreatureActionBase@@UAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?Validate@CBaseObject@@UBEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?WaitForMeshToLoad@CCreatureActionBase@@MBE_NXZ=_nf425_unused_slot")
static unsigned int g_size = 0; static int g_news = 0, g_fail = 0;
static long g_block[0xAC / 4 + 4];
void* operator new(size_t n) { g_size = (unsigned int)n; ++g_news; return g_fail ? 0 : (void*)g_block; }

extern "C" CCreatureActionBase* __fastcall nf427_clone(const void* self);   // the row's CCreatureAction_BanditKingChangeWeapons::Clone
#pragma comment(linker, "/alternatename:@nf427_clone@4=?Clone@CCreatureAction_BanditKingChangeWeapons@@UBEPAVCCreatureActionBase@@XZ")

int main(void)
{
    static long src_store[0xAC / 4 + 4];
    unsigned char* s = (unsigned char*)src_store;
    for (int i = 4; i < 0xAC; ++i) s[i] = (unsigned char)(i * 7 + 3);
    const void* src = (void*)src_store;
    CCreatureActionBase* r = nf427_clone(src);
    unsigned char* d = (unsigned char*)g_block;
    int ok = r == (CCreatureActionBase*)(void*)g_block && g_news == 1 && g_size == 0xAC && g_ctors == 1 && g_this == (void*)g_block && g_other == (const void*)src_store
        && memcmp(d + 4, s + 4, 0xA8 - 4) == 0 && memcmp(d + 0xA8, s + 0xA8, 1) == 0 && *(void**)d != 0;
    g_fail = 1;
    r = nf427_clone(src);
    ok = ok && r == 0 && g_news == 2 && g_ctors == 1;
    if (!ok) {
        printf("008c9720_TEST FAIL news=%d ctors=%d size=%u\n", g_news, g_ctors, g_size);
        return 1;
    }
    printf("008c9720_TEST PASS\n");
    return 0;
}
