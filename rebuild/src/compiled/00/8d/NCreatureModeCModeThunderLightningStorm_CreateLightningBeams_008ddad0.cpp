// NCreatureMode::CModeThunderLightningStorm::CreateLightningBeams() -- spawn NumberOfBeams LIGHTNING_BEAM_EFFECT things at the
//   caster (main beams, flagged MainBeamsCreated) and NumberOfBeams LIGHTNING_BEAM_END_EFFECT things, keeping intelligent pointers
//   to the live ones in the two emitter vectors; the special-effects sub-def reference is released at the end
// Leaf: ?CreateLightningBeams@CModeThunderLightningStorm@NCreatureMode@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DDAD0, 661 bytes (`ret`).
// Flags: /d2inlT65
// Inline-budget: /d2inlT65 -- LTCG per-site split: GetEffectFromString 0x00669D30 and push_back 0x004AD520 called at the first site, expanded (CCRC::Calc and the map find included) at the second; default / T70 / T80 expand both push_backs, T60 also calls Calc, T40 expands nothing; T65 matches (T61-T64 not bisected for budget).
// Port (NF74, round 7; nf44:50): every class from the shared headers (the 16 local models and the stand-in GetSubDef /
//   GetEffectFromString bodies gone), on the macro block of the landed sibling CreateSkyBeam 0x008DAA80 plus STLport's
//   vector<CIntelligentPointer<CThing> > (push_back inline; _M_insert_overflow 0x004AC940 visible but out of line). Retail is
//   link-time code generated, so VC7.1's inliner decides per call site: the first GetEffectFromString (0x00669D30) and the first
//   push_back (0x004AD520) are calls, the second ones are expanded (CCRC::Calc and the map find included); the visible inline
//   bodies (NF74 overlay opt-in FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_INLINE, the existing FABLE_RETAIL_CCRC_PASS22_BODY, the
//   STLport const find FABLE_RETAIL_STLP_TREE_CONSTFIND, CIntelligentPointer's copy ctor FABLE_RETAIL_CINTELLIGENTPOINTER_COPY) let it
//   decide. Locals carry the Ego_d.pdb names. FITTED: the NF74 overlay opt-in FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_NAMED_ARGS
//   (length then text as named locals before the CRC; Ego_d lists only `it` in GetEffectFromString) gives retail's index-first
//   `movzx edi,byte ptr [ecx+esi]` (0x008DDC00); without it the only difference is that SIB order (661 v 661).
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_SKYBEAM_ORIGINAL
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_INLINE
#define FABLE_RETAIL_CCRC_PASS22_BODY
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_NAMED_ARGS
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#include "engine/CThing.h"
#include "engine/C3DVector.h"
#include "engine/CSpecialAbilitiesThunderLightningStormDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/NCreatureMode_CModeThunderLightningStorm.h"
#include "engine/ThingAlive.h"

void NCreatureMode::CModeThunderLightningStorm::CreateLightningBeams()
{
 CDefPointer<const CSpecialEffectsDef> pspecial_fx_def;
 long effect_index=0;
 PLightningStormDef->GetSubDef(&pspecial_fx_def);
 if(pspecial_fx_def->GetEffectFromString("LIGHTNING_BEAM_EFFECT",effect_index)) {
  MainBeamsCreated=true;
  C3DVector weapon_pos=GetParentThing().GetPos();
  for(long n=0;n<PLightningStormDef->NumBeams;++n) {
   CThing* p_emitter_thing=CTCDParticleEmitter::Create(effect_index,weapon_pos,false);
   if(IS_THING_ALIVE(p_emitter_thing))LightningBeamParticleEmitters.push_back(CIntelligentPointer<CThing>(p_emitter_thing));
  }
 }
 if(pspecial_fx_def->GetEffectFromString("LIGHTNING_BEAM_END_EFFECT",effect_index)) {
  C3DVector weapon_pos=GetParentThing().GetPos();
  for(long n=0;n<PLightningStormDef->NumBeams;++n) {
   CThing* p_emitter_thing=CTCDParticleEmitter::Create(effect_index,weapon_pos,false);
   if(IS_THING_ALIVE(p_emitter_thing))LightningBeamEndParticleEmitters.push_back(CIntelligentPointer<CThing>(p_emitter_thing));
  }
 }
}
