// CThingCreatureBase::ReactToGenericStrike(CHitParameters const &) -- select and start the hit-response action
// Leaf: ?ReactToGenericStrike@CThingCreatureBase@@IAEXABVCHitParameters@@@Z
// Compiler: VC7.1 RTM /O2 /Oy; no extra flags.
// Grade: RELOCATION_MATCH(1755)
#define FABLE_RETAIL_REACT_STRIKE
#define FABLE_RETAIL_EFFECT_LOOKUP
#define FABLE_RETAIL_DEFINITION_MANAGER
#define FABLE_RETAIL_CREATURE_ACTIONS
#define FABLE_RETAIL_TYPED_GETTC
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_VISIBLE_LOWERBOUND
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CHitParameters.h"
#include "engine/CTCDExplosion.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/NHitLocations_CHitLocation.h"
#include "engine/CCombatAction_GenericResponseToDecapitation.h"
#include "engine/CCombatAction_GenericResponseToDrain.h"
#include "engine/CCombatAction_GenericResponseToExplosion.h"
#include "engine/CCombatAction_GenericStrikeResponse.h"
#include "engine/CCombatAction_GenericStrikeResponseKnockdown.h"
#include "engine/CDefinitionManager.h"
extern const float DELTA_NOTIONAL_ZERO;
template<class T> inline bool __fastcall IS_THING_ALIVE(const T& thing) { return thing && !thing->IsDead(); }
template<> __declspec(noinline) bool CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<const CSpecialEffectsDef>* out) const
{
    const CSubDefInfo* index;
    { CCharString name("CSpecialEffectsDef",-1); index=GetPSubDefInfo(name); }
    if(index) {
        CDefPointer<const CSpecialEffectsDef> result(PDefManager->GetPDefFromGlobalIndex(index->DefIndex));
        *out=result;
        return true;
    }
    return false;
}
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name,long& result) const
{
    unsigned long key=name.GetCRC();
    std::map<unsigned long,long>::const_iterator item=SpecialEffectsMap.ContainmentMap.find(key);
    if(item!=SpecialEffectsMap.ContainmentMap.end()) { result=item->second;return true; }
    return false;
}
void CThingCreatureBase::ReactToGenericStrike(const CHitParameters& hit_params)
{
    { const bool& coop=HasTC(TCI_COOP_SPIRIT); if(coop) return; }
    if(!hit_params.IsPlayGetHitResponse(this) || !IsToRespondToHits()) return;
    CThing* pobject_that_hit_me=hit_params.GetPThingHitting();
    C3DVector point_of_hit(GetPos());
    if(hit_params.IsLocalised()) point_of_hit=hit_params.GetHitPosition();
    C3DVector direction_of_hit(0.0f,0.0f,0.0f);
    if(hit_params.IsDirectional()) direction_of_hit=hit_params.GetHitDirection();
    else if(IS_THING_ALIVE(pobject_that_hit_me)) direction_of_hit=point_of_hit-pobject_that_hit_me->GetPos();
    if(!(direction_of_hit.GetSquaredMagnitude()>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO))
        direction_of_hit=C3DVector(1.0f,0.0f,0.0f);
    bool nomoveback=!(hit_params.IsMoveBackOnHit() && !IsUnderInfluenceOfEpicSpell());
    bool knock_down=hit_params.IsKnockDown();
    bool killing_blow=GetHealth()<DELTA_NOTIONAL_ZERO;
    bool decapitate=hit_params.IsDecapitate();
    bool explosion=hit_params.IsExplosionStrike();
    bool drain=hit_params.IsDrainStrike();
    if(!PeekDefinition().IsPushedByExplosions) explosion=false;
    { const bool& berserk=HasTC(TCI_BERSERK); if(berserk) knock_down=false; }
    NHitLocations::CHitLocation* phit_location=hit_params.GetPHitLocation();
    EActionRegisteredType action_response_type=ACTION_NULL;
    if(!decapitate && !explosion && (!drain || killing_blow)) {
        if(killing_blow) action_response_type=phit_location->GetHitKnockdownDeathActionType();
        else if(knock_down) action_response_type=phit_location->GetHitKnockdownActionType();
        else if(!hit_params.IsEpicSpell()) action_response_type=phit_location->GetHitResponseActionType();
    } else if(explosion && IS_THING_ALIVE(pobject_that_hit_me)) {
        CTCDExplosion* pTCExplosion=0;
        if(pobject_that_hit_me->GetTC(&pTCExplosion)) {
            if(!pTCExplosion->GetPExplosionDef().IsNull() && !pTCExplosion->GetPExplosionDef()->ExplosionShouldKnockdown) {
                explosion=false;
                knock_down=false;
            }
        }
    }
    if(action_response_type!=ACTION_NULL) {
        CCountedPointer<CCreatureActionBase> paction_response=CCreatureActionBase::GetPActionFromEnum(action_response_type,*this);
        paction_response->SetHitDirection(direction_of_hit);
        paction_response->SetHitPosition(point_of_hit);
        if(IS_THING_ALIVE(pobject_that_hit_me)) paction_response->SetHitter(*pobject_that_hit_me);
        SetCurrentAction(*paction_response);
        return;
    }
    if(decapitate && HasTC(TCI_DECAPITATION) && IsKillable() && IsDamageable()) {
        SetCurrentAction(CCombatAction_GenericResponseToDecapitation(*this));
        return;
    }
    if(!knock_down && drain && IS_THING_ALIVE(pobject_that_hit_me)) {
        const CTCCreatureModeManager* ptc_cmm=0;
        PeekTC(&ptc_cmm);
        if(ptc_cmm->GetCurrentMode().GetType()==NCreatureMode::MODE_DRAINED) return;
        SetCurrentAction(CCombatAction_GenericResponseToDrain(*this,*pobject_that_hit_me));
        return;
    }
    if(explosion && IS_THING_ALIVE(pobject_that_hit_me)) {
        const CTCCreatureModeManager* ptc_cmm=0;
        PeekTC(&ptc_cmm);
        if(ptc_cmm->GetCurrentMode().GetType()==NCreatureMode::MODE_EXPLOSIVE_FORCE_PUSHED) return;
        SetCurrentAction(CCombatAction_GenericResponseToExplosion(*this,*pobject_that_hit_me,point_of_hit,direction_of_hit));
        return;
    }
    if(killing_blow) {
        CDefPointer<const CSpecialEffectsDef> pfx_death;
        long effect_index;
        if(PBaseDef->GetSubDef(&pfx_death) && pfx_death->GetEffectFromString("EXPLODE_ON_KILLING_BLOW",effect_index)) {
            Kill(true);
            return;
        }
        SetCurrentAction(CCombatAction_GenericStrikeResponseKnockdown(*this,pobject_that_hit_me,point_of_hit,direction_of_hit));
        return;
    }
    if(knock_down) {
        SetCurrentAction(CCombatAction_GenericStrikeResponseKnockdown(*this,pobject_that_hit_me,point_of_hit,direction_of_hit));
        return;
    }
    if(!hit_params.IsEpicSpell()) {
        CCombatAction_GenericStrikeResponse combatresponse(*this,pobject_that_hit_me,point_of_hit,direction_of_hit);
        combatresponse.SetCreateDustEffect(hit_params.IsCreateDustParticleEffectOnHit());
        if(nomoveback) combatresponse.SetMoveOnHitOverride(false);
        SetCurrentAction(combatresponse);
    }
}
