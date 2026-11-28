// Leaf: ??0CCreatureAction_SummonerLightningOrbOutOfGroundSmash@@QAE@AAVCThingCreatureBase@@@Z
// Flags:
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_DRAGONBASE_LIFETIME_PASS29
#define FABLE_RETAIL_JACK_STAGE1_UPDATE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_SummonerLightningOrbOutOf.h"

#ifndef FABLE_ORB_CTOR_LINK_PRODUCTION
CCreatureAction_SummonerLightningOrbOutOfGroundSmash::CCreatureAction_SummonerLightningOrbOutOfGroundSmash(CThingCreatureBase& creature)
    : CCombatActionBase(creature, 150, CSoundPair())
{
}
#endif
#include <new>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
namespace {
int made,pointerReads,sets,drops;
CThingCreatureBase* seen;
void verify(bool ok){if(!ok) abort();}
}
CCombatActionBase::CCombatActionBase(CThingCreatureBase& creature,long priority,const CSoundPair& s) {
    ++made; seen=&creature; verify(priority==150 && !s.FirstSound && !s.LastSound);
}
CCombatActionBase::~CCombatActionBase(){}
CCharString::~CCharString(){}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){++drops;}
CBaseObject* CBaseIntelligentPointer::GetPItem() const {++pointerReads;return reinterpret_cast<CBaseObject*>(PData);}
void CBaseIntelligentPointer::SetPItem(CBaseObject* item){++sets;PData=reinterpret_cast<CBaseObjectPointer*>(item);}
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
CCreatureActionBase* CCreatureAction_SummonerLightningOrbOutOfGroundSmash::Clone() const { abort(); return 0; }
void CCreatureAction_SummonerLightningOrbOutOfGroundSmash::Initialise(){abort();}
void CCreatureAction_SummonerLightningOrbOutOfGroundSmash::PerformAction(){abort();}
void CCreatureAction_SummonerLightningOrbOutOfGroundSmash::FinishAction(){abort();}
int main(){
    unsigned long storage[128],creature_data[128],orb_data[128];
    CThingCreatureBase& creature=*reinterpret_cast<CThingCreatureBase*>(creature_data);
    for(int i=0;i<3;++i) {
        memset(storage,0,sizeof(storage));memset(orb_data,0,sizeof(orb_data));
        CThing* orb=i?reinterpret_cast<CThing*>(orb_data):0;
        if(i==2)reinterpret_cast<unsigned char*>(orb_data)[0x91]=1;
        made=pointerReads=sets=drops=0;seen=0;
        CCreatureAction_SummonerLightningOrbOutOfGroundSmash* action=new(storage) CCreatureAction_SummonerLightningOrbOutOfGroundSmash(creature);
        verify(made==1 && seen==&creature);
        bool assigned=false;
        const void* stored=0;memcpy(&stored,reinterpret_cast<const char*>(action)+0xac,4);
        verify(stored==(assigned?orb:0) && pointerReads==int(assigned) && sets==2*int(assigned) && drops==int(assigned));
        action->~CCreatureAction_SummonerLightningOrbOutOfGroundSmash();
    }
    puts("008cbea0_TEST PASS");return 0;
}
