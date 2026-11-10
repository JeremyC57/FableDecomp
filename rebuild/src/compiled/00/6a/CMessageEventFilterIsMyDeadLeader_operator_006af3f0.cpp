// CMessageEventFilter_IsMyDeadLeader::operator() -- accept death messages for this creature's leader.
// Retail: 0x006AF3F0, 160 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ??RCMessageEventFilter_IsMyDeadLeader@@QBE_NABVCMessageEvent@@@Z
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREDEF_LEADER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENT_TARGET_ORIGINAL
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CMessageEventFilter_IsMyDeadLeader.h"
#include "engine/EMessageEventType.h"
#include "engine/CMessageEvent.h"
#include "engine/CGameDefinitionManager.h"
bool CMessageEventFilter_IsMyDeadLeader::operator()(const CMessageEvent& test_event)const
{
 if(test_event.Type==MESSAGE_EVENT_DEATH){
  CThing* dead_thing=test_event.GetPTarget<CThing>();
  if(dead_thing){
   CDefPointer<const CThingCreatureDef> def;
   if(Creature.PeekDefinitionManager().GetDef(Creature.GetDefGlobalIndex(),&def)){
    if(def.Get()&&def->LeaderTypeDef==dead_thing->GetDefGlobalIndex())return true;
   }
  }
 }
 return false;
}