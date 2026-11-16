// 007ce7a0 CTCElectrocutedEffect::Reset
// Flags: none
// Leaf: ?Reset@CTCElectrocutedEffect@@QAEXXZ
#define FABLE_RETAIL_ELECTROCUTED_ORIGINAL_PASS23
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCElectrocutedEffect.h"
#include "engine/_quarantine/CSpecialAbilitiesElectrocutedEffectDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/ThingAlive.h"
void CTCElectrocutedEffect::Reset()
{
 CThing& parent=GetParentThing();
 parent.SetAsRenderedThisFrame(1);
 parent.SetAsFrameUpdated();
 CurTime=0;
 FadeInTime=(long)(PDef->FadeInTimeSecs*CMainGameComponent::GetConstantFPS());
 FadeOutTimeStart=(long)(PDef->FadeOutTimeStartSecs*CMainGameComponent::GetConstantFPS());
 FadeOutTime=(long)(PDef->FadeOutTimeSecs*CMainGameComponent::GetConstantFPS());
 CDefPointer<const CSpecialEffectsDef> pspecial_effects;
 long effect=-1;
 if(!IS_THING_ALIVE(PEffect)) {
  bool use_dummy_center=false;
  C3DVector pos=parent.GetPos();
  CCharString effect_name("FRAZZLED_EFFECT_MEDIUM");
  C3DVector bottompos=pos;
  C3DVector toppos=bottompos+C3DVector(0.0f,0.0f,PDef->HeightMedium);
  const NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance=parent.GetTCGraphicAppearance();
  if(ptc_graphic_appearance) {
   toppos=ptc_graphic_appearance->GetFocalPos(1.0f);
   CRightHandedSet rhs;
   use_dummy_center=ptc_graphic_appearance->GetDummyObject("DUMMY_CENTRE",0,0,pos,rhs,1.0f);
  }
  CDefPointer<const CSpecialEffectsDef> pcreature_special_fx;
  GetParentThing().GetPBaseDef()->GetSubDef(&pcreature_special_fx);
  if(!(pcreature_special_fx.Get()&&pcreature_special_fx->GetEffectFromString("ELECTROCUTED_EFFECT",effect))) {
   float height=toppos.Z-bottompos.Z;
   if(use_dummy_center)effect_name="FRAZZLED_EFFECT_POINT";
   else if(height<PDef->HeightSmall)effect_name="FRAZZLED_EFFECT_SMALL";
   else if(height>PDef->HeightMedium)effect_name="FRAZZLED_EFFECT_LARGE";
   if(PDef->GetSubDef(&pspecial_effects))pspecial_effects->GetEffectFromString(effect_name,effect);
  }
  if(effect!=-1) {
   PEffect=CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effect,pos,false));
   if(IS_THING_ALIVE(PEffect)) {
    CTCDParticleEmitter* ptc_emitter=0;
    PEffect->GetTC(&ptc_emitter);
    if(ptc_emitter) {
     if(use_dummy_center)ptc_emitter->AttachToThing(parent,10,"DUMMY_CENTRE",0,0.0f);
     else ptc_emitter->AttachToThing(parent,8,"",0,0.0f);
    }
   }
  }
 }
}