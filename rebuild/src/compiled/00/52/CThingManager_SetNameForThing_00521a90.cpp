// CThingManager::SetNameForThing(uid,new_name) -- assign an NPC display name and initialize a new script name.
// Leaf: ?SetNameForThing@CThingManager@@QAEX_KABVCWideString@@@Z
// Retail: 00521a90,78 bytes,thiscall
// Flags: none
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHINGMANAGER_NPCNAMES
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "engine/CThingManager.h"
void CThingManager::SetNameForThing(unsigned __int64 uid,const CWideString& new_name)
{
 std::map<unsigned __int64,std::pair<CCharString,CWideString> >::const_iterator name_iter=NPCNameMap.find(uid);
 if(name_iter!=NPCNameMap.end()){
  NPCNameMap[uid].second=new_name;
 }else{
  NPCNameMap[uid].first="";
  NPCNameMap[uid].second=new_name;
 }
}