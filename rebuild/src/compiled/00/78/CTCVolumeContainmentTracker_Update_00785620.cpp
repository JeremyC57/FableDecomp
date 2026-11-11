// CTCVolumeContainmentTracker::Update -- track containing buildings, propagate lighting and ring a shop door bell
// Leaf: ?Update@CTCVolumeContainmentTracker@@QAEXXZ
// Retail: 00785620,1301 code bytes; assigned1312 includes11 alignment bytes.
// Flags: none
#define FABLE_RETAIL_CTCVOLUMECONTAINMENTTRACKER_UPDATE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#define FABLE_RETAIL_TC_VOLUME_CONTAINMENT_TRACKER_DEF
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL
#define FABLE_RETAIL_CTCCARRYING_ALLCARRIED
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_CASOUNDBANK_SOUNDINDEX
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CASOUNDMANAGER_ENABLEFLAGS
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CWORLD
#include "engine/CThingBuilding.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/CTCVolumeContainmentTrackerDef.h"
#include "engine/CWorld.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_OverlappingSphere.h"
#include "engine/CThingFilter_PosInContainmentVolume.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCCarrying.h"
#include "engine/CShopDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CASoundManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CASoundBank.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CSoundInit.h"
#include "engine/CSoundPair.h"
#include "engine/C2DBoxI.h"
#include "engine/ThingAlive.h"
extern const CCharString BUILDING_BOUNDRY;
extern CASoundManager* PSoundManager;
void CTCVolumeContainmentTracker::Update()
{
 if(UpdateLightingChannels) {
  CurrentThemeChannel.ChannelIndex0=0;
  CurrentThemeChannel.ChannelIndex1=0;
  CurrentThemeChannel.ChannelBlendFactor=0;
 }
 CThing* last_thing_inside=PThingInside.Get();
 C3DVector parent_pos=GetParentThing().GetPos();
 CThingBuilding* pthing;
 if(GetWorld().PeekThingSearchTools().GetPFirstLeafThing(GFFilter_Bind2(CThingFilter_OverlappingSphere(parent_pos,0),CThingFilter_PosInContainmentVolume(parent_pos,BUILDING_BOUNDRY)),&pthing)) {
  PThingInside=pthing;
  if(UpdateLightingChannels&&!PDef->CalculateBlendedLightingChannel) {
   NGraphicAppearance::CTCGraphicAppearance* graphic_app=0;
   if(pthing->GetTC(&graphic_app))CurrentThemeChannel=graphic_app->PeekLightingChannel();
  }
 } else PThingInside=(CThing*)0;
 if(UpdateLightingChannels) {
  if(PDef->CalculateBlendedLightingChannel)CalculateBlendedLightingChannel(CurrentThemeChannel);
  NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance=GetParentThing().GetTCGraphicAppearance();
  if(ptc_graphic_appearance) {
   const CEnginePrimitiveThemeChannelDesc& existing_channel=ptc_graphic_appearance->PeekLightingChannel();
   if(existing_channel.ChannelIndex0!=CurrentThemeChannel.ChannelIndex0||existing_channel.ChannelIndex1!=CurrentThemeChannel.ChannelIndex1||existing_channel.ChannelBlendFactor!=CurrentThemeChannel.ChannelBlendFactor) {
    ptc_graphic_appearance->SetLightingChannel(CurrentThemeChannel,true,true);
    ptc_graphic_appearance->SetAsChanged(4);
    CTCCarrying* ptc_carrying;
    if(GetParentThing().GetTC(&ptc_carrying)) {
     std::vector<CThing*> carried_things;
     ptc_carrying->GetAllCarriedThings(carried_things);
     for(long c1=0;c1<carried_things.size();++c1) {
      CThing* pcarried_thing=carried_things[c1];
      if(IS_THING_ALIVE(pcarried_thing)&&pcarried_thing->GetTCGraphicAppearance()) {
       pcarried_thing->GetTCGraphicAppearance()->SetLightingChannel(CurrentThemeChannel,true,true);
       pcarried_thing->GetTCGraphicAppearance()->SetAsChanged(4);
      }
     }
    }
   }
  }
 }
 if(PSoundManager&&PSoundManager->SoundEnabled&&PreviouslyUpdated&&last_thing_inside!=PThingInside.Get()&&(GetParentThing().GetType()==1||GetParentThing().GetType()==2)) {
  CDefPointer<const CShopDef> shop_def;
  if((IS_THING_ALIVE(PThingInside)&&PThingInside->HasTC(TCI_SHOP)&&!PThingInside->HasTC(TCI_TAVERN)&&PThingInside->GetPBaseDef()->GetSubDef(&shop_def)&&shop_def->IsStockDisplayPermanent)||
     (IS_THING_ALIVE(last_thing_inside)&&last_thing_inside->HasTC(TCI_SHOP)&&!last_thing_inside->HasTC(TCI_TAVERN)&&last_thing_inside->GetPBaseDef()->GetSubDef(&shop_def)&&shop_def->IsStockDisplayPermanent)) {
   CThing* sound_thing=0;
   const C3DVector& pos=GetParentThing().GetPos();
   C2DBoxI box(C2DCoordI((long)pos.X,(long)pos.Y),3);
   if(PeekThingSearchTools().GetPBestThingInRoughArea(box,CThingFilter_HasTC(TCI_DOOR),CThingCompare_Nearest(GetParentThing().GetPos()),&sound_thing)) {
    CTCSoundPlayer* sound_player;
    if(sound_thing->GetTC(&sound_player)) {
     long sound;
     {
      CCharString symbol("SND_SHOPBELL_01");
      sound=CMainGameComponent::Get()->GetSampleBank().GetSoundIndexFromSymbol(symbol);
     }
     if(sound) {
      CSoundInit init;
      init.GainFaderBits=0x8010;
      sound_player->PlayAttachedSample(CSoundPair(sound,sound),init,true,false,false);
     }
    }
   }
  }
 }
 PreviouslyUpdated=true;
}