// 008da7a0 NCreatureMode::CModeDrained::OnDeactivate
// Leaf: ?OnDeactivate@CModeDrained@NCreatureMode@@UAEXXZ
// Flags: none
// Retail:008da7a0,227 bytes.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_DEACTIVATE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_DRAINED
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_REMOVEMODE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/EPlayerMode.h"
#include "engine/NCreatureMode_CModeDrained.h"
#include "engine/ThingAlive.h"
void NCreatureMode::CModeDrained::OnDeactivate()
{
 if(GetParentThing().GetType()==1){
  CThingAICreature& pai_creature=static_cast<CThingAICreature&>(GetParentThing());
  pai_creature.SuspendAI(false,0);
  pai_creature.ForceAIReset();
 }else if(GetParentThing().GetType()==2){
  CThingPlayerCreature& pplayer_creature=static_cast<CThingPlayerCreature&>(GetParentThing());
  pplayer_creature.GetPlayer().RemoveMode(PLAYER_MODE_PARALYSED);
 }
 if(IS_THING_ALIVE(PDrainer)){
  CThingCreatureBase& creature=static_cast<CThingCreatureBase&>(GetParentThing());
  CTCCreatureModeManager* pTCModeMan=0;
  if(PDrainer->GetTC(&pTCModeMan))pTCModeMan->RemoveMode(DrainerMode);
  PDrainer=CIntelligentPointer<CThing>(0);
 }
 CModeStandBase::OnDeactivate();
}