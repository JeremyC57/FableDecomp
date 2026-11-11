// CCreatureAction_GenericLeaderCommand::Initialise -- react to combat knockdowns or choose a random order
// Inline-budget: /d2inlT61 preserves native iterator call while expanding bitset/accessors; T60 outlines a required helper.
// Retail: 008bf800,570 bytes; virtual thiscall.
// Leaf: ?Initialise@CCreatureAction_GenericLeaderCommand@@UAEXXZ
// Flags: /d2inlT61
#define FABLE_RETAIL_LEADER_INITIALISE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureAction_GenericLeaderCommand.h"
#include "engine/CTCCombatLeader.h"
#include "engine/CCombatManager.h"
#include "engine/CCombatDialogueDef.h"
#include "engine/CWorld.h"
#include "engine/GFRandom.h"
#include "engine/NCreatureMode.h"
#include "engine/ThingAlive.h"
void CCreatureAction_GenericLeaderCommand::Initialise()
{
 bool cheer=false;
 bool embarassed=false;
 CCharString animation_name("LEADER_IDLE");
 CTCCombatLeader* pCombatLeaderTC;
 if(PCreature->GetTC(&pCombatLeaderTC)) {
  if(IS_THING_ALIVE(PTarget)&&(PTarget->GetTypeFlags()&8)) {
   CThingCreatureBase* pcreature_defender=GFDebugPDynamicCast(PTarget.Get(),(CThingCreatureBase*)0);
   const CCombatManager& comb_man=GetCombatManager();
   const std::map<unsigned __int64,CThingCreatureBase*>& combatants=comb_man.GetCombatantInfo(*pcreature_defender).DefenderInfo.Attackers;
   for(std::map<unsigned __int64,CThingCreatureBase*>::const_iterator it=combatants.begin();it!=combatants.end();++it) {
    const CThingCreatureBase* pattacker=it->second;
    if(NCreatureMode::IsKnockedDown(*pattacker))embarassed=true;
   }
   if(NCreatureMode::IsKnockedDown(*PTarget))cheer=true;
  }
  CDefPointer<const CCombatDialogueDef> p_dialogue_def=GetCombatManager().GetCombatDialogueManager().GetPDialogueDef();
  if(cheer) {
   animation_name="LEADER_CHEER";
   pCombatLeaderTC->SetFrameOfLastEventReaction(GetWorld().GetFrame());
   pCombatLeaderTC->SetLastEventReaction(0);
   CThingCreatureBase* creature=PCreature.Get();
   GetCombatManager().GetCombatDialogueManager().SaySpeech(*creature,p_dialogue_def->FoeKnockedDownByColleague);
  } else if(embarassed) {
   animation_name="LEADER_DISAPPOINTED";
   pCombatLeaderTC->SetFrameOfLastEventReaction(GetWorld().GetFrame());
   pCombatLeaderTC->SetLastEventReaction(1);
   CThingCreatureBase* creature=PCreature.Get();
   GetCombatManager().GetCombatDialogueManager().SaySpeech(*creature,p_dialogue_def->MenLosing);
  } else if(GFRandom(100,GetWorld().GetWorldSeed())<20) {
   animation_name="LEADER_GIVE_ORDERS";
   pCombatLeaderTC->SetFrameOfLastEventReaction(GetWorld().GetFrame());
   pCombatLeaderTC->SetLastEventReaction(2);
   CThingCreatureBase* creature=PCreature.Get();
   GetCombatManager().GetCombatDialogueManager().SaySpeech(*creature,p_dialogue_def->Encourage);
  }
 }
 SetCombatAnimation(animation_name);
 CCombatActionBase::Initialise();
}