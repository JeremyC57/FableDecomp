// NCreatureMode::CModeThunderLightningStorm::CreateSkyBeam -- spawn the sky and attached creator effects and add electrocution on touch
// Leaf: ?CreateSkyBeam@CModeThunderLightningStorm@NCreatureMode@@AAEXXZ
// Retail: 008daa80,551 bytes; private thiscall.
// Flags: none
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_SKYBEAM_ORIGINAL
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CMODESTANDBASE
#include "engine/CThing.h"
#include "engine/C3DVector.h"
#include "engine/CSpecialAbilitiesThunderLightningStormDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/NCreatureMode_CModeThunderLightningStorm.h"
#include "engine/ThingAlive.h"
void NCreatureMode::CModeThunderLightningStorm::CreateSkyBeam()
{
 CDefPointer<const CSpecialEffectsDef> pspecial_fx_def;
 long effect_index=0;
 PLightningStormDef->GetSubDef(&pspecial_fx_def);
 if(pspecial_fx_def->GetEffectFromString("LIGHTNING_SKY_BEAM_EFFECT",effect_index)) {
  C3DVector weapon_pos=GetParentThing().GetPos();
  CThing* pEmitterThing=CTCDParticleEmitter::Create(effect_index,weapon_pos,false);
  if(IS_THING_ALIVE(pEmitterThing))PSkyBeam=CIntelligentPointer<CThing>(pEmitterThing);
 }
 effect_index=0;
 if(pspecial_fx_def->GetEffectFromString("LIGHTNING_CREATOR_EFFECT",effect_index)) {
  CThing* pTargetEmitterThing=CTCDParticleEmitter::Create(effect_index,GetParentThing().GetPos(),false);
  if(IS_THING_ALIVE(pTargetEmitterThing)) {
   PCreatorEffect=CIntelligentPointer<CThing>(pTargetEmitterThing);
   CTCDParticleEmitter* ptcEmitter=0;
   pTargetEmitterThing->GetTC(&ptcEmitter);
   if(ptcEmitter)ptcEmitter->AttachToThing(GetParentThing(),12,"",2,0.0f);
  }
 }
 if(!GetParentThing().HasTC(TCI_ELECTROCUTE_ON_TOUCH))GetParentThing().AddTC("CTCElectrocuteOnTouch",false,0);
}