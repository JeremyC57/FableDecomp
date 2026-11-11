// CTCPerceiveThing::UpdateHearing -- find the latest audible noise and update awareness.
// Retail: 00744fb0,355 bytes.
// Leaf: ?UpdateHearing@CTCPerceiveThing@@AAEXXZ
// Flags: none
#define FABLE_RETAIL_HEARING
#define FABLE_RETAIL_CTCPERCEIVETHING_SMELL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGEMEMBER
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTLEAF
#define FABLE_RETAIL_CWORLD
#include "engine/CTCPerceiveThing.h"
#include "engine/CPerceivedThingDef.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CThingNoise.h"
#include "engine/CThingNoiseFilters.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CWorld.h"
void CTCPerceiveThing::UpdateHearing()
{
 Heard=false;
 CTCVillageMember* tc_village_member=0;
 if(GetParentThing().HasTC((ETCInterfaceType)0xb4)&&GetParentThing().GetTC(&tc_village_member)) {
  CTCVillage* ptc_village=tc_village_member->GetPVillage();
  if(ptc_village && GetWorld().GetFrame()<ptc_village->GetFrameBribeEnds())return;
 }
 const CThing* pperceived_thing=GetPerceivedThing();
 CThingNoise* pnoise=0;
 GetThingSearchTools().GetPBestLeafThing(
  GFFilter_Bind2(CThingFilter_NoiseCameFromThing(*pperceived_thing),CThingFilter_NoiseCanBeHeardFromPosition(GetParentThing().GetPos(),GetSoundRadius())),
  CThingCompare_GetMostRecentlyCreatedThing(),&pnoise);
 if(IS_THING_ALIVE(pnoise)) {
  Heard=true;
  if(!(IsAwareOfThing()&&IsAwarenessOfThingDirect())) {
   LastPerceivedPos=pnoise->GetPos();AwarenessTimer=PDef->AwarenessTimer;
  } else SetAsDirectlyAwareOfPerceivedThing();
 }
 ReactToAlerts();
}