// Behaviour test for 0x0084C800 CCreatureAction_AlertAllNearbyToThing::Clone (NF425): links the row object; operator new and the base copy constructor are defined here.
// Checks: one allocation of 0xB0 bytes; the base copy constructor runs once into the block from `*this`; the block gets the class's
// vftable pointer (the source's, as both are CCreatureAction_AlertAllNearbyToThing objects); the class's own members are copied (base bytes, then each member; padding is not copied); 0 with no construction when new fails.
#include <stdio.h>
#include <string.h>
#include <stddef.h>

static int g_ctors = 0; static const void* g_this = 0; static const void* g_other = 0;
class CThing;
class CCreatureActionBase;
class CCreatureActionBase
{
public:
    CCreatureActionBase(const CCreatureActionBase& other);
    void* vptr_;
    unsigned char _base[0xA8 - 4];
};
CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase& other) { g_this = this; g_other = &other; ++g_ctors; memcpy(_base, other._base, 0xA8 - 4); }
class CCreatureAction_AlertAllNearbyToThing : public CCreatureActionBase
{
    friend int main(void);
public:
public:
    virtual CCreatureActionBase* Clone() const;   // defined in the row object
public:
    long Radius;
    CThing * Thing;
};
// The row object emits the class's vftable (its inlined copy constructor stores it); the test never calls through it, so
// every slot target the object leaves unresolved is mapped onto one unused stub for the link.
extern "C" void nf425_unused_slot(void) { }
#pragma comment(linker, "/alternatename:??1CCreatureActionBase@@UAE@XZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:??3@YAXPAX@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:??_GCCreatureAction_AlertAllNearbyToThing@@UAEPAXI@Z=_nf425_unused_slot")
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
#pragma comment(linker, "/alternatename:?PerformAction@CCreatureAction_AlertAllNearbyToThing@@UAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?PreventSneaking@CCreatureActionBase@@UBE_NXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?SetHitDirection@CCreatureActionBase@@UAEXVC3DVector@@@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?SetHitPosition@CCreatureActionBase@@UAEXVC3DVector@@@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?SetHitter@CCreatureActionBase@@UAEXABVCThing@@@Z=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?UpdateActionMoveToPos@CCreatureActionBase@@MAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?UpdateActionRunningTime@CCreatureActionBase@@UAEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?Validate@CBaseObject@@UBEXXZ=_nf425_unused_slot")
#pragma comment(linker, "/alternatename:?WaitForMeshToLoad@CCreatureActionBase@@MBE_NXZ=_nf425_unused_slot")
static unsigned int g_size = 0; static int g_news = 0, g_fail = 0;
static long g_block[0xB0 / 4 + 4];
void* operator new(size_t n) { g_size = (unsigned int)n; ++g_news; return g_fail ? 0 : (void*)g_block; }

int main(void)
{
    static long src_store[0xB0 / 4 + 4];
    unsigned char* s = (unsigned char*)src_store;
    for (int i = 4; i < 0xB0; ++i) s[i] = (unsigned char)(i * 7 + 3);
    const CCreatureAction_AlertAllNearbyToThing* src = (const CCreatureAction_AlertAllNearbyToThing*)(void*)src_store;
    CCreatureActionBase* r = src->CCreatureAction_AlertAllNearbyToThing::Clone();
    unsigned char* d = (unsigned char*)g_block;
    int ok = r == (CCreatureActionBase*)(void*)g_block && g_news == 1 && g_size == 0xB0 && g_ctors == 1 && g_this == (void*)g_block && g_other == (const void*)src_store
        && memcmp(d + 4, s + 4, 0xA8 - 4) == 0 && memcmp(d + 0xA8, s + 0xA8, 4) == 0 && memcmp(d + 0xAC, s + 0xAC, 4) == 0 && *(void**)d != 0;
    g_fail = 1;
    r = src->CCreatureAction_AlertAllNearbyToThing::Clone();
    ok = ok && r == 0 && g_news == 2 && g_ctors == 1;
    if (!ok) {
        printf("0084c800_TEST FAIL news=%d ctors=%d size=%u\n", g_news, g_ctors, g_size);
        return 1;
    }
    printf("0084c800_TEST PASS\n");
    return 0;
}
