// CTCInventoryMap::OnSerialise(CPersistContext &,bool) -- transfer the inventory map persistence state.
// Leaf: ?OnSerialise@CTCInventoryMap@@MAEXAAVCPersistContext@@_N@Z
#define FABLE_RETAIL_INVENTORY_MAP_SERIALISE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CWIDESTRING_COPY
#define FABLE_STLPORT_HTTP
#define FABLE_RETAIL_QUEST_COMPLETION
#define FABLE_STLPORT_CONSTRUCTORS
#define FABLE_STLPORT_UI_ACTIONS
#include "engine/CTCInventoryMap.h"
#include "engine/CPersistContext.h"
#include "engine/CThingMarker.h"
#include "engine/CTCTeleporter.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingManager.h"
#include "engine/CWorldMap.h"
#include "engine/CRegion.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CMainGameComponent.h"

#pragma auto_inline(off)
template<> CVectorMap<ETCInterfaceType,CTCBase*>::const_iterator
CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound(const ETCInterfaceType& key) const
{
 const_iterator first=begin();long count=end()-first;
 while(count>0) {long half=count/2;const_iterator middle=first+half;
  if(middle->first<key) {first=middle+1;count-=half+1;} else count=half;
 }
 return first;
}
template<> void CThingSearchTools::GetAllLeafThings<CThingMarker,std::list<const CThingMarker*>,CThingFilter_HasTC>(const CThingFilter_HasTC& filter,std::list<const CThingMarker*>& container)const
{
 const std::list<CClass*>& things=PeekTypeList(CThingMarker::StatGetType());
 for(std::list<CClass*>::const_iterator it=things.begin();it!=things.end();++it) {
  const CThingMarker* thing=static_cast<const CThingMarker*>(*it);
  if(thing->IsIncludedInThingScans() && filter(*thing)) container.push_back(thing);
 }
}
namespace std {
template<> void list<pair<const CRegion*,const CRegion*> >::push_back(const pair<const CRegion*,const CRegion*>& value)
{
 insert(end(),value);
}
}
#pragma auto_inline(on)

void CTCInventoryMap::OnSerialise(CPersistContext& context,bool in_game)
{
 NInventory::CTCInventoryBase::OnSerialise(context,in_game);
 if(!PeekWorld().IsSerialisingHeroState()) {
  context.TransferVector("MiniMapMarkers",MiniMapMarkers);
  context.TransferVector("VisitedRegions",VisitedRegions);
  context.TransferVector("UIRegionsInfo",UIRegionsInfo);
 }
 if(in_game) {
 if(context.IsSaving()) {
  if(!PeekWorld().IsSerialisingHeroState()) {
   std::vector<unsigned __int64> uids;
   std::list<const CThingMarker*> marker_list;
   PeekWorld().PeekThingSearchTools().GetAllLeafThings<CThingMarker>(CThingFilter_HasTC(TCI_TELEPORTER),marker_list);
   for(std::list<const CThingMarker*>::const_iterator it=marker_list.begin();it!=marker_list.end();++it) {
    const CThingMarker* pmarker=*it;
    const CTCTeleporter* ptc_teleporter=0;
    pmarker->PeekTC(&ptc_teleporter);
    if(ptc_teleporter->IsActive()) {
     unsigned __int64 unique_id=pmarker->GetUniqueID();
     if(unique_id>0) {
      unique_id=GetThingManager().RemapUIDForSerialisation(true,pmarker->IsLevelLess(),pmarker->GetCurrentMapNumber(),unique_id);
      uids.push_back(unique_id);
     }
    }
   }
   context.TransferVector("ActiveTeleportUID",uids);
  }
  if(!PeekWorld().IsSerialisingHeroState()) {
   std::vector<unsigned long> from,to;
   for(std::list<std::pair<const CRegion*,const CRegion*> >::const_iterator it=RegionsCorrespondances.begin();it!=RegionsCorrespondances.end();++it) {
    from.push_back(GetWorldMap().GetRegionNumberFromName(it->first->GetName()));
    to.push_back(GetWorldMap().GetRegionNumberFromName(it->second->GetName()));
   }
   context.TransferVector("RegionsFrom",from);
   context.TransferVector("RegionsTo",to);
  }
  std::vector<CCharString> logbook_name_tag,logbook_abbreviated_name_tag,logbook_text_tag;
  for(unsigned long count=0;count<LogBookNameTag.size();++count) logbook_name_tag.push_back(LogBookNameTag[count].ToCharString());
  for(unsigned long count=0;count<LogBookAbbreviatedNameTag.size();++count) logbook_abbreviated_name_tag.push_back(LogBookAbbreviatedNameTag[count].ToCharString());
  for(unsigned long count=0;count<LogBookTextTag.size();++count) logbook_text_tag.push_back(LogBookTextTag[count].ToCharString());
  context.TransferVector("logbook_name_tag",logbook_name_tag);
  context.TransferVector("logbook_abbreviated_name_tag",logbook_abbreviated_name_tag);
  context.TransferVector("logbook_text_tag",logbook_text_tag);
  context.TransferVector("LogBookCategory",LogBookCategory);
 } else {
  if(!PeekWorld().IsSerialisingHeroState()) {
   SerialisedTeleporterUIDs.Reset(new std::vector<unsigned __int64>);
   context.TransferVector("ActiveTeleportUID",*SerialisedTeleporterUIDs);
   RemapUIDsForLoading(*SerialisedTeleporterUIDs);
   std::vector<unsigned long> from,to;
   context.TransferVector("RegionsFrom",from);
   context.TransferVector("RegionsTo",to);
   RegionsCorrespondances.clear();
   for(unsigned long count=0;count<from.size()&&count<to.size();++count) {
    std::pair<const CRegion*,const CRegion*> region_pair(&GetWorldMap().GetRegion(from[count]),&GetWorldMap().GetRegion(to[count]));
    RegionsCorrespondances.push_back(region_pair);
   }
  }
  std::vector<CCharString> logbook_name_tag,logbook_abbreviated_name_tag,logbook_text_tag;
  LogBookTextTag.clear();LogBookAbbreviatedNameTag.clear();LogBookNameTag.clear();LogBookCategory.clear();
  context.TransferVector("logbook_name_tag",logbook_name_tag);
  context.TransferVector("logbook_abbreviated_name_tag",logbook_abbreviated_name_tag);
  context.TransferVector("logbook_text_tag",logbook_text_tag);
  context.TransferVector("LogBookCategory",LogBookCategory);
  for(unsigned long count=0;count<logbook_name_tag.size();++count) LogBookNameTag.push_back(logbook_name_tag[count].ToWideString());
  for(unsigned long count=0;count<logbook_abbreviated_name_tag.size();++count) LogBookAbbreviatedNameTag.push_back(logbook_abbreviated_name_tag[count].ToWideString());
  for(unsigned long count=0;count<logbook_text_tag.size();++count) LogBookTextTag.push_back(logbook_text_tag[count].ToWideString());
  for(unsigned long count=0;count<LogBookNameTag.size()&&count<LogBookAbbreviatedNameTag.size()&&count<LogBookTextTag.size()&&count<LogBookCategory.size();++count)
   CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPlayerHeroLogBook().AddEntry(LogBookNameTag[count],LogBookAbbreviatedNameTag[count],LogBookTextTag[count],LogBookCategory[count],false);
 }
}
}
