// Leaf: ??0CCreatureAction_JumpOutFromCover@@QAE@AAVCThingCreatureBase@@0ABVC3DVector@@@Z
// Flags:
#define FABLE_RETAIL_JUMP_OUT_FROM_COVER_OWNER_PASS30
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_DRAGONBASE_LIFETIME_PASS29
#define FABLE_RETAIL_JACK_STAGE1_UPDATE
#define FABLE_RETAIL_MOVE_FROM_COVER_PASS30
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_JumpOutFromCover.h"

#ifndef FABLE_COVER_CTOR_TEST_LINK
CCreatureAction_JumpOutFromCover::CCreatureAction_JumpOutFromCover(CThingCreatureBase& creature, CThingCreatureBase& target, const C3DVector& direction)
        : CCombatActionBase(creature, target, 150, GetSoundPair(creature, "SWORD_SWING")), JumpDirection(direction) {}
#endif

#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
namespace {
int sounds, bases;
CThingCreatureBase* seenOwner;CThing* seenTarget;
void check(bool b){if(!b)abort();}
}
CCharString::CCharString(const char* text,long length){check(!strcmp(text,"SWORD_SWING") && length==-1);++sounds;}
CCharString::~CCharString(){}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){}
CSoundPair CCombatActionBase::GetSoundPair(const CThingCreatureBase& c,const CCharString&){check(&c==seenOwner); CSoundPair s;s.FirstSound=11;s.LastSound=28;return s;}
CCombatActionBase::CCombatActionBase(CThingCreatureBase& c,CThing& t,long p,const CSoundPair& s){check(&c==seenOwner&&&t==seenTarget&&p==150&&s.FirstSound==11&&s.LastSound==28);++bases;}
CCombatActionBase::~CCombatActionBase(){}
CCreatureActionBase* CCreatureAction_JumpOutFromCover::Clone()const{abort();return 0;}
void CCreatureAction_JumpOutFromCover::Initialise(){abort();}
int main(){
    unsigned long actor[96],enemy[96],store[96];
    for(int who=0;who<2;++who)for(int n=-5;n<=5;++n){
        seenOwner=reinterpret_cast<CThingCreatureBase*>(who?enemy:actor);seenTarget=reinterpret_cast<CThing*>(who?actor:enemy);
        C3DVector dir((float)n/3, (float)n*n, (float)-n/2);sounds=bases=0;
        CCreatureAction_JumpOutFromCover* action=new(store) CCreatureAction_JumpOutFromCover(*seenOwner,*reinterpret_cast<CThingCreatureBase*>(seenTarget),dir);
        check(sounds==1&&bases==1); C3DVector actual;memcpy(&actual,reinterpret_cast<char*>(action)+0x114,12);
        check(actual.X==dir.X&&actual.Y==dir.Y&&actual.Z==dir.Z);action->~CCreatureAction_JumpOutFromCover();
    }
    puts("0097b3f0_TEST PASS");return 0;
}
// These unused engine boundary calls fail the fixture if reached.
CCreatureActionBase::~CCreatureActionBase() {}
void CCombatActionBase::PerformAction() { abort(); }
CCreatureActionBase* CCombatActionBase::Clone() const { abort(); return 0; }
void CCreatureActionBase::EditUpdate(float) { abort(); }
void CCreatureActionBase::FinishAction() { abort(); }
bool CCreatureActionBase::IsPlayingAnimation() const { abort(); return 0; }
long CCreatureActionBase::GetAnimDelayTime() const { abort(); return 0; }
long CCreatureActionBase::GetAnimationIndex() const { abort(); return 0; }
long CCreatureActionBase::GetAnimationTransitionInTime() const { abort(); return 0; }
long CCreatureActionBase::GetAnimationTransitionOutTime() const { abort(); return 0; }
unsigned long CCreatureActionBase::GetAnimExtraFlags() const { abort(); return 0; }
void CCreatureActionBase::Initialise() { abort(); }
bool CCreatureActionBase::IsStrikeEvent(long) const { abort(); return 0; }
bool CCreatureActionBase::IsStrikeEventActive() const { abort(); return 0; }
bool CCreatureActionBase::IsStrikeAction() const { abort(); return 0; }
bool CCreatureActionBase::IsPartOfComboChain() const { abort(); return 0; }
bool CCreatureActionBase::IsStrikeEventFinishedBeingActive() const { abort(); return 0; }
bool CCreatureActionBase::IsVitalStrike() const { abort(); return 0; }
bool CCreatureActionBase::IsKnockdownStrike() const { abort(); return 0; }
bool CCreatureActionBase::IsBlockCounter() const { abort(); return 0; }
bool CCreatureActionBase::IsMeleeFlourish() const { abort(); return 0; }
bool CCreatureActionBase::IsActionWithStrikeEventHelperName() const { abort(); return 0; }
CCharString CCreatureActionBase::GetStrikeEventHelperName() const { abort(); return CCharString(""); }
bool CCreatureActionBase::IsOverrideableByActionOfSamePriority() const { abort(); return 0; }
void CCreatureActionBase::OnFrameUpdate(float) { abort(); }
bool CCreatureActionBase::IsQueuable() const { abort(); return 0; }
bool CCreatureActionBase::IsActionReadyForInterruption() const { abort(); return 0; }
bool CCreatureActionBase::IsActionReadyForInterruptionByMovement(const C3DVector &) const { abort(); return 0; }
bool CCreatureActionBase::IsBlockable() const { abort(); return 0; }
bool CCreatureActionBase::IsAffectedByHaste() const { abort(); return 0; }
bool CCreatureActionBase::IsAffectedByHeroStats() const { abort(); return 0; }
bool CCreatureActionBase::IsPhysicsPushableByHero() const { abort(); return 0; }
bool CCreatureActionBase::IsAlwaysPhysicsPushableBySelfAvoidance() const { abort(); return 0; }
bool CCreatureActionBase::IsAlwaysUseAnimationMovement() const { abort(); return 0; }
bool CCreatureActionBase::IsClearingMovementVector() const { abort(); return 0; }
bool CCreatureActionBase::IsToHaveEmptyHands() const { abort(); return 0; }
bool CCreatureActionBase::IsToReplaceItemIfRemoved() const { abort(); return 0; }
bool CCreatureActionBase::IsAllowingAnimationMovementToMoveSpeakAndWaitCameras() const { abort(); return 0; }
bool CCreatureActionBase::IsPreventingQuickAccessItemUsage() const { abort(); return 0; }
void CCreatureActionBase::SetHitter(const CThing &) { abort(); }
void CCreatureActionBase::SetHitDirection(C3DVector) { abort(); }
void CCreatureActionBase::SetHitPosition(C3DVector) { abort(); }
CTCWeaponTrail * CCreatureActionBase::GetWeaponTrailUsedInStrikeAction(long &) const { abort(); return 0; }
float CCreatureActionBase::GetTotalTimeSeconds() const { abort(); return 0; }
bool CCreatureActionBase::IsToEvadeAllHits() const { abort(); return 0; }
bool CCreatureActionBase::IsToCauseRecoil() const { abort(); return 0; }
bool CCreatureActionBase::IsAbleToBeAttacked() const { abort(); return 0; }
bool CCreatureActionBase::IsPlayHitResponse() const { abort(); return 0; }
bool CCreatureActionBase::IsPreventingNormalMovement() const { abort(); return 0; }
bool CCreatureActionBase::IsPreventingLookTurning() const { abort(); return 0; }
void CCreatureActionBase::OnBlowBlocked(bool) { abort(); }
void CCreatureActionBase::OnFollowUpActionFailedToAdd() { abort(); }
void CCreatureActionBase::UpdateActionRunningTime() { abort(); }
bool CCreatureActionBase::IsDecapitatingAttack() const { abort(); return 0; }
bool CCreatureActionBase::PreventSneaking() const { abort(); return 0; }
void CCreatureActionBase::FrameUpdate() { abort(); }
bool CCreatureActionBase::IsUsingAnimationMovement() const { abort(); return 0; }
bool CCreatureActionBase::IsUsingForcedMovement() const { abort(); return 0; }
bool CCreatureActionBase::IsMovementUsingPhysics() const { abort(); return 0; }
void CCreatureActionBase::UpdateActionMoveToPos() { abort(); }
void CCreatureActionBase::InitialiseActionMovement() { abort(); }
float CCreatureActionBase::GetFrameUpdateTime() const { abort(); return 0; }
float CCreatureActionBase::GetActionSpeedParameter() const { abort(); return 0; }
float CCreatureActionBase::GetActionStartRunningTimeSeconds() const { abort(); return 0; }
bool CCreatureActionBase::OverrideFaceWithDefault() const { abort(); return false; }
bool CCreatureActionBase::AllowsCombinationAnims() const { abort(); return false; }
bool CCreatureActionBase::CanAnimationTransitionOut() const { abort(); return false; }
void CCreatureActionBase::OnAnimationLoaded() { abort(); }
bool CCreatureActionBase::WaitForMeshToLoad() const { abort(); return false; }
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseObject::CBaseObject() : IntelligentPointer(0) {}
CBaseObject::~CBaseObject() {}
void CBaseObject::Validate() const { abort(); }


