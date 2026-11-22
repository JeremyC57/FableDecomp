#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CCREATUREACTION_SPIRITHITRESPONSE_PASS26
// Leaf: ?OnHit@CThingPlayerCreature@@UAEXAAVCHitParameters@@@Z
// Flags: (none)
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_ONHIT_PASS26
#define FABLE_RETAIL_CPLAYERGUI_ONHEROHIT_PASS26
#define FABLE_RETAIL_CTCCARRYING_PLAYERHIT_PASS26
#define FABLE_RETAIL_CJOYSTICK_INITRUMBLE_BODY_PASS26
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETATTACKEDBYCREATURE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CHITPARAMETERS_VECTOR_MEMBERS
#define FABLE_RETAIL_CJOYSTICK
#define FABLE_RETAIL_CJOYSTICK_VIRTUALS
#define FABLE_RETAIL_CINPUTMANAGER_JOYSTICKS
#define FABLE_RETAIL_CTCENEMY_ARETHINGSENEMIES
#define FABLE_RETAIL_CTCCARRYING_ISTHINGCARRYINGMELEEWEAPON
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_NENTITYEVENTS_BASE_ORIGINAL_PASS26
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CMODEBASE_ORIGINAL_GETTYPE_PASS26
#include "engine/CThingPlayerCreature.h"
#include "engine/CInputManager.h"
#include "engine/CHitParameters.h"
#include "engine/CTCEnemy.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_CEventHitThing.h"
#include "engine/CCreatureAction_SpiritHitResponse.h"
#include "engine/NCreatureMode.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/ThingAlive.h"
#include "engine/CPlayerGui.h"
extern CInputManager* PInputManager;
void CThingPlayerCreature::OnHit(CHitParameters& hit_params)
{
 CThingCreatureBase::OnHit(hit_params);
 PInputManager->GetJoystick(GetJoystickDeviceNumber()).AddRumble(2,CJoystick::CInitRumble(1.0f,0.5f,0.0f,0.4f,false),false);
 CThing* phitting_thing=hit_params.GetPThingHitting();
 if(IS_THING_ALIVE(phitting_thing)&&(phitting_thing->GetTypeFlags()&8)) {
  CTCEntityEvents* ptc_entity_events;
  if(phitting_thing->GetTC(&ptc_entity_events)) {
   bool decapitate=false;
   bool hit_enemy=CTCEnemy::AreThingsEnemies(*this,*phitting_thing);
   bool used_melee_weapon=CTCCarrying::IsThingCarryingMeleeWeapon(*phitting_thing);
   bool used_ranged_weapon=CTCCarrying::IsThingCarryingRangedWeapon(*phitting_thing);
   ptc_entity_events->AddEvent(new NEntityEvents::CEventHitThing(*phitting_thing,*this,hit_enemy,used_melee_weapon,used_ranged_weapon,hit_params.GetPWeapon(),hit_params.IsFlourish(),decapitate));
  }
  SetAttackedByCreature(static_cast<CThingCreatureBase*>(phitting_thing));
 }
 const CTCCreatureModeManager* ptc_mode_manager;
 if(PeekTC(&ptc_mode_manager)&&ptc_mode_manager->GetCurrentMode().GetType()==NCreatureMode::MODE_CARRYING_GENERIC_BOX) {
  CTCCarrying* ptc_carrying;
  GetTC(&ptc_carrying);
  if(!ptc_carrying->IsPrimarySlotFree())ptc_carrying->RemoveThingFromPrimarySlot(false);
 }
 if(HasTC(TCI_COOP_SPIRIT))SetCurrentAction(CCreatureAction_SpiritHitResponse(*this,hit_params.GetHitDirection()));
 CPlayerGui::Get().OnHeroHit(hit_params.GetDamageToThing(*this));
}