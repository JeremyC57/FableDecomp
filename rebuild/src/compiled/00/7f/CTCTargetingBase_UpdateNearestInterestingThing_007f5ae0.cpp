// CTCTargetingBase::UpdateNearestInterestingThing -- refresh perceived interest and preserve nearby script/current choices.
// Retail: 007f5ae0,544 bytes.
// Leaf: ?UpdateNearestInterestingThing@CTCTargetingBase@@MAEXXZ
// Flags: none
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_OPERATORS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#include "engine/CThingAICreature.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/NTargeting_ThingFilters.h"
#include "engine/CFilter_Bind3.h"
#include "engine/C2DBoxI.h"
#include "engine/ThingAlive.h"
void CTCTargetingBase::UpdateNearestInterestingThing()
{
 if((GetWorld().GetFrame()+(__int64)GetParentThing().GetUniqueID())%CMainGameComponent::GetConstantFPS()!=0)return;
 if(ScriptOverrideIgnoreEverything)return;
 const C3DVector& pos=GetParentThing().GetPos();
 if(PScriptOverrideInterestingThing!=0) {
  if((PScriptOverrideInterestingThing->GetPos()-pos).GetMagnitude2D()<4.0f) {
   SetPNearestInterestingThing(PScriptOverrideInterestingThing.Get());return;
  }
 }
 const CThingCreatureBase& parent_creature=static_cast<const CThingCreatureBase&>(GetParentThing());
 C2DBoxI area(pos.ToC2DCoordI(),4);
 CThingPhysical* pnearest_thing;
 PeekThingSearchTools().GetPBestThingInRoughArea(area,
  GFFilter_Bind3(CThingFilter_IsInterestingToCreature(parent_creature),CThingFilter_IsInCreatureFOV(parent_creature,false),CThingFilter_IsPerceptible()),
  CThingCompare_Nearest(pos),&pnearest_thing);
 if(IS_THING_ALIVE(pnearest_thing)) {
  if(!CThingFilter_IsInSameVolumeContextAsThing(parent_creature,false)(*pnearest_thing))pnearest_thing=0;
  else {
   const CWorldMap& wmap=GetWorld().PeekWorldMap();
   if(wmap.GetNavigationLayerAt(pnearest_thing->GetPos(),0)!=wmap.GetNavigationLayerAt(parent_creature.GetPos(),0))pnearest_thing=0;
  }
 }
 if(PNearestInterestingThing!=0 && (!pnearest_thing || pnearest_thing->GetType()!=2) && parent_creature.IsThingInFOV(*PNearestInterestingThing,false)) {
  const C3DVector& current=PNearestInterestingThing->GetPos();
  if(area.ContainsPoint(C2DCoordI((long)current.X,(long)current.Y)))return;
 }
 SetPNearestInterestingThing(pnearest_thing);
}