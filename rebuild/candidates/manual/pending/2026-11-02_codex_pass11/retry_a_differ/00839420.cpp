// NThingHelpers::CreateEffectOnHit(const CThing& thing_hit, const CHitParameters& hit_params, const CWorldMap& world_map, float delta_health) -- create and orient material hit effects
// Leaf: ?CreateEffectOnHit@NThingHelpers@@YIXABVCThing@@ABVCHitParameters@@ABVCWorldMap@@M@Z
// Grade: DIFFER(1300v1308) -- frame slot ranking of the vector temporaries and x87/effect-call setup scheduling
// Context: grades against the Codex row context $S/codex11_dl/x/rows/00839420/selected_context/hdr (Codex per-feature macros; NOT rebased on rebuild/include); source = Codex selected; 00839420 probes in probe/00839420.log
// Flags: /Gr
#define FABLE_RETAIL_HIT_EFFECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CCREATUREDEF
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_DEFINITION_MANAGER
#include "engine/GFMaths.h"
#include "engine/CDefString.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CMaterialDef.h"
#include "engine/CCreatureDef.h"
#include "engine/CHitParameters.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorldMap.h"
#include "engine/CRegion.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CCRC.h"
extern const float DELTA_NOTIONAL_ZERO;
namespace NGlobalConsole {extern bool CombatParticleEffects;}
namespace NCombatActionHelpers {bool __fastcall IsToCreateCombatParticleEffects(const CThing&,const CThing*);}
template<class T>bool __fastcall IS_THING_ALIVE(const T& thing){return thing&&!thing->IsDead();}
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
namespace NThingHelpers {
void __fastcall CreateEffectOnHit(const CThing& thing_hit,const CHitParameters& hit_params,const CWorldMap& world_map,float delta_health)
{
 const CThing* pthing_hitting=0;
 pthing_hitting=hit_params.GetPThingHitting();
 if(!NCombatActionHelpers::IsToCreateCombatParticleEffects(thing_hit,pthing_hitting))return;
 C3DVector point_of_hit=hit_params.GetHitPosition();
 if(!NGlobalConsole::CombatParticleEffects||!hit_params.IsCreateParticleEffectOnHit())return;
 C3DVector attack_direction=hit_params.GetHitDirection();
 CDefPointer<const CMaterialDef> pmaterial_def=hit_params.GetPHitMaterialDef();
 {
 if(IS_THING_ALIVE(pthing_hitting)&&(thing_hit.GetTypeFlags()&8)&&hit_params.IsMeleeStrike()){
  CDefPointer<const CCreatureDef> pcreature_def;
  if(thing_hit.GetPBaseDef()->GetSubDef(&pcreature_def)&&!pcreature_def->UseActualHitPosForHitEffects){
   C3DVector line_out=pthing_hitting->GetPos()-thing_hit.GetPos();line_out.Z=0.0f;
   if(line_out.GetSquaredMagnitude2D()>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO){
    line_out.AccurateNormalise();
    C3DVector new_point_of_hit=thing_hit.GetPos()+line_out*thing_hit.GetPhysicsTC()->GetRadius()*0.75f;
    new_point_of_hit.Z=point_of_hit.Z;
    C3DVector new_direction_of_hit=attack_direction;
    if(pthing_hitting->GetTypeFlags()&8){
     const CThingCreatureBase* phitter=static_cast<const CThingCreatureBase*>(pthing_hitting);
     C3DVector perp;
     if(phitter->GetLastActionHandedness()==HANDED_RIGHT){perp.Initialise(line_out.Y,-line_out.X,0.0f);new_direction_of_hit=perp*-1.0f;}
     else if(phitter->GetLastActionHandedness()==HANDED_LEFT){perp.Initialise(line_out.Y,-line_out.X,0.0f);new_direction_of_hit=perp;}
    }
    point_of_hit=new_point_of_hit;attack_direction=new_direction_of_hit;
   }
  }
 }
 }
 if(!world_map.IsPosInMap(point_of_hit))return;
 if(!thing_hit.GetPBaseDef().IsNull()){
  C3DVector lod_centre=CMainGameComponent::Get()->PeekWorld().GetGameLODCentre();
  if((point_of_hit-lod_centre).GetSquaredMagnitude()>GFSqr(thing_hit.GetPBaseDef()->RenderFadeDistance))return;
 }
 if(!pmaterial_def.IsNull()){
  const CGameDefinitionManager& def_man=CMainGameComponent::Get()->PeekDefinitionManager();
  const CRegion& region=world_map.GetRegion(world_map.GetLoadedRegion());
  const CRegionDef* pregion_def=region.GetPRegionDef().Get();
  long region_def_index=0;
  if(pregion_def)region_def_index=def_man.GetDefGlobalIndexFromName(pregion_def->GetInstantiationName().GetString());
  long from_effect_id=0,to_effect_id=0;float interpolate=0.0f;CThing* pthing_effect=0;
  if(pmaterial_def->GetBlendedSwordHitEffect(region_def_index,delta_health,from_effect_id,to_effect_id,interpolate)){
   if(from_effect_id&&to_effect_id)pthing_effect=CTCDParticleEmitter::Create(from_effect_id,to_effect_id,interpolate,point_of_hit,false);
  }else{
   if(IS_THING_ALIVE(pthing_hitting)&&pthing_hitting->HasTC(TCI_BERSERK))from_effect_id=pmaterial_def->GetRandomBerserkedSwordHitParticleEffect();
   else from_effect_id=pmaterial_def->GetRandomSwordHitParticleEffect();
   if(from_effect_id>0)pthing_effect=CTCDParticleEmitter::Create(from_effect_id,point_of_hit,false);
  }
  if(IS_THING_ALIVE(pthing_effect)){
   CTCDParticleEmitter* tcd=0;pthing_effect->GetTCDriver(&tcd);
   std::pair<unsigned long,C3DVector> parameter(CCRC::Calc(0,"DIRECTION",9),attack_direction);
   NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long> params;
   params.VectorParams.push_back(parameter);tcd->SetEmitterUpdateParameters(params);
  }
 }
}
}
