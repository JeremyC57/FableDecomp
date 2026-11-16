// 00972f30 CAIStateGroup_JackOfBladesStage2::SetupArena
// Flags: /GS
// Leaf: ?SetupArena@CAIStateGroup_JackOfBladesStage2@@MAEXXZ
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_C2DCOORDI_COPY
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_OVERFLOW_SCOPE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ORIGINAL_SETPTR_PASS22
#include <stdio.h>
#define FABLE_RETAIL_JACK_SETUPARENA_PASS23
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_FULL
#define FABLE_RETAIL_CCAMERA_CTORS
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_CTOR
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#define FABLE_RETAIL_CTHING_TYPEFLAGS
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_CTORS
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_JackOfBladesStage2.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasDefGlobalIndex.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingFilter_HasScriptName.h"
#include "engine/CCameraManagerSetDef.h"
#include "engine/CGameCameraManager.h"
void CAIStateGroup_JackOfBladesStage2::SetupArena()
{
 CAIStateGroup_JackOfBladesBase::SetupArena();
 CThing* pMarkerThing=0;
 long markerNumber=0;
 char markerName[25];
 for(;;){
  ++markerNumber;
  pMarkerThing=0;
  sprintf(markerName,"jack_pillar%d",markerNumber);
  PeekThingSearchTools().GetPFirstThingInRoughArea(C2DBoxI(GetCreature().GetPos().ToC2DCoordI(),100),CThingFilter_HasScriptName(markerName),&pMarkerThing);
  if(IS_THING_ALIVE(pMarkerThing))PillarMarkers.push_back(pMarkerThing);
  if(!IS_THING_ALIVE(pMarkerThing))break;
 }
 std::vector<CThing*> things;
 long pillar_def_index=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("OBJECT_FRESCO_DOME_PILLAR_01");
 PeekThingSearchTools().GetAllThingsInRoughArea<CThing>(C2DBoxI(GetCreature().GetPos().ToC2DCoordI(),100),CThingFilter_HasDefGlobalIndex(pillar_def_index),things);
 for(std::vector<CThing*>::iterator it=things.begin();it!=things.end();it++){
  CThing* pillar=*it;
  Pillars.push_back(pillar);
 }
 pMarkerThing=0;
 PeekThingSearchTools().GetPFirstThingInRoughArea(C2DBoxI(GetCreature().GetPos().ToC2DCoordI(),100),CThingFilter_HasScriptName("JACK_TO_LOOK"),&pMarkerThing);
 if(pMarkerThing)PCutsceneLookAtMarker=pMarkerThing;
 CGameCameraManager* pcamera_manager;
 if(GetWorld().GetGameCamera()->GetCameraManager(&pcamera_manager)){
  CDefPointer<const CCameraManagerSetDef> pcam_man_def;
  PeekDefinitionManager().GetDef("CAMERA_MANAGER_SET_JACK_BOSS_FIGHT",&pcam_man_def);
  pcamera_manager->SetCameraManagerSetDef(pcam_man_def);
 }
}