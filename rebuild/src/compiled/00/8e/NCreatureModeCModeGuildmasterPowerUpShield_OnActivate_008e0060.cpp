// NCreatureMode::CModeGuildmasterPowerUpShield::OnActivate() -- guildmaster shield power-up: find nearby bodyguards, give each a physical-shield spell and a POWER_UP_BEAM effect
// Leaf: ?OnActivate@CModeGuildmasterPowerUpShield@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E0060, 839 bytes, frame 0x158.
// Port (NF74, round 7; nf44:50): every class from the shared headers (the 27 local models and the stand-in bodies of GetSubDef,
//   GetEffectFromString, GetAllThingsInRoughArea and both _M_insert_overflow instances gone): the sibling FrameUpdate 0x008E20F0 block,
//   the retail NCreatureMode_CModeGuildmasterPowerUpShield.h members (LightningBeamParticleEmitters +0x78, Guards +0x84), Ego_d.pdb locals.
// Callees: CModeStandBase::OnActivate 0x008621A0, CThingAICreature::SuspendAI 0x00833A10, CGameDefinitionManager::Get 0x0044C6B0 /
//   CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, CModeBase::GetWorld 0x0073A680, CThingSearchTools::GetAllThingsInRoughArea
//   <CThingAICreature, vector<CThingAICreature*>, CThingFilter_HasDefGlobalIndex> 0x008E3EB0 (same-TU body visible but out of line:
//   FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY), GetSubDef<CSpecialEffectsDef> 0x00669580 (CThing::GetPBaseDef by value),
//   CSpecialEffectsDef::GetEffectFromString 0x00669D30 (visible noinline), CWorld::GetWorldSeed 0x0049D810, GFROR13 0x00497890,
//   CCreatureAction_CastPhysicalShieldSpell 0x008BB6F0 (the temporary's implicit destructor calls ~CCombatActionBase 0x0062E900), SetCurrentAction 0x006644F0,
//   vector<CIntelligentPointer<CThingAICreature> >::_M_insert_overflow 0x007AFD10 and vector<CIntelligentPointer<CThing> >::
//   _M_insert_overflow 0x004AC940 (visible noinline: FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE), memmove (erase), CTCDParticleEmitter::
//   Create 0x006E0880, free. NF74 overlay opt-in FABLE_RETAIL_CCREATUREACTION_CASTPHYSICALSHIELD_CTOR (the ctor, both PDBs).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_SKYBEAM_ORIGINAL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_DRAINED
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_ACTIVATE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCREATUREACTION_CASTPHYSICALSHIELD
#define FABLE_RETAIL_CCREATUREACTION_CASTPHYSICALSHIELD_CTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#include <string.h>
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/ThingAlive.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_CastPhysicalShieldSpell.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CWorld.h"
class CTCMapwho;   // forward declaration the default CTCMapwho.h path needs before NThingMap::CMapwhoNode
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasDefGlobalIndex.h"
#include "engine/C2DBoxI.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFRandom.h"
#include "engine/NCreatureMode_CModeGuildmasterPowerUpShield.h"

void NCreatureMode::CModeGuildmasterPowerUpShield::OnActivate()
{
 CModeStandBase::OnActivate();
 if (GetParentThing().GetType() == 1) {
  CThingAICreature* p_ai_creature = static_cast<CThingAICreature*>(&GetParentThing());
  p_ai_creature->SuspendAI(true, this);
 }
 std::vector<CThingAICreature*> guards;
 long guard_def_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_GUILDMASTER_BODYGUARD");
 GetWorld().GetThingSearchTools().GetAllThingsInRoughArea<CThingAICreature>(C2DBoxI(GetParentThing().GetPos().ToC2DCoordI(), 20),
  CThingFilter_HasDefGlobalIndex(guard_def_index), guards);
 long effect_index = 0;
 CDefPointer<const CSpecialEffectsDef> pEffects;
 GetParentThing().GetPBaseDef()->GetSubDef(&pEffects);
 pEffects->GetEffectFromString("POWER_UP_BEAM", effect_index);
 while (guards.size() > 0 && Guards.size() < 3) {
  long random_guard = GFRandom(guards.size(), GetWorld().GetWorldSeed());
  long duration_in_frames = (long)((float)CMainGameComponent::GetConstantFPS() * 12.0f);
  guards[random_guard]->SetCurrentAction(CCreatureAction_CastPhysicalShieldSpell(*guards[random_guard], 2, true, "", duration_in_frames, 10.0f));
  Guards.push_back(CIntelligentPointer<CThingAICreature>(guards[random_guard]));
  std::vector<CThingAICreature*>::iterator it = guards.begin() + random_guard;
  guards.erase(it);
  CThing* p_emitter_thing = CTCDParticleEmitter::Create(effect_index, GetParentThing().GetPos(), false);
  if (IS_THING_ALIVE(p_emitter_thing))
   LightningBeamParticleEmitters.push_back(CIntelligentPointer<CThing>(p_emitter_thing));
 }
}
