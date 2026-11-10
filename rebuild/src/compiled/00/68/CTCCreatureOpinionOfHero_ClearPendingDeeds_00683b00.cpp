// CTCCreatureOpinionOfHero::ClearPendingDeeds() -- resolve pending deeds against the hero log, then remove them.
// Leaf: ?ClearPendingDeeds@CTCCreatureOpinionOfHero@@QAEXXZ
// Retail:00683b00,261 bytes,thiscall
// Flags: none
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST_REVERSE
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_COMPLETE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_POPBACK
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_DEEDS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/EOpinionDeedType.h"
#include "engine/CTCCreatureOpinionOfHero.h"

#include "stlport/fable_map.h"
#include "engine/COpinionDeed.h"
#include "engine/CTCHeroOpinionDeedLog.h"
void CTCCreatureOpinionOfHero::ClearPendingDeeds()
{
 const COpinionDeed* pdeed=0;
 CThingPlayerCreature* player=GetPlayerManager().GetMainPlayer().GetPControlledCreature();
 if(player){
  CTCHeroOpinionDeedLog* ptc_deedlog;
  player->GetTC(&ptc_deedlog);
  while(!PendingDeeds.empty()){
   pdeed=ptc_deedlog->GetDeedFromHandle(*PendingDeeds.rbegin());
   if(pdeed)KnownDeeds[pdeed->DeedType].push_front(pdeed->DeedHandle);
   PendingDeeds.pop_back();
  }
 }else{
  PendingDeeds.clear();
 }
}