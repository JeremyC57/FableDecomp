// 007c60a0 CTCGhostSword::UpdateGhostSwordParticleEffects
// Leaf: ?UpdateGhostSwordParticleEffects@CTCGhostSword@@QAEXXZ
// Flags: none
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CCRC_ORIGINAL_TYPED_LOOP_PASS27
#define FABLE_RETAIL_CRC32TAB_PASS29
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_VECTORKEY_PASS26
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CTCDPARTICLEEMITTER
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_UPDATEPARAMETERS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "fable_assert.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCGHOSTSWORD
#define FABLE_RETAIL_CTCGHOSTSWORD_FRAME
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/ThingAlive.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCGhostSword.h"
#include "engine/CRightHandedSet.h"
#include "engine/NParticleEngine_CParticleUpdateParameters.h"
#include "engine/CTCDParticleEmitter.h"
void CTCGhostSword::UpdateGhostSwordParticleEffects()
{
 for(long i=0;i<Minions.size();++i) {
  if(IS_THING_ALIVE(Minions[i])) {
   C3DVector startpos=Minions[i]->GetPos();
   NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance_creature=Minions[i]->GetTCGraphicAppearance();
   if(ptc_graphic_appearance_creature) {
    CRightHandedSet rhs;
    if (!ptc_graphic_appearance_creature->GetDummyObject("EMITTER_BASE",0,0,startpos,rhs,1.0f)) { FABLE_RETAIL_ASSERT(!"Ghost sword spline 1"); }
   }
   std::pair<unsigned long,C3DVector> vtStartPos(NParticleEngine::MAKE_VECTOR_PARAM_FROM_BASE(0,"EMITTER_BASE"),startpos);
   C3DVector endpos=Minions[i]->GetPos();
   if(ptc_graphic_appearance_creature) {
    CRightHandedSet rhs;
    if (!ptc_graphic_appearance_creature->GetDummyObject("EMITTER_TIP",0,0,endpos,rhs,1.0f)) { FABLE_RETAIL_ASSERT(!"Ghost Sword spline 2"); }
   }
   std::pair<unsigned long,C3DVector> vtEndPos(NParticleEngine::MAKE_VECTOR_PARAM_FROM_BASE(0,"EMITTER_TIP"),endpos);
   NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long> upParams;
   upParams.VectorParams.Insert(vtStartPos);
   upParams.VectorParams.push_back(vtEndPos);
   upParams.Update();
   if(IS_THING_ALIVE(MinionEffects[i])) {
    CTCDParticleEmitter* ptcdEmitter=0;
    if(!MinionEffects[i]->GetTC(&ptcdEmitter)) { FABLE_RETAIL_ASSERT(!"MinionEffects[i]->GetTC(&ptcd_" "emitter)"); }
    ptcdEmitter->SetEmitterUpdateParameters(upParams);
   }
  }
 }
}