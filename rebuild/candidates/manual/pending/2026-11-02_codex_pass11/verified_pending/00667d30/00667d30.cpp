// CThingCreatureBase::OnHit(CHitParameters &) -- process the incoming hit and dispatch its component responses.
// Leaf: ?OnHit@CThingCreatureBase@@UAEXAAVCHitParameters@@@Z
// Grade: RELOCATION_MATCH (1210 bytes); typed component lookups with complete branch calls.
#define FABLE_RETAIL_DEFINITION_MANAGER
#define FABLE_RETAIL_TYPED_GETTC
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CCREATUREDEF
#define FABLE_RETAIL_CHEROCOMBATDEF
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCBULLETTIME
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_VISIBLE_LOWERBOUND
#include "engine/CThingCreatureBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CCreatureDef.h"
#include "engine/CHitParameters.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCFlashOnHit.h"
#include "engine/CTCTalk.h"
#include "engine/CTCScriptedControl.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCHaste.h"
#include "engine/CTCBulletTime.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCCombat.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerManager.h"
#include "engine/NCreatureMode.h"
#include "engine/CDefinitionManager.h"
extern const float DELTA_NOTIONAL_ZERO;
template<class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing) { return thing && !thing->IsDead(); }
namespace {
void __fastcall CreateWeaponHitTargetEffect(const CThing*,const C3DVector&,const C3DVector&,bool,bool,bool);
}
template<> __declspec(noinline) bool CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>* out) const
{
    const CSubDefInfo* index;
    { CCharString name("CCreatureDef",-1); index=GetPSubDefInfo(name); }
    if(index) {
        CDefPointer<const CCreatureDef> result(PDefManager->GetPDefFromGlobalIndex(index->DefIndex));
        *out=result;
        return true;
    }
    return false;
}
void CThingCreatureBase::OnHit(CHitParameters& hit)
{
    CThing* weapon=0;
    CThingGameObject::OnHit(hit);
    if(hit.GetPHitLocation() && IsAlive()) {
        CTCPhysicsBase& physics=*GetPhysicsTC();
        CThing* hitter=hit.GetPThingHitting();
        weapon=hit.GetPWeapon();
        const C3DVector* position=hit.IsLocalised()?&hit.GetHitPosition():&physics.GetPosition();
        const C3DVector* direction=hit.IsDirectional()?&hit.GetHitDirection():&physics.GetRHSet().GetForward();
        if(!hit.IsBlocked() && !hit.IsHitNegated()) {
            CDefPointer<const CCreatureDef> creatureDef;
            bool flash=GetPBaseDef()->GetSubDef(&creatureDef) && creatureDef->FlashOnHit;
            if(flash) {
                CTCFlashOnHit* effect=static_cast<CTCFlashOnHit*>(AddTC("CTCFlashOnHit",true,0));
                effect->SetFadeInTime(0);
                effect->SetFadeOutTime(0);
                effect->SetLifeTime(2);
            }
            CTCTalk* talk;
            if(GetTC(&talk)) {
                const CTCScriptedControl* script;
                if(talk->IsTalking() && (!PeekTC(&script) || !script->IsActive()))
                    talk->StopCurrentSpeech();
            }
            if(IS_THING_ALIVE(hitter) && hitter->GetType()==2 && hit.IsMeleeStrike() && GetPlayerManager().IsThingMainPlayer(*hitter)) {
                if(hit.IsLocalised() && hit.IsDirectional())
                    CreateWeaponHitTargetEffect(weapon,*position,*direction,hit.IsFlourish(),hit.IsKnockDown(),hit.IsBlockCounter());
                CPlayerGui::Get().AddHitByMelee(*this,*hitter,*position);
                {
                const CTCHeroExperience* experience=0;
                hitter->PeekTC(&experience);
                const CHeroCombatDef& combat=experience->PeekHeroCombatDef();
                CTCHaste* haste;
                if(!GetTC(&haste)) haste=static_cast<CTCHaste*>(AddTC("CTCHaste",false,0));
                haste->SetOnHitPause(combat.MeleeStrikeNumFramesPauseOnHeroStrike);
                if(!hitter->GetTC(&haste)) haste=static_cast<CTCHaste*>(hitter->AddTC("CTCHaste",false,0));
                haste->SetOnHitPause(combat.MeleeStrikeNumFramesPauseOnHeroStrike);
            }
            }
            CTCBulletTime* bullet;
            if(IS_THING_ALIVE(hitter) && hitter->GetTC(&bullet))
                bullet->PlayCreatureOnHitEffect(this);
            else if(GetTC(&bullet))
                bullet->PlayCreatureOnHitEffect(this);
            const CTCCreatureModeManager* mode;
            if(PeekTC(&mode) && mode->GetCurrentMode().GetType()==NCreatureMode::MODE_DAZED && hit.IsKnockDown())
                hit.SetDecapitate(true);
            const CTCCombat* combatComponent;
            if(PeekTC(&combatComponent) && !combatComponent->IsAbleToBeKnockedDown())
                hit.SetKnockDown(false);
            PQueuedActions.clear();
            if(PeekTC(&mode) && mode->IsModeActive(NCreatureMode::MODE_SLEEP_BED)) {
                hit.SetKnockDown(false);
                hit.SetPlayHitResponse(false);
                hit.SetDecapitate(false);
                hit.SetMoveBackOnHit(false);
            }
            if(GetHealth()<DELTA_NOTIONAL_ZERO) {
                hit.SetKnockDown(true);
                hit.SetPlayHitResponse(true);
            }
            ReactToGenericStrike(hit);
        }
    }
}
