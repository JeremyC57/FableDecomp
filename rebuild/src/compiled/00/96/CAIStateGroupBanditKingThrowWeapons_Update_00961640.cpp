// CAIStateGroup_BanditKingThrowWeapons::Update() -- throw and replace the weapons, then advance the boss phase.
// Retail: 0x00961640, 323 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
#define FABLE_RETAIL_C3DVECTOR_PACKED
// Leaf: ?Update@CAIStateGroup_BanditKingThrowWeapons@@MAEXXZ
// Flags: none
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_TARGETFACE
#define FABLE_RETAIL_CAISTATEGROUP_BANDITKINGTHROWWEAPONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_MAXHEALTH
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETHEALTH
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCREATUREACTION_BANDITKINGTHROWWEAPONS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroup_BanditKingThrowWeapons.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCBoss.h"
#include "engine/CCreatureAction_BanditKingThrowWeapons.h"
#include "engine/CCreatureAction_BanditKingChangeWeapons.h"
void CAIStateGroup_BanditKingThrowWeapons::Update()
{
 while(GetCreature().IsPerformingAnyAction()) {
  EAIActionReturn ret=AIAction_DoNothing();
  if(ret!=AI_ACTION_IN_PROGRESS&&ret!=AI_ACTION_FINISHED){Terminate();return;}
 }
 AIAction_PerformActionTargetFaceAndWait(CCreatureAction_BanditKingThrowWeapons(GetCreature(),*Player),Player,false);
 AIAction_PerformActionAndWait(CCreatureAction_BanditKingChangeWeapons(GetCreature()),false);
 CTCBoss* ptcBoss;
 if(GetCreature().GetTC(&ptcBoss)) {
  long phase=ptcBoss->GetBossPhase();
  if(ptcBoss->HasBossDef()) GetCreature().SetHealth(float(ptcBoss->GetBossDef().PhaseHealthPercent[phase])*GetCreature().GetMaxHealth()*0.01f);
  ptcBoss->SetNextBossPhase(phase+1,false);
 }
 Terminate();
}