void CCombatActionBase::FrameUpdate() { abort();  }
void CCombatActionBase::UpdateActionRunningTime() { abort();  }
float CCombatActionBase::GetFrameUpdateTime() const { abort(); return 0; }
void CCombatActionBase::Initialise() { abort();  }
long CCombatActionBase::GetAnimationIndex() const { abort(); return 0; }
float CCombatActionBase::GetTotalTimeSeconds() const { abort(); return 0; }
long CCombatActionBase::GetAnimationTransitionInTime() const { abort(); return 0; }
long CCombatActionBase::GetAnimDelayTime() const { abort(); return 0; }
bool CCombatActionBase::AllowsCombinationAnims() const { abort(); return 0; }
bool CCombatActionBase::IsActionReadyForInterruption() const { abort(); return 0; }
bool CCombatActionBase::IsActionReadyForInterruptionByMovement(const C3DVector & p0) const { abort(); return 0; }
unsigned long CCombatActionBase::GetAnimExtraFlags() const { abort(); return 0; }
void CCombatActionBase::OnBlowBlocked(bool p0) { abort();  }
C3DVector CCombatActionBase::GetNextPos() const { abort(); return C3DVector(0,0,0); }
bool CCombatActionBase::IsStrikeAction() const { abort(); return 0; }
bool CCombatActionBase::IsStrikeEvent(long p0) const { abort(); return 0; }
bool CCombatActionBase::IsStrikeEventActive() const { abort(); return 0; }
bool CCombatActionBase::IsStrikeEventFinishedBeingActive() const { abort(); return 0; }
bool CCombatActionBase::IsToUseRotationInAnimation() const { abort(); return 0; }
bool CCombatActionBase::IsToAlignWithGroundNormal() const { abort(); return 0; }
bool CCombatActionBase::IsToFaceTarget() const { abort(); return 0; }
bool CCombatActionBase::IsToFaceTargetAtStart() const { abort(); return 0; }
C3DVector CCombatActionBase::GetActionMovementVector() const { abort(); return C3DVector(0,0,0); }
bool CCombatActionBase::IsToPerformActionOnStrike() const { abort(); return 0; }
bool CCombatActionBase::IsToHitProneThings() const { abort(); return 0; }
float CCombatActionBase::GetIsToFaceTargetCone() const { abort(); return 0; }
float CCombatActionBase::GetIsToFaceTargetMinDist() const { abort(); return 0; }
CCharString CCombatActionBase::GetStrikeEventHelperName() const { abort(); return CCharString(""); }
float CCombatActionBase::GetDamageMultiplier() const { abort(); return 0; }
void CCombatActionBase::UpdateActionMoveToPos() { abort();  }
bool CCombatActionBase::IsVitalStrike() const { abort(); return 0; }
bool CCombatActionBase::IsKnockdownStrike() const { abort(); return 0; }
bool CCombatActionBase::IsBlockCounter() const { abort(); return 0; }
bool CCombatActionBase::IsMeleeFlourish() const { abort(); return 0; }
bool CCombatActionBase::IsDecapitatingAttack() const { abort(); return 0; }
bool CCombatActionBase::IsBlockable() const { abort(); return 0; }
bool CCombatActionBase::IsUsingAnimationMovement() const { abort(); return 0; }
bool CCombatActionBase::IsMovementUsingPhysics() const { abort(); return 0; }
void CCombatActionBase::InitialiseActionMovement() { abort();  }
void CCombatActionBase::FinishAction() { abort();  }
CTCWeaponTrail * CCombatActionBase::GetWeaponTrailUsedInStrikeAction(long & p0) const { abort(); return 0; }
bool CCombatActionBase::IsActionWithStrikeEventHelperName() const { abort(); return 0; }
void CCombatActionBase::OnAnimationLoaded() { abort();  }